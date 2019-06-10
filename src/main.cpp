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

	if( archiver.SetInputFile("C:/devel/LZArchiver/lz_archiver/input.txt") )
	{
		archiver.CompressLz77();
	}

	system("pause");
	return 0;
}
