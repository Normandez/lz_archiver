#include "CArchiver.h"

namespace
{
	// Total window size = 32KB
	static const size_t s_search_buf_size = 20000;
	static const size_t s_look_ahead_buf_size = 12000;
	static const size_t s_low_compression_rate = 16;
	static const size_t s_middle_compression_rate = 32;
	static const size_t s_high_compression_rate = 64;

	// 32MB
	static const size_t s_input_buf_size = 32768000;

	// CLI components
	static const char* s_compress_progress_line = "Compressing:";
	static const char* s_extract_progress_line = "Extracting:";
	static const char s_progress_bar_edge_char = '|';
	static std::string s_progress_bar_filler = ""; // 1 '#' = 2%
	static std::string s_progress_bar_spacer = "                                                  "; // 50 spaces
}

CArchiver::CArchiver()
	: m_in_file_length(0),
	  m_progress_bar_step(0)
{ }

CArchiver::~CArchiver()
{
	if( m_in_file_strm.is_open() ) m_in_file_strm.close();
	if( m_out_file_strm.is_open() ) m_out_file_strm.close();
}

bool CArchiver::SetInputFile( const std::string& input_file_name )
{
	m_in_file_strm.open( input_file_name, std::ios::in | std::ios::binary );
	if( m_in_file_strm.is_open() )
	{
		// Get total file length
		m_in_file_strm.seekg( 0, m_in_file_strm.end );
		m_in_file_length = (float)m_in_file_strm.tellg();
		m_in_file_strm.seekg( 0, m_in_file_strm.beg );

		return true;
	}
	else
	{
		return false;
	}
}

bool CArchiver::SetOutputFile( const std::string& output_file_name )
{
	m_out_file_strm.open( output_file_name, std::ios::out | std::ios::binary | std::ios::trunc );
	return m_out_file_strm.is_open();
}

void CArchiver::CompressLz77( ECompressionRate rate )
{
	// Define compression rate
	size_t compression_rate;
	switch(rate)
	{
	case 0:
		compression_rate = s_low_compression_rate;
		break;
	case 1:
		compression_rate = s_middle_compression_rate;
		break;
	case 2:
		compression_rate = s_high_compression_rate;
		break;
	default:
		compression_rate = s_middle_compression_rate;
	}

	// SB, LAB and SM definition
	std::string search_buf;
	std::string look_ahead_buf;
	std::vector<long> search_mask;

	// SB, LAB and SM size setting
	search_buf.reserve(s_search_buf_size);
	look_ahead_buf.reserve(s_look_ahead_buf_size);
	search_mask.reserve(compression_rate);

	// Serialization components
	CArchiver::SLz77SaveNode save_node;
	char* serialized_node = new char[ sizeof(CArchiver::SLz77SaveNode) ];

	// File compressing loop
	UpdateProgressBar( 0.0, s_compress_progress_line );
	bool is_eof = false;
	while(true)
	{
		// EOF checking
		is_eof = m_in_file_strm.eof();

		// Progress bar update
		UpdateProgressBar( m_in_file_strm.tellg() / m_in_file_length * 100.0, s_compress_progress_line );

		if( look_ahead_buf.size() < s_look_ahead_buf_size && !is_eof )	// LAB fill
		{
			int read_ch = m_in_file_strm.get();
			if( read_ch != -1 ) look_ahead_buf.push_back(read_ch);
			continue;
		}
		else	// Compressing
		{
			int length_count, offset_count;
			std::string search_substr;
			while( !look_ahead_buf.empty() )	// LAB flush
			{
				// SM (mask of matches) fill
				for( size_t it = 0; it < compression_rate; it++ )
				{
					search_substr = look_ahead_buf.substr( 0, it + 1 );
					std::reverse( search_substr.begin(), search_substr.end() );
					search_mask.push_back( ( search_buf.find( search_substr, 0 ) != -1 ) ? (long) ( search_buf.find( search_substr, 0 ) + search_substr.size() ) : 0 );
				}

				// SM read
				for( long it = (long) ( compression_rate - 1 ); it >= 0; it-- )
				{
					if( search_mask.at(it) )
					{
						offset_count = search_mask.at(it);
						length_count = it + 1;
						break;
					}
					else if( it == 0 )
					{
						offset_count = 0;
						length_count = 0;
					}
				}

				// Compressed data serialization and saving
				try
				{
					save_node.Init( offset_count, length_count, look_ahead_buf.at(length_count) );
					save_node.Serialize(serialized_node);
					m_out_file_strm.write( serialized_node, sizeof(CArchiver::SLz77SaveNode) );
				}
				catch(...)
				{
					for( char ch : look_ahead_buf )
					{
						save_node.Init( 0, 0, ch );
						save_node.Serialize(serialized_node);
						m_out_file_strm.write( serialized_node, sizeof(CArchiver::SLz77SaveNode) );
					}
				}

				// Window shifting and SM flushing
				std::string seek_str = look_ahead_buf.substr( 0, length_count + 1 );
				std::reverse( seek_str.begin(), seek_str.end() );
				search_buf.insert( 0, seek_str );
				look_ahead_buf.erase( 0, length_count + 1 );
				search_mask.clear();

				// EOF handling (LAB flushing condition)
				if(!is_eof)
				{
					break;
				}
			}
		}

		// EOF handling (exit condition)
		if(is_eof)
		{
			break;
		}
	}

	delete[] serialized_node;

	std::cout.flush();
	std::cout << std::endl;
}

