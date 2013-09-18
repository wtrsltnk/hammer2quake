#include "hammer2quake.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "tokenizer.h"
#include "wadfile.h"
#include "targa.h"

using namespace std;

Hammer2Quake::Hammer2Quake(const std::vector<std::string>& args)
    : MapParser(args), _shaderScript("materials/hammer-shaders.mtr")
{
    for (int i = 0; i < int(args.size()); i++)
    {
        std::string arg = args[i];
        if (arg == std::string("--output-map") && i + 1 < int(args.size()))
            this->_outputMap = args[i + 1];
        else if (arg == std::string("--output-root") && i + 1 < int(args.size()))
            this->_outputRoot = args[i + 1];
        else if (arg == std::string("--shader-script") && i + 1 < int(args.size()))
            this->_shaderScript = args[i + 1];
    }
}

Hammer2Quake::~Hammer2Quake()
{ }


bool Hammer2Quake::ParseMapFile()
{
    std::ofstream quakemap;
    quakemap.open (this->_outputMap.c_str());

    if (quakemap.good())
    {
        int parsedLines = 0;
        while (this->HasNextLine())
        {
            std::string line = this->ReadNextLine();
            if (line == "")
                break;

            bool isFaceLine = false;
            if (line[0] == '(')
            {
                isFaceLine = true;
                std::vector<std::string> tokens;
                common::Tokenizer tok(line.c_str(), line.size());
                while (tok.NextToken())
                    tokens.push_back(tok.GetToken());

                if (tokens.size() != 31)
                    isFaceLine = false;
                else if (tokens[0] != "(")
                    isFaceLine = false;
                else if (tokens[4] != ")")
                    isFaceLine = false;
                else if (tokens[5] != "(")
                    isFaceLine = false;
                else if (tokens[9] != ")")
                    isFaceLine = false;
                else if (tokens[10] != "(")
                    isFaceLine = false;
                else if (tokens[14] != ")")
                    isFaceLine = false;
                else if (tokens[16] != "[")
                    isFaceLine = false;
                else if (tokens[21] != "]")
                    isFaceLine = false;
                else if (tokens[22] != "[")
                    isFaceLine = false;
                else if (tokens[27] != "]")
                    isFaceLine = false;

                if (isFaceLine)
                {
                    parsedLines++;
                    this->_textures.insert(tokens[15]);
                    for (int i = 0; i < 16; i++)
                        quakemap << tokens[i] << " ";

                    // x-shift
                    quakemap << tokens[20] << " ";
                    // y-shift
                    quakemap << tokens[26] << " ";
                    // angle
                    quakemap << tokens[28] << " ";
                    // x-scale
                    quakemap << tokens[29] << " ";
                    // y-scale
                    quakemap << tokens[30] << " ";
                }
            }

            if (isFaceLine == false)
            {
                quakemap << line;
                if (line.find_first_of("wad") == 1)
                {
                    std::vector<std::string> tokens;
                    common::Tokenizer tok(line.c_str(), line.size());
                    while (tok.NextToken())
                        tokens.push_back(tok.GetToken());

                    if (tokens.size() == 2)
                        this->_wads = tokens[1];
                }
            }

            quakemap << std::endl;
        }
        quakemap.close();
        cout << parsedLines << " brush sides parsed" << endl;
    }
    return true;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

bool FileExists(const std::string& filename)
{
    ifstream Infield(filename.c_str());

    return Infield.good();
}

bool Hammer2Quake::ParseTextures()
{
    if (this->_textures.size() > 0 && this->_wads != "")
    {
        // Load wadfiles
        std::vector<std::string> tokens;
        std::vector<WadFile*> wadfiles;
        split(this->_wads, ';', tokens);
        for (std::vector<std::string>::iterator w = tokens.begin(); w != tokens.end(); ++w)
        {
            std::string wad = *w;
            std::replace(wad.begin(), wad.end(), '\\', '/');
            std::string wadfile = wad.substr(wad.find_last_of('/') + 1);
            std::string wadpath = wad.substr(0, wad.find_last_of('/'));
            std::string wadmod = wadpath.substr(wadpath.find_last_of('/') + 1);
            std::string fullwad = this->_gameRoot + "/" + wadmod + "/" + wadfile;
            wadfiles.push_back(new WadFile(fullwad.c_str()));
        }

        std::string materials = std::string(this->_outputRoot + "/" + this->_shaderScript);

        // Create materials description file
        Material::LoadCollection(materials, this->_materials);

        for (std::set<std::string>::iterator tex = this->_textures.begin(); tex != this->_textures.end(); ++tex)
        {
            std::string str = (*tex);
            if (this->_materials.find(str) == this->_materials.end())
            {
                Material* m = new Material(str);
                m->AddLine(std::string("    diffusemap textures/") + str);
                this->_materials.insert(std::make_pair(str, m));
            }

            std::string texturefile(this->_outputRoot + "/textures/" + str + ".tga");
            if (FileExists(texturefile) == false)
            {
                // Export textures from one of the WAD files to a TGA file
                for (std::vector<WadFile*>::iterator w = wadfiles.begin(); w != wadfiles.end(); ++w)
                {
                    WadFile* wad = (*w);
                    miptex_t* m = wad->FindTexture(str.c_str());
                    if (m != 0)
                    {
                        this->MiptexToTGA(m, texturefile);
                        break; // Exit the for-loop
                    }
                }
            }
        }
        Material::SaveCollection(materials, this->_materials);

        // Clean-up wadfiles
        while (wadfiles.empty() == false)
        {
            WadFile* w = wadfiles.back();
            wadfiles.pop_back();
            delete w;
        }
        return true;
    }
    else
    {
        cout << "No textures and/or wads found. Did ParseMapFile() run?" << endl;
        return false;
    }
}

void Hammer2Quake::MiptexToTGA(miptex_t* miptex, const std::string& tgafile)
{
    int size = miptex->width * miptex->height;

    unsigned char* indices = (unsigned char*)miptex + miptex->offsets[0];
    unsigned char* palette = (unsigned char*)miptex + miptex->offsets[3] + (size / 64) + 2;

    unsigned char* image = new unsigned char[size * 4];

    int pos = 0;
    for (int i = 0; i < size; i++)
    {
        unsigned char rgba[4] = { 	palette[indices[i]*3],
                            palette[indices[i]*3 + 1],
                            palette[indices[i]*3 + 2],
                                255};

        image[pos++] = rgba[0];//palette[indices[i]*3];
        image[pos++] = rgba[1];//palette[indices[i]*3 + 1];
        image[pos++] = rgba[2];//palette[indices[i]*3 + 2];
        image[pos++] = rgba[3];//255;
    }

    tga_result r = tga_write_rgb(tgafile.c_str(), image, miptex->width, miptex->height, 32);

    if (r != TGA_NOERR)
        cout << tga_error(r) << endl;

    delete []image;
}

void Hammer2Quake::PrintSummary()
{
    cout << "Parsing Hammer MAP file to Radiant/Quake MAP file" << endl;
    cout << "Input MAP file        : " << this->_input << endl;
    cout << "Input game root       : " << this->_gameRoot << endl;
    cout << "Output MAP file       : " << this->_outputMap << endl;
    cout << "Output texture folder : " << this->_outputRoot << endl;
    cout << "Output shader script  : " << this->_shaderScript << endl;
    cout << endl;
}

void Hammer2Quake::PrintResults()
{

}
