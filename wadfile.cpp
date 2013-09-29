#include "wadfile.h"
#include <string>
#include <iostream>
#include <strings.h>

using namespace std;

WadFile::WadFile(const char* filename)
    : _filename(filename)
{
    this->_file = fopen(filename, "rb");

    if (this->_file != 0)
    {
        fseek(this->_file, 0, SEEK_END);
        this->_size = ftell(this->_file);
        fseek(this->_file, 0, SEEK_SET);
        fread((void*)&this->_header, 1, sizeof(tWadHeader), this->_file);

        this->_lumps = new tWadLump[this->_header.numlumps];
        fseek(this->_file, this->_header.infotableofs, SEEK_SET);
        fread(this->_lumps, this->_header.numlumps, sizeof(tWadLump), this->_file);
    }
    else
    {
        cout << "File not found: " << filename << endl;
    }
}

WadFile::~WadFile()
{
    if (this->_file != 0)
    {
        fclose(this->_file);
    }
}

int WadFile::FindTextureIndex(const char* name)
{
    for (int i = 0; i < this->_header.numlumps; i++)
#ifdef WIN32
        if (stricmp(this->_lumps[i].name, name) == 0)
#else
        if (strcasecmp(this->_lumps[i].name, name) == 0)
#endif
            return i;
    return -1;
}

miptex_t* WadFile::FindTexture(const char* name)
{
    return (miptex_t*)this->GetMiptex(this->FindTextureIndex(name));
}

char* WadFile::GetMiptex(int index)
{
    char* result = 0;

    if (index > 0 && index < this->_header.numlumps)
	{
        tWadLump lump = this->_lumps[index];
        result = new char[lump.size];
        fseek(this->_file, lump.filepos, SEEK_SET);
        fread(result, 1, lump.size, this->_file);
	}
	
	return result;
}

int WadFile::GetMiptexCount()
{
    return this->_header.numlumps;
}