void CArchiver::DecompressLz77()
{
	// SB definition
	std::string search_buf = "";

	// SB size setting
	search_buf.reserve( 8 * s_search_buf_size );

	// Serialization components
	CArchiver::SLz77SaveNode save_node;
	char* serialized_node = new char[sizeof( CArchiver::SLz77SaveNode )];

	// File decompressing loop
	UpdateProgressBar( 0.0, s_extract_progress_line );
	while( m_in_file_strm.read( serialized_node, sizeof( CArchiver::SLz77SaveNode ) ) )
	{
		// Deserialization
		save_node.Deserialize(serialized_node);

		// Decompression
		std::string search_substr = "";
		if(save_node.m_offset)
		{
			int search_it = save_node.m_offset - 1;
			int length_count = save_node.m_length;
			while(length_count)
			{
				search_substr += search_buf[search_it--];
				length_count--;
			}
		}
		search_substr += save_node.m_next_char;
		std::reverse( search_substr.begin(), search_substr.end() );

		// SB overflow condition
		if( ( search_buf.size() + save_node.m_length + 1 ) > 8 * s_search_buf_size )
		{
			std::reverse( search_buf.begin(), search_buf.end() );
			m_out_file_strm << search_buf;
			search_buf.clear();
		}

		// SB shifting
		search_buf.insert( 0, search_substr );

		// Progress bar update
		UpdateProgressBar( m_in_file_strm.tellg() / m_in_file_length * 100.0, s_extract_progress_line );
	}

	// SB flush
	std::reverse( search_buf.begin(), search_buf.end() );
	m_out_file_strm << search_buf;

	delete[] serialized_node;

	std::cout.flush();
	std::cout << std::endl;
}

void CArchiver::CompressLz78()
{
	// D(dictionary) and IB(input buffer) def
	std::map<std::string, int> dict;
	std::string input_buf;

	// D and IB init
	dict.insert( { "", 0 } );
	input_buf.reserve(s_input_buf_size);

	// Serialization components
	CArchiver::SLz78Node node;
	char* serialized_node = new char[sizeof( CArchiver::SLz78Node )];

	// File compression loop
	UpdateProgressBar( 0.0, s_compress_progress_line );
	bool is_eof = false;
	std::string search_buf = "";
	while(true)
	{
		// EOF checking
		is_eof = m_in_file_strm.eof();

		// Progress bar update
		UpdateProgressBar( m_in_file_strm.tellg() / m_in_file_length * 100.0, s_compress_progress_line );

		if( input_buf.size() < s_input_buf_size && !is_eof )	// IB filling
		{
			int read_ch = m_in_file_strm.get();
			if( read_ch != -1 ) input_buf.push_back(read_ch);
			continue;
		}
		else	// Compressing
		{
			for( int it = 0; it <= input_buf.size() - 1; it++ )	// IB flushing
			{
				// D searching
				if( dict.find( search_buf + input_buf[it] ) != dict.end() )
				{
					search_buf += input_buf[it];
				}
				else
				{
					// Output writing
					node.Init( dict[search_buf], input_buf[it] );
					node.Serialize(serialized_node);
					m_out_file_strm.write( serialized_node, sizeof(CArchiver::SLz78Node) );

					// D filling
					dict[search_buf + input_buf[it]] = (int) dict.size();
					search_buf.clear();
				}
			}

			input_buf.clear();
		}

		// EOF handling (exit condition)
		if(is_eof)
		{
			break;
		}
	}

	delete[] serialized_node;

	std::cout.flush();
	std::cout << std::endl;
}

void CArchiver::DecompressLz78()
{
	// D(dictionary) def and init
	std::vector<std::string> dict = {""};

	// Serialization components
	CArchiver::SLz78Node node;
	char* serialized_node = new char[sizeof(CArchiver::SLz78Node)];

	// Decompression loop
	UpdateProgressBar( 0.0, s_extract_progress_line );
	std::string buf = "";
	while( m_in_file_strm.read( serialized_node, sizeof( CArchiver::SLz78Node ) ) )
	{
		// Deserialization
		node.Deserialize(serialized_node);
		
		// Decode
		buf = dict[node.dict_pos] + node.next_ch;

		// Output saving
		m_out_file_strm << buf;

		// D filling
		dict.push_back(buf);
		buf.clear();

		// Progress bar update
		UpdateProgressBar( m_in_file_strm.tellg() / m_in_file_length * 100.0, s_extract_progress_line );
	}

	delete[] serialized_node;

	std::cout.flush();
	std::cout << std::endl;
}

void CArchiver::UpdateProgressBar( double new_value, const std::string& process_name )
{
	std::cout.clear();
	std::cout << "\r";
	std::cout.flush();

	if( new_value >= 0.0 && new_value <= 100.0 )
	{
		int buf_step = (int)new_value / 2;
		if( m_progress_bar_step != buf_step )
		{
			s_progress_bar_spacer.resize( s_progress_bar_spacer.size() - 1 );
			s_progress_bar_filler.resize( s_progress_bar_filler.size() + 1, '#' );
			m_progress_bar_step = buf_step;
		}

		std::cout << process_name << " "
					<< s_progress_bar_edge_char << s_progress_bar_filler << s_progress_bar_spacer << s_progress_bar_edge_char
					<< std::showpoint << std::setprecision(4) << new_value << "%" << '\0';
	}
}
