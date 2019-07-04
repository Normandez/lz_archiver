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

enum EArchiveRate
{
	EArchiveRate_low,
	EArchiveRate_middle,
	EArchiveRate_high
};

struct SLaunchOptions
{
	bool m_is_valid = true;
	
	EArchiveMode m_archive_mode;
	EArchiveType m_archive_type = EArchiveType_LZ78;
	EArchiveRate m_archive_rate = EArchiveRate_middle;

	std::string m_input_file;
	std::string m_output_file = "";
};

SLaunchOptions ParseInputParams( int argc, char** argv )
{
	SLaunchOptions launch_opt;

	if( argc < 3 )
	{
		std::cout << "Too few input params.\n" << s_usage_line << std::endl;
		launch_opt.m_is_valid = false;
	}
	else
	{
		for( size_t it = 1; it < argc; it++ )
		{
			if( it == 1 && argv[it] == "-c" )
			{
				launch_opt.m_archive_mode = EArchiveMode_Compress;
			}
			else if( it == 1 && argv[it] == "-x" )
			{
				launch_opt.m_archive_mode = EArchiveMode_Extract;
			}
			else if( it == 2 )
			{
				launch_opt.m_input_file = argv[it];
			}
			else if( it == 3 && std::string(argv[it]).find("--output=") == 0 )
			{
				launch_opt.m_output_file = std::string(argv[it]).substr(9);
			}
			else if( it == 4 && std::string(argv[it]).find("--type=") == 0 )
			{
				std::string type = std::string(argv[it]).substr(7);
				if( type == "-lz77" )
				{
					launch_opt.m_archive_type = EArchiveType_LZ77;
				}
				else if( type == "-lz78" )
				{
					launch_opt.m_archive_type = EArchiveType_LZ78;
				}
			}
			else if( it == 5 && std::string(argv[it]).find("--rate=") == 0 )
			{
				std::string rate = std::string(argv[it]).substr(7);
				if( rate == "-l" )
				{
					launch_opt.m_archive_rate = EArchiveRate_low;
				}
				else if( rate == "-m" )
				{
					launch_opt.m_archive_rate = EArchiveRate_middle;
				}
				else if( rate == "-h" )
				{
					launch_opt.m_archive_rate = EArchiveRate_high;
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

	// Auto-add extension
	if(launch_opt.m_is_valid)
	{
		launch_opt.m_output_file = launch_opt.m_output_file.empty() ? ( launch_opt.m_input_file + s_archive_ext ) : launch_opt.m_output_file + s_archive_ext;
	}

	return launch_opt;
}
