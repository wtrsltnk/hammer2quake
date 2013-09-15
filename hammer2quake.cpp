#include "hammer2quake.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "tokenizer.h"

using namespace std;

Hammer2Quake::Hammer2Quake(const std::string &input)
    : MapParser(input)
{ }

Hammer2Quake::~Hammer2Quake()
{ }


bool Hammer2Quake::ParseMapFile(const std::string &output)
{
    std::ofstream quakemap;
    quakemap.open (output.c_str());

    if (quakemap.good())
    {
        while (this->HasNextLine())
        {
            std::string line = this->ReadNextLine();

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
                    {
                        cout << "Found wads:" << endl << tokens[1] << endl;
                        this->_wads = tokens[1];
                    }
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

bool Hammer2Quake::ParseTextures(const std::string &output)
{
    std::vector<std::string> tokens;
    split(this->_wads, ';', tokens);
    cout << tokens.size() << endl;
    return true;
}
