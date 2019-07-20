#include "SLaunchOptions.h"
#include "CArchiver.h"

#include <stdlib.h>

#include <exception>

void LogTerminate()
{
	std::cerr << "terminate() called" << std::endl;
}

int main( int argc, char** argv )
{
	std::set_terminate(LogTerminate);

	// General exception handler
	try
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
					std::cout << "Method: 'LZ77'\nCompression rate: '";
					switch(launch_opt.m_compression_rate)
					{
					case 0:
						std::cout << "low'";
						break;
					case 1:
						std::cout << "middle'";
						break;
					case 2:
						std::cout << "high'";
						break;
					}
					std::cout << std::endl;

					archiver.CompressLz77(launch_opt.m_compression_rate);
				}
				else if( launch_opt.m_archive_type == EArchiveType_LZ78 )
				{
					std::cout << "Method: 'LZ78'" << std::endl;

					archiver.CompressLz78();
				}
			}
			else if( launch_opt.m_archive_mode == EArchiveMode_Extract )
			{
				if( launch_opt.m_archive_type == EArchiveType_LZ77 )
				{
					std::cout << "Method: 'LZ77'" << std::endl;

					archiver.DecompressLz77();
				}
				else if( launch_opt.m_archive_type == EArchiveType_LZ78 )
				{
					std::cout << "Method: 'LZ78'" << std::endl;

					archiver.DecompressLz78();
				}
			}

			std::cout << "Done! Output file: '" << launch_opt.m_output_file << "'" << std::endl;
		}
	}
	catch( const std::bad_alloc& bad_alloc_ex )
	{
		std::cerr << "bad_alloc exception handled: " << bad_alloc_ex.what() << std::endl;
		return 4;
	}
	catch( const std::logic_error& logic_error_ex )
	{
		std::cerr << "logic_error exception handled: " << logic_error_ex.what() << std::endl;
		return 5;
	}
	catch( const std::runtime_error& runtime_error_ex )
	{
		std::cerr << "runtime_error exception handled: " << runtime_error_ex.what() << std::endl;
		return 6;
	}
	catch( const std::bad_exception& bad_ex )
	{
		std::cerr << "bad_exception exception handled: " << bad_ex.what() << std::endl;
		return 8;
	}
	catch( const std::exception& ex )
	{
		std::cerr << "common exception handled: " << ex.what() << std::endl;
		return 7;
	}
	catch(...)
	{
		std::cerr << "Unhandled exception thrown" << std::endl;
		throw "Unhandled exception thrown";
	}

	return 0;
}
