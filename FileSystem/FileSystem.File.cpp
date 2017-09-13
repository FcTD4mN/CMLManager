#include "FileSystem.File.h"

#include <locale>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>


namespace nFileSystem
{


cFile::~cFile()
{
}

nFileSystem::cFile::cFile( const std::string& iPath ) :
    tSuperClass( iPath ),
    mFileType( kOther )
{
    ReadType();
    ReadOS();
}


cFile::eType
cFile::FileType() const
{
    return  mFileType;
}


int
cFile::ReadType()
{
    std::string name = Name();
    std::size_t dotPos = name.find_last_of( '.' );

    // No . in the file name = no extension ?!
    if( dotPos == std::string::npos )
        return  -1;

    std::string type = name.substr( dotPos + 1 );

    // This would be, just a . like File.
    if( type.size() <= 0 )
        return  -1;

    // going to lower, so we can easily compare
    for( unsigned int i = 0; i < type.size(); ++i )
        type[ i ] = std::tolower( type[ i ], std::locale() );

    if( !strcmp( type.c_str(), "cpp" )
        || !strcmp( type.c_str(), "c" )
        || !strcmp( type.c_str(), "mm" ) )
    {
        mFileType = kSource;
    }
    else if( !strcmp( type.c_str(), "h" )
        || !strcmp( type.c_str(), "hpp" ) )
    {
        mFileType = kHeader;
    }

    return 0;
}


bool
cFile::IsDirectory() const
{
    return  false;
}


int 
cFile::BuildCMakeListEntryString( std::string* oString ) const
{
    *oString = "${RELATIVE_DIR}/" + Name();
    return 0;
}


int
cFile::PrintInCMakeListFile( std::ofstream& iOFStream, int iIntentTabs ) const
{
    std::string tabs;
    for( int i = 0; i < iIntentTabs; ++i )
        tabs.append( "    " );

    std::string stuffToWrite;

    if( !IsCompiled() )
        stuffToWrite += "# ";

    std::string fileEntry;
    BuildCMakeListEntryString( &fileEntry );

    stuffToWrite += tabs + fileEntry + "\n";

    if( IsTargeted() )
        WriteTargetPart( &stuffToWrite, iIntentTabs, stuffToWrite );

    iOFStream << stuffToWrite;

    return 0;
}


int
cFile::ReadOS()
{
    //TODO: Maybe use regex in all File reading (OS and type )

    std::string name = Name();
    // going to lower, so we can easily compare
    for( unsigned int i = 0; i < name.size(); ++i )
        name[ i ] = std::tolower( name[ i ], std::locale() );

    //========= EXTENSION SEARCH =============
    // First, looking extension if mm
    std::size_t dotPos = name.find_last_of( '.' );
    std::string extension = name.substr( dotPos + 1 );

    if( !strcmp( extension.c_str(), "mm" ) )
    {
        FileOS( kMacosx );
        return  0;
    }

    //=========== Name-OS SEARCH =============
    // If extension isn't specific, we look for OS name in the file name after a dash '-'
    std::size_t dashPos = name.find_last_of( '-' );

    // If - in the name, we extract the name
    if( dashPos != std::string::npos )
    {
        std::string os = name.substr( dashPos + 1 );

        // This would be, just a - like File-
        if( os.size() <= 0 )
            return  0;

        std::size_t found = os.find( "linux" );
        if( found != std::string::npos )
        {
            FileOS( kLinux );
            return  0;
        }

        found = os.find( "mac" );
        if( found != std::string::npos )
        {
            FileOS( kMacosx );
            return  0;
        }

        found = os.find( "cocoa" );
        if( found != std::string::npos )
        {
            FileOS( kMacosx );
            return  0;
        }

        found = os.find( "win" );
        if( found != std::string::npos )
        {
            FileOS( kWindows );
            return  0;
        }
    }

    //=========== Full name SEARCH =============
    std::string nameWithoutExtension = name.substr( 0, dotPos );

    // If then name is A.B.C.Cocoa(.h), we need one more extraction
    dotPos = nameWithoutExtension.find_last_of( '.' );

    // One more . = file was something.cocoa(.h)
    if( dotPos != std::string::npos )
        nameWithoutExtension = nameWithoutExtension.substr( dotPos + 1 );

    if( !strcmp( nameWithoutExtension.c_str(), "linux" ) )
    {
        FileOS( kLinux );
        return  0;
    }

    if( !strcmp( nameWithoutExtension.c_str(), "macosx" ) || !strcmp( nameWithoutExtension.c_str(), "cocoa" ) )
    {
        FileOS( kMacosx );
        return  0;
    }

    if( !strcmp( nameWithoutExtension.c_str(), "windows" ) || !strcmp( nameWithoutExtension.c_str(), "win32" ) || !strcmp( nameWithoutExtension.c_str(), "win64" ) )
    {
        FileOS( kWindows );
        return  0;
    }

    FileOS( kNone );

    return 0;
}


int
cFile::DebugPrint() const
{
    std::cout << "File : " << Name();
    std::cout << std::setfill( ' ' ) << std::setw( 100 - int( Name().size() ) - Depth() * 6 );

    if( IsCompiled() )
        std::cout << "C  ";
    else
        std::cout << "NC ";

    if( IsNewFile() )
        std::cout << "  N";
    else
        std::cout << "   ";

    if( mFileType == kSource )
        std::cout << "   Source file ";
    else if( mFileType == kHeader )
        std::cout << "   Header file ";
    else
        std::cout << "   Other file  ";

    switch( FileOS() )
    {
        case kLinux :
        {
            std::cout << "   -- Linux File";
            break;
        }
        case kMacosx :
        {
            std::cout << "   -- MacOSX File";
            break;
        }
        case kWindows:
        {
            std::cout << "   -- Windows File";
            break;
        }
        default :
            break;
    }

    if( IsTargeted() )
        std::cout << "   Targeted : " << TargetName();

    std::cout << std::endl;

    return 0;
}

}

