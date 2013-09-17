#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <map>
#include <vector>

class Material
{
public:
    typedef std::map<std::string, Material*> Collection;
public:
    Material(const std::string& name);
    virtual ~Material();

    void AddLine(const std::string& line);
    void WriteToStream(std::ofstream& stream);
private:
    const std::string _name;
    std::vector<std::string> _lines;

public:
    static int LoadCollection(const std::string& filename, Collection& collection);
    static void SaveCollection(const std::string& filename, Collection& collection);

};

#endif // MATERIAL_H
