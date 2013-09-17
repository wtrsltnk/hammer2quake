#include "wadfile.h"
#include <string>

int WadFile::FindTextureIndex(const char* name)
{
    for (int i = 0; i < this->_size; i++)
        if (std::string(this->_lumps[i].name) == std::string(name))
            return i;
    return -1;
}

miptex_t* WadFile::FindTexture(const char* name)
{
    return (miptex_t*)this->GetMiptex(this->FindTextureIndex(name));
}

char* WadFile::GetMiptex(int index)
{
	static char result[512 * 512 * 3] = { 0 };
	
    if (index > 0 && index < this->_header.numlumps)
	{
        tWadLump lump = this->_lumps[index];
        fseek(this->_file, lump.filepos, SEEK_SET);
        fread(result, 1, lump.size, this->_file);
	}
	
	return result;
}

int WadFile::GetMiptexCount()
{
    return this->_header.numlumps;
}

WadFile::WadFile(const char* filename)
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
        printf("%s: %i lumps found\n", this->_header.identification, this->_header.numlumps);
	}
	else
	{
		printf("File not found: %s\n", filename);
	}
}

WadFile::~WadFile()
{
    if (this->_file != 0)
	{
        fclose(this->_file);
	}
}
