#include "SLaunchOptions.h"
#include "CArchiver.h"

int main( int argc, char** argv )
{
	SLaunchOptions launch_opt = ParseInputParams( argc, argv );

	if(!launch_opt.m_is_valid)
	{
		return 1;
	}
	else
	{
		CArchiver archiver;

		if( !archiver.SetInputFile(launch_opt.m_input_file) )
		{
			std::cout << "Bad input file" << std::endl;
			return 2;
		}
		if( !archiver.SetOutputFile(launch_opt.m_output_file) )
		{
			std::cout << "Bad output file" << std::endl;
			return 3;
		}

		if( launch_opt.m_archive_mode == EArchiveMode_Compress )
		{
			if( launch_opt.m_archive_type == EArchiveType_LZ77 )
			{
				archiver.CompressLz77(launch_opt.m_compression_rate);
			}
			else if( launch_opt.m_archive_type == EArchiveType_LZ78 )
			{
				archiver.CompressLz78();
			}
		}
		else if( launch_opt.m_archive_mode == EArchiveMode_Extract )
		{
			if( launch_opt.m_archive_type == EArchiveType_LZ77 )
			{
				archiver.DecompressLz77();
			}
			else if( launch_opt.m_archive_type == EArchiveType_LZ78 )
			{
				archiver.DecompressLz78();
			}
		}
	}

	return 0;
}
