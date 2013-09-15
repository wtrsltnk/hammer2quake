#ifndef HAMMER2QUAKE_H
#define HAMMER2QUAKE_H

#include "mapparser.h"

class Hammer2Quake : public MapParser
{
public:
    Hammer2Quake(const std::string& input);
    virtual ~Hammer2Quake();

    virtual bool ParseMapFile(const std::string &output);
    virtual bool ParseTextures(const std::string &output);

private:
    std::string _wads;
};

#endif // HAMMER2QUAKE_H
