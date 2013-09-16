#ifndef HAMMER2QUAKE_H
#define HAMMER2QUAKE_H

#include "mapparser.h"
#include "material.h"
#include <set>

class Hammer2Quake : public MapParser
{
public:
    Hammer2Quake(const std::string& input);
    virtual ~Hammer2Quake();

    virtual bool ParseMapFile(const std::string &output);
    virtual bool ParseTextures(const std::string &output);

private:
    std::string _wads;
    std::set<std::string> _textures;
    Material::Collection _materials;
};

#endif // HAMMER2QUAKE_H
