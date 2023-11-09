#include <cstdlib>
#include <string>

#include "simpleini.h"

#pragma once

class Configuration {
public:
    Configuration();

    std::string getPath();
    std::string getWindowMode();
    std::wstring getWidth();
    std::wstring getHeight();
    std::wstring getFrameRate();
    void setConfiguration(int width, int height, int fps, int mode);

private:
    void setWidth(int width);
    void setHeight(int height);
    void setFrameRate(int fps);
    void setWindowMode(int mode);

    CSimpleIniA ini;
    const char* section = "/Script/FortniteGame.FortGameUserSettings";
    std::string path;
};
