#ifndef CARCHIVER_H
#define CARCHIVER_H

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

class CArchiver
{
public:
	explicit CArchiver();
	~CArchiver();

	bool SetInputFile( const std::string& input_file_name );
	bool SetOutputFile( const std::string& output_file_name );

	void CompressLz77();
	void DecompressLz77();

	void CompressLz78();
	void DecompressLz78();

private:
	std::ifstream m_in_file_strm;
	std::ofstream m_out_file_strm;

	struct SLz77SaveNode
	{
		int m_offset;
		int m_length;
		char m_next_char;

		void Init( int offset, int length, char next_char )
		{
			m_offset = offset;
			m_length = length;
			m_next_char = next_char;
		}

		void Serialize( char* serialized_str )
		{
			int* int_it = (int*) serialized_str;
			*int_it = this->m_offset, int_it++;
			*int_it = this->m_length, int_it++;
	
			char* ch_it = (char*) int_it;
			*ch_it = this->m_next_char, ch_it++;
		}

		void Deserialize( char* serialized_str )
		{
			int* int_it = (int*) serialized_str;
			this->m_offset = *int_it, int_it++;
			this->m_length = *int_it, int_it++;
	
			char* ch_it = (char*) int_it;
			this->m_next_char = *ch_it, ch_it++;
		}
	};

};

#endif // CARCHIVER_H
