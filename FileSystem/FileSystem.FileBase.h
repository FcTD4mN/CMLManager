#pragma once

#include <string>

#include <experimental/filesystem>

namespace  fs = std::experimental::filesystem;

namespace nFileSystem {

class cFileBase
{
public:
/**@Name Construction/Destruction */
//@{
    virtual ~cFileBase() = 0;
    cFileBase( const fs::path&  iPath );
//@}

public:
/**@Name Accessors/Modifiers */
//@{
    const fs::path&     Path() const;

    std::string         Name() const;

    const std::string&  TargetName() const;
    int                 TargetName( const std::string& iTargetName );

    const std::string&  TargetOperator() const;
    int                 TargetOperator( const std::string& iTargetOperator );

    bool                IsTargeted() const;
    int                 IsTargeted( bool iIsTargeted );

    bool                IsCompiled() const;
    int                 IsCompiled( bool iIsCompiled );

    bool                IsNewFile() const;
    int                 IsNewFile( bool iIsNewFile );

    virtual  bool       IsDirectory() const = 0;

    int                 Depth() const;
    virtual  int        IncDepth();

protected:
    int                 Depth( int iDepth );
//@} 

public:
/**@Name CMakeLists */
//@{
    virtual  int    BuildCMakeListEntryString( std::string* oString ) const = 0;
    virtual  int    PrintInCMakeListFile( std::ofstream& iOFStream, int iIntentTabs ) const = 0;

    int             WriteSetHeaderPart( std::ofstream& iOFStream, int iIntentTabs ) const;
    int             WriteSetSourcePart( std::ofstream& iOFStream, int iIntentTabs ) const;
    int             WriteTargetPart( std::string* oString, int iIntentTabs, const std::string& iTextInsideTargetPart ) const;
//@}

public:
/**@Name Operators */
//@{
    bool operator<( const cFileBase& iRHS ) const;
    bool operator<( const cFileBase* iRHS ) const;
    bool operator()( const cFileBase* iRHS, const cFileBase* iLHS ) const;
//@} 

public:
/**@Name Debug */
//@{
    virtual  int  DebugPrint() const = 0;
//@}

private:
    fs::path  mPath;

    bool        mIsTargeted;
    std::string mTargetName;
    std::string mTargetOperator; // Between STREQUAL or NOT STREQUAL, so you can specify = or != targetName

    bool        mIsCompiled;    // The file is compiled or not ( is it in commentary (#) in the CMakeLists file )
    bool        mIsNewFile;     // The file is new = it's not in the CMakeLists file yet

    int         mDepth;         //Depth within the file tree

};

}
