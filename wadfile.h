#ifndef WADFILE_H_
#define WADFILE_H_

#include "wad.h"
#include <stdio.h>

class WadFile
{
protected:
    FILE* _file;
    int _size;
    tWadHeader _header;
    tWadLump* _lumps;
	
public:
    WadFile(const char* filename);
    virtual ~WadFile();
	
	int GetMiptexCount();
    int FindTextureIndex(const char* name);
    miptex_t* FindTexture(const char* name);
    char* GetMiptex(int index);
};

#endif /*WADFILE_H_*/
