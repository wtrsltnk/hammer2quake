#include <iostream>
#include <string>
#include "hammer2quake.h"

using namespace std;

namespace ParseType
{
    enum eType {
        Unknown = 0,
        Hammer2Quake = 1,
        Quake2Hammer = 2
    };
}

int main(int argc, char* argv[])
{
    ParseType::eType parsertype = ParseType::Unknown;
    string input, output_map, output_textures, gameroot;

    for (int i = 0; i < argc; i++)
    {
        string arg = string(argv[i]);
        if (arg == "--hammer2quake")
            parsertype = ParseType::Hammer2Quake;
        if (arg == "--quake2hammer")
            parsertype = ParseType::Quake2Hammer;
        if (arg == "--input" && i < argc-1)
            input = string(argv[++i]);
        if (arg == "--output-map" && i < argc-1)
            output_map = string(argv[++i]);
        if (arg == "--output-textures" && i < argc-1)
            output_textures = string(argv[++i]);
        if (arg == "--gameroot" && i < argc-1)
            gameroot = string(argv[++i]);
    }

    if (parsertype == ParseType::Hammer2Quake)
    {
        Hammer2Quake parser(input);
        parser.SetGameRoot(gameroot);
        if (parser.ParseMapFile(output_map))
        {
            cout << "Map output to: " << output_map << endl;
            if (parser.ParseTextures(output_textures) == true)
                cout << "Textures output to: " << output_textures << endl;
            else
                cout << "No texture output." << endl;
        }
    }
	return 0;
}
