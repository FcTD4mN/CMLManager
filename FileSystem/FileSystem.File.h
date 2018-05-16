#pragma once

#include "FileSystem.FileOSSpecific.h"

namespace nFileSystem
{

class cFile :
    public cFileOSSpecific
{
    typedef cFileOSSpecific tSuperClass;
    typedef cFile           tSelfClass; 

public:
    enum eType
    {
        kOther = 0,
        kSource,
        kHeader,
        kUi,
        kResource
    }; 

public:
/**@Name Construction/Destruction */
//@{
    virtual  ~cFile();
    cFile( const std::string& iPath );
//@}

public:
/**@Name Accessors/Modifiers */
//@{ 
    eType   FileType() const;
//@}

public:
/**@Name Computing methods */
//@{ 
    int  ReadType();
//@}

public:
/**@Name FileBase Overrides */
//@{
    virtual  bool   IsDirectory()  const;

    virtual  int    BuildCMakeListEntryString( std::string* oString ) const;
    virtual  int    PrintInCMakeListFile( std::ofstream& iOFStream, int iIntentTabs ) const;
//@}

public:
/**@Name cFileOSSpecific Overrides */
//@{
    virtual  int  ReadOS();
//@}

public:
/**@Name Debug */
//@{
    virtual  int  DebugPrint() const;
//@}

private:  
    eType       mFileType;
};


}


