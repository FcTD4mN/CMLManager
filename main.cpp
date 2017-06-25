#include "FileSystem/FileSystem.Directory.h"
#include "FileSystem/FileSystem.FileBase.h"
#include "FileSystem/FileSystem.FileSystem.h"

#include <cstdio>


int 
main( int argc, char** argv )
{
	::nFileSystem::cDirectory* mainDir = ::nFileSystem::cFileSystem::ReadDirectory( "/home/damien/projects/CMLManager/MakeList" );
	
	if( !mainDir )
		return  0;
		
	mainDir->DebugPrint(); 

	mainDir->CreateCMakeListFile();

	delete  mainDir;

    return 0;
}

