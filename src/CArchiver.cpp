#include "CArchiver.h"

namespace
{
	// Total window size = 32
	static const size_t s_search_buf_size = 20000;
	static const size_t s_look_ahead_buf_size = 12000;
}

CArchiver::CArchiver()
{ }

CArchiver::~CArchiver()
{
	if( m_in_file_strm.is_open() ) m_in_file_strm.close();
	if( m_out_file_strm.is_open() ) m_out_file_strm.close();
}

bool CArchiver::SetInputFile( const std::string& input_file_name )
{
	m_in_file_strm.open( input_file_name, std::ios::in | std::ios::binary );
	return m_in_file_strm.is_open();
}

bool CArchiver::SetOutputFile( const std::string& output_file_name )
{
	m_out_file_strm.open( output_file_name, std::ios::out | std::ios::binary | std::ios::trunc );
	return m_out_file_strm.is_open();
}

void CArchiver::CompressLz77()
{
	using namespace std::string_literals;

	// SB, LAB and SM definition
	std::string search_buf;
	std::string look_ahead_buf;
	std::vector<short> search_mask;

	// SB, LAB and SM size setting
	search_buf.reserve(s_search_buf_size);
	look_ahead_buf.reserve(s_look_ahead_buf_size);
	search_mask.reserve(s_look_ahead_buf_size);

	// File compressing loop
	bool is_eof = false;
	while(true)
	{
		// EOF checking
		is_eof = m_in_file_strm.eof();

		if( look_ahead_buf.size() < s_look_ahead_buf_size && !is_eof )	// LAB fill
		{
			look_ahead_buf.push_back( m_in_file_strm.get() );
			continue;
		}
		else	// Compressing
		{
			short length_count, offset_count;
			std::string search_substr;
			while( !look_ahead_buf.empty() )	// LAB flush
			{
				// SM (mask of matches) fill
				for( size_t it = 0; it < look_ahead_buf.size(); it++ )
				{
					search_substr = look_ahead_buf.substr( 0, it + 1 );
					std::reverse( search_substr.begin(), search_substr.end() );
					search_mask.push_back( ( search_buf.find( search_substr, 0 ) != -1 ) ? (short) ( search_buf.find( search_substr, 0 ) + search_substr.size() ) : 0 );
				}

				// SM read
				for( short it = (short) look_ahead_buf.size() - 1; it >= 0; it-- )
				{
					if( search_mask.at(it) )
					{
						offset_count = search_mask.at(it);
						length_count = (short) it + 1;
						break;
					}
					else if( it == 0 )
					{
						offset_count = 0;
						length_count = 0;
					}
				}

				// Ready compressed data
				m_out_file_strm << "<" << offset_count << ", " << length_count << ", " << look_ahead_buf[length_count] << ">";

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
}

void CArchiver::DecompressLz77()
{

}

void CArchiver::CompressLz78()
{

}

void CArchiver::DecompressLz78()
{

}
