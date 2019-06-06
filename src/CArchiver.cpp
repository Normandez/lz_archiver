#include "CArchiver.h"

namespace
{
	static const size_t s_wnd_size = 32;
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
	while( !m_in_file_strm.eof() )
	{

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
