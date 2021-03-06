#pragma once

#include <string>
#include <vector>

namespace nFileSystem{ class cDirectory; }

namespace nFileSystem
{

class cFileSystem
{
public:
/**@Name Construction/Destruction */
//@{
    ~cFileSystem();
    cFileSystem();
//@} 

public:
/**@Name Accessors/Modifiers */
//@{
    /** Reads a directory and creates the whole file tree associated */
    const std::string FavoritePath( unsigned int iPath ) const;

    /** How many directories are in favorites ? */
    unsigned long FavoriteCount() const;
//@}

public:
/**@Name Methods */
//@{
    /** Reads a directory and creates the whole file tree associated */
    static  cDirectory* ReadDirectory( const std::string& iPath );

    int     GenerateCMakeLists( const std::string& iPath, bool iUserConfirm, bool iRecursive );

private:
    /** Reads user options */
    int  ReadOptions();
//@}

private:
    std::vector< std::string > mFavoritePaths;

};

}

