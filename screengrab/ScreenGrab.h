#ifndef SCREENGRAB_H
#define SCREENGRAB_H

#include <string>

class ScreenGrab
{
public:
    static bool waitForFinish(const char *filepath, const char* eventName);
};

#endif // SCREENGRAB_H
