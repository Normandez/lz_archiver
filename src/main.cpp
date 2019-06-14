#include "CArchiver.h"

enum EArchiveMode
{
	EArchiveMode_Compress,
	EArchiveMode_Decompress,
	EArchiveMode_Size
};

int main( int argc, char** argv )
{
	CArchiver archiver;

	if( archiver.SetInputFile("C:/devel/LZArchiver/lz_archiver/main_icon.ico.arch") && archiver.SetOutputFile( "C:/devel/LZArchiver/lz_archiver/main_icon_decompressed.ico" ) )
	{
		archiver.DecompressLz77();
	}

	system("pause");
	return 0;
}
