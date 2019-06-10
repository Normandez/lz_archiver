#ifndef CARCHIVER_H
#define CARCHIVER_H

#include <iostream>
#include <fstream>

#include <string>

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

};

#endif // CARCHIVER_H
