#include "FileSystem/FileSystem.Directory.h"
#include "FileSystem/FileSystem.FileBase.h"
#include "FileSystem/FileSystem.FileSystem.h"

#include <cstdio>
#include <iostream>
#include <string.h>


void
usage(int argc, char** argv)
{
    std::cout << "Usage of " << argv[0] << ":" << std::endl;
    std::cout << argv[0] << " [--no-confirm | -y] " << "[--recursive | -r] " << "path1 [path2 [...]]" << std::endl;
}


int
main( int argc, char** argv )
{
    ::nFileSystem::cFileSystem fileSystem;

    if( argc < 2 )
    {
        for( unsigned int i = 0; i < fileSystem.FavoriteCount(); ++i )
        {
            fileSystem.GenerateCMakeLists( fileSystem.FavoritePath( i ), true, true );
        }        
        return 0;
    }
    
    bool noConfirm = false;
    bool recursive = false;
    std::vector< char* >  pathArray;
    for( int i = 1; i < argc; ++i )
    {
        auto arg = argv[i];
        
        if( !strcmp( arg, "--no-confirm" ) || !strcmp( arg, "-y" ) )
        {       
            noConfirm = true;
        }
        else if( !strcmp( arg, "--recursive" ) || !strcmp( arg, "-r" ) )
        {
            recursive = true;
        }
        else if( arg[0] != '-' )
        {
            pathArray.push_back(arg);
        }
        else
        {
            std::cerr << "Invalid argument: " << arg << std::endl;
            usage(argc, argv);
            return  3;
        }
    }

    for(auto p: pathArray)
        fileSystem.GenerateCMakeLists( p, !noConfirm, recursive );

    return 0;
}

