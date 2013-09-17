#include "hammer2quake.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "tokenizer.h"
#include "wadfile.h"

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

        // Create materials description file
        Material::LoadCollection(this->_outputRoot + "/" + this->_shaderScript, this->_materials);
        for (std::set<std::string>::iterator tex = this->_textures.begin(); tex != this->_textures.end(); ++tex)
        {
            std::string str = (*tex);
            if (this->_materials.find(str) == this->_materials.end())
            {
                Material* m = new Material(str);
                m->AddLine(std::string("    diffusemap textures/") + str);
                this->_materials.insert(std::make_pair(str, m));
            }
        }
        Material::SaveCollection(this->_outputRoot + "/" + this->_shaderScript, this->_materials);

        // Export textures from one of the WAD files to a TGA file
        /// TODO

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
