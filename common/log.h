#ifndef LOG_H
#define LOG_H

namespace common
{

class Log
{
public:
    static void Warning(const char* str, ...);
    static void Error(const char* str, ...);

};

}

#endif // LOG_H
