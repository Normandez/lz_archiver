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


	}
}
