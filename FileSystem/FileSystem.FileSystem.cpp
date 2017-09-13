
#include "FileSystem.FileSystem.h"

#include "FileSystem.CMakeListsFile.h"
#include "FileSystem.Directory.h"
#include "FileSystem.File.h"
#include "FileSystem.FileBase.h"

#include <locale>
#include <fstream>
#include <string.h>
#include <iostream>

#include <experimental/filesystem>

namespace  fs = std::experimental::filesystem;

namespace nFileSystem
{

cFileSystem::~cFileSystem()
{
}


cFileSystem::cFileSystem()
{
    ReadOptions();
}


const std::string
cFileSystem::FavoritePath( unsigned int iPath ) const
{
    if( iPath >= mFavoritePaths.size() )
        return  "";

    return  mFavoritePaths[ iPath ];
}


unsigned long
cFileSystem::FavoriteCount() const
{
    return  mFavoritePaths.size();
}


// static
cDirectory*
cFileSystem::ReadDirectory( const std::string& iPath )
{
    ::nFileSystem::cDirectory* mainDirectory = new ::nFileSystem::cDirectory( iPath );

    fs::path  directory( mainDirectory->Path().c_str() );
    if( !fs::exists( directory ) )
    {
        std::cout << "Error while opening directory\n";
        return  0;
    }

    for( auto  it = fs::directory_iterator( directory ); it != fs::directory_iterator(); ++it )
    {
        if( fs::is_directory( it->status() ) )
        {
            // Dismisses folders starting with . : .vscode, .svn
            if( it->path().filename().c_str()[0] == L'.' )
                continue;

            ::nFileSystem::cDirectory* directory = ReadDirectory( it->path().string() );
            directory->SortAlphabetically();
            mainDirectory->AddContent( directory );
        }
        else
        {
            if( !it->path().filename().compare( L"CMakeLists.txt" ) )
            {
                ::nFileSystem::cCMakeListsFile* makeFile = new ::nFileSystem::cCMakeListsFile( it->path().string() );
                mainDirectory->SetMakeFile( makeFile );
            }
            else
            {
                // We only wanna add C++ related files, so we filter for cpp, c, mm, h, hpp files
                if( it->path().has_extension() )
                {
                    std::string  extension = it->path().extension().string();
                    if( !extension.compare( ".cpp" ) ||
                        !extension.compare( ".c" )   || 
                        !extension.compare( ".mm" )  || 
                        !extension.compare( ".h" )   || 
                        !extension.compare( ".hpp" ) )
                    {
                        ::nFileSystem::cFile* file = new ::nFileSystem::cFile( it->path().string() );
                        mainDirectory->AddContent( file );
                    }
                }
            }
        }
    }

    mainDirectory->ReadAllPropertiesFromCMakeListsFile();

    return  mainDirectory;
}


int
cFileSystem::GenerateCMakeLists( const std::string& iPath, bool iUserConfirm, bool iRecursive )
{
    ::nFileSystem::cDirectory* mainDir = ReadDirectory( iPath );
    if( !mainDir )
    {
        std::cout << mainDir->Path() << " does not name a file or directory" << std::endl;
        return  1;
    }

    mainDir->SortAlphabetically();
    mainDir->DebugPrint();

    if( iUserConfirm )
    {
        char input;

        std::cout << "Generate CMakeLists ? (y/n)\n";
        std::cin >> input;

        if( input != 'y' && input != 'Y' )
            return  0;
    }

    std::cout << "Generating CMakeLists ...\n";
    mainDir->CreateCMakeListFile( iRecursive );
    std::cout << "Done\n";

    delete  mainDir;

    return  0;
}


int
cFileSystem::ReadOptions()
{
    cFile* optionFile = new cFile( "Options/option.txt" );
    if( !optionFile )
        return  1;

    std::ifstream optionStream( optionFile->Path() );
    if( optionStream.is_open() )
    {
        std::string path;
        while( getline( optionStream, path ) )
        {
            if( path[ 0 ] != '#' )
                mFavoritePaths.push_back( path );
        }
    }
    else
    {
        std::cout << "Stream for option not opened\n";
        return  1;
    }

    std::cout << "Option successfully read : \n";
    for( auto i : mFavoritePaths )
        std::cout << i << std::endl;

    return 0;
}

}

