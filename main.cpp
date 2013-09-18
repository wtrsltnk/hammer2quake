#include <iostream>
#include <string>
#include "hammer2quake.h"

#define APPLICATION_NAME            "Map-Q3-HL"
#define APPLICATION_VERSION         "0.1.0"
#define APPLICATION_AUTHOR          "Wouter Saaltink"
#define APPLICATION_DATE            "18 September 2013"

using namespace std;

namespace ParseType
{
    enum eType {
        Unknown = 0,
        Hammer2Quake = 1,
        Quake2Hammer = 2
    };
}

void PrintApplicationInfo()
{
    cout << APPLICATION_VERSION << endl;
    cout << APPLICATION_NAME << endl;
    cout << APPLICATION_AUTHOR << endl;
    cout << APPLICATION_DATE << endl;
    cout << endl;
}

int main(int argc, char* argv[])
{
    PrintApplicationInfo();

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

        cout << "--- Parsing MAP file ---" << endl;
        if (parser.ParseMapFile())
        {
            if (parseTextures)
            {
                cout << "--- Parsing textures ---" << endl;
                parser.ParseTextures();
            }
        }
        parser.PrintResults();
    }
	return 0;
}
