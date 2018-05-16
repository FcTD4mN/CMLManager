#pragma once

#include "FileSystem.FileOSSpecific.h"

#include "FileSystem.File.h"

#include <vector>

namespace nFileSystem { class cCMakeListsFile; }

namespace nFileSystem
{

class cDirectory :
    public cFileOSSpecific
{
    typedef cFileOSSpecific tSuperClass;
    typedef cDirectory      tSelfClass;

public:
/**@Name Construction/Destruction */
//@{
    virtual  ~cDirectory();
    cDirectory( const std::string & iPath );
//@}

public:
/**@Name FileBase Overrides */
//@{
    virtual  bool   IsDirectory()  const;
    virtual  int    IncDepth();

    virtual  int    BuildCMakeListEntryString( std::string* oString ) const;
    virtual  int    PrintInCMakeListFile( std::ofstream& iOFStream, int iIntentTabs ) const;
//@}

public:
/**@Name cFileOSSpecific Overrides */
//@{
    virtual  int  ReadOS();
//@}

public:
/**@Name Accessors/Modifiers */
//@{
    int  AddContent( cFileBase* iFile );
    unsigned int ContentCount() const;

    int  SetMakeFile( cCMakeListsFile* iCMakeFile );

    bool IsThereOSSpecificFileInContent( cFile::eOS iOS ) const;

    bool  ContainsSubDir() const;
    bool  ContainsOSSpecificSourceFiles() const;
    bool  ContainsNonOSSpecificSourceFiles() const;
//@}

public:
/**@Name Content management */
//@{
    int  ReadAllPropertiesFromCMakeListsFile();
    int  SortAlphabetically();
//@}

public:
/**@Name CMakeList */
//@{
    int  CreateCMakeListFile( bool iRecursive );
//@}

public:
/**@Name Debug */
//@{
    int  DebugPrint() const;
    int  DebugPrintContent() const;
//@}

private:
    std::vector< cFileBase* >   mContent;
    cCMakeListsFile*            mCMakeFile;

    //Flags
    bool                        mContainsSubDir;
    bool                        mContainsNonOSSpecificSourceFiles;
    bool                        mContainsOSSpecificSourceFiles; 
    bool                        mContainsUiFiles; 
    bool                        mContainsResourceFiles; 
};


}


