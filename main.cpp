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
    ParseType::eType parserType = ParseType::Unknown;
    bool parseTextures = true;

    vector<string> args;
    for (int i = 0; i < argc; i++)
    {
        string arg = string(argv[i]);
        if (arg == std::string("--hammer2quake"))
            parserType = ParseType::Hammer2Quake;
        else if (arg == std::string("--quake2hammer"))
            parserType = ParseType::Quake2Hammer;
        else if (arg == std::string("--parse-textures") && i + 1 < argc)
            parseTextures = (std::string(args[i + 1]) == std::string("yes"));
        else
            args.push_back(arg);
    }

    if (parserType == ParseType::Hammer2Quake)
    {
        Hammer2Quake parser(args);
        parser.PrintSummary();

        cout << "Parsing MAP file...";
        if (parser.ParseMapFile())
        {
            cout << "done!" << endl;

            if (parseTextures)
            {
                cout << "Parsing textures for MAP file...";
                if (parser.ParseTextures())
                    cout << "done!" << endl;
            }
        }
        parser.PrintResults();
    }
	return 0;
}
