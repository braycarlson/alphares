#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "../lib/simpleini/simpleini.h"

#pragma once

typedef enum {
    MODE_FULLSCREEN = 0,
    MODE_WINDOWED_FULLSCREEN = 1,
    MODE_WINDOWED = 2
} WindowMode;

class Configuration {
public:
    Configuration();

    bool createBackup();
    bool isBackup();
    void reloadConfiguration();
    bool revertToBackup();

    std::wstring getPath();
    std::wstring getWindowMode();
    std::wstring getWidth();
    std::wstring getHeight();
    std::wstring getFrameRate();
    void setConfiguration(int, int, int, int);
    bool setReadOnlyAttribute(const std::wstring&);
    bool setWritableAttribute(const std::wstring&);

private:
    void setWidth(int);
    void setHeight(int);
    void setFrameRate(int);
    void setWindowMode(int);

    CSimpleIniW ini;
    const wchar_t* section = L"/Script/FortniteGame.FortGameUserSettings";
    std::wstring path;
};
