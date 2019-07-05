#include <string>
#include <iostream>

// Global defs
namespace
{
	static const char* s_usage_line = "Usage: -c|-x input_file [--output=output_file] [--type=-lz77|-lz78 [--rate=-l|-m|-h](only for lz77)]";
	static const char* s_archive_ext = ".arch";
};

enum EArchiveMode
{
	EArchiveMode_Compress,
	EArchiveMode_Extract
};

enum EArchiveType
{
	EArchiveType_LZ77,
	EArchiveType_LZ78
};

enum ECompressionRate
{
	ECompressionRate_low,
	ECompressionRate_middle,
	ECompressionRate_high
};

struct SLaunchOptions
{
	bool m_is_valid = true;
	
	EArchiveMode m_archive_mode;
	EArchiveType m_archive_type = EArchiveType_LZ78;
	ECompressionRate m_compression_rate = ECompressionRate_middle;

	std::string m_input_file;
	std::string m_output_file = "";
};

SLaunchOptions ParseInputParams( int argc, char** argv )
{
	SLaunchOptions launch_opt;

	// Arguments parsing
	if( argc < 3 )
	{
		std::cout << "Too few input params.\n" << s_usage_line << std::endl;
		launch_opt.m_is_valid = false;
	}
	else
	{
		for( size_t it = 1; it < argc; it++ )
		{
			std::string arg = argv[it];

			if( it == 1 && arg == "-c" )
			{
				launch_opt.m_archive_mode = EArchiveMode_Compress;
			}
			else if( it == 1 && arg == "-x" )
			{
				launch_opt.m_archive_mode = EArchiveMode_Extract;
			}
			else if( it == 2 )
			{
				launch_opt.m_input_file = arg;
				if( launch_opt.m_archive_mode == EArchiveMode_Extract
					&& launch_opt.m_input_file.compare( launch_opt.m_input_file.size() - 5, 5, s_archive_ext ) != 0 )
				{
					std::cout << "Input file isn't .arch-file\n" << s_usage_line << std::endl;
					launch_opt.m_is_valid = false;
					break;
				}
			}
			else if( std::string(arg).find("--output=") == 0 )
			{
				launch_opt.m_output_file = std::string(arg).substr(9);
			}
			else if( std::string(arg).find("--type=") == 0 )
			{
				std::string type = std::string(arg).substr(7);
				if( type == "-lz77" )
				{
					launch_opt.m_archive_type = EArchiveType_LZ77;
				}
				else if( type == "-lz78" )
				{
					launch_opt.m_archive_type = EArchiveType_LZ78;
				}
			}
			else if( std::string(arg).find("--rate=") == 0 )
			{
				std::string rate = std::string(arg).substr(7);
				if( rate == "-l" )
				{
					launch_opt.m_compression_rate = ECompressionRate_low;
				}
				else if( rate == "-m" )
				{
					launch_opt.m_compression_rate = ECompressionRate_middle;
				}
				else if( rate == "-h" )
				{
					launch_opt.m_compression_rate = ECompressionRate_high;
				}
			}
			else
			{
				std::cout << "Invalid input params.\n" << s_usage_line << std::endl;
				launch_opt.m_is_valid = false;
				break;
			}
		}
	}

	// Auto add/remove extension
	if(launch_opt.m_is_valid)
	{
		if( launch_opt.m_archive_mode == EArchiveMode_Compress )
		{
			launch_opt.m_output_file = launch_opt.m_output_file.empty() ? ( launch_opt.m_input_file + s_archive_ext ) : launch_opt.m_output_file + s_archive_ext;
		}
		else if( launch_opt.m_archive_mode == EArchiveMode_Extract )
		{
			launch_opt.m_output_file = launch_opt.m_output_file.empty() ? ( launch_opt.m_input_file.substr( 0, launch_opt.m_input_file.size() - 5 ) ) : launch_opt.m_output_file;
		}
	}

	return launch_opt;
}
