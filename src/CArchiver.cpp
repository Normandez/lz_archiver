#include "CArchiver.h"

namespace
{
	// Total window size = 32
	static const size_t s_search_buf_size = 20;
	static const size_t s_look_ahead_buf_size = 12;
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
	// SB and LAB definition
	std::string search_buf;
	std::string look_ahead_buf;

	// SB and LAB size setting
	search_buf.reserve( s_search_buf_size );
	look_ahead_buf.reserve(s_look_ahead_buf_size);

	// File compressing loop
	bool is_end_of_file = false;
	while(true)
	{
		// EOF checking
		is_end_of_file = m_in_file_strm.eof();

		if( look_ahead_buf.size() < s_look_ahead_buf_size && !is_end_of_file )	// LAB fill
		{
			look_ahead_buf.push_back( m_in_file_strm.get() );
			continue;
		}
		else	// Compressing
		{
			long length_count, offset_count;
			while( !look_ahead_buf.empty() )	// LAB flush
			{


				if(!is_end_of_file)
				{
					break;
				}
			}
		}

		// EOF handling (exit condition)
		if(is_end_of_file)
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
