#include "configuration.h"

Configuration::Configuration() {
    std::string path = this->getPath();
    LPCSTR file = path.c_str();

    this->ini.SetSpaces(false);
    this->ini.SetUnicode();
    this->ini.LoadFile(file);
}

std::string Configuration::getPath() {
    const char* appdata = std::getenv("LOCALAPPDATA");

    if (appdata != nullptr) {
        return std::string(appdata) + "/FortniteGame/Saved/Config/WindowsClient/GameUserSettings.ini";
    }

    return "";
}

std::wstring Configuration::getWidth() {
    std::string x = ini.GetValue(section, "ResolutionSizeX", "1920");
    return std::wstring(x.begin(), x.end());

}

std::wstring Configuration::getHeight() {
    std::string y = ini.GetValue(section, "ResolutionSizeY", "1080");
    return std::wstring(y.begin(), y.end());
}

std::wstring Configuration::getFrameRate() {
    std::string f = this->ini.GetValue(this->section, "FrameRateLimit", "0");
    std::string rate = f.substr(0, f.find(".", 0));
    return std::wstring(rate.begin(), rate.end());
}

std::string Configuration::getWindowMode() {
    return ini.GetValue(section, "FullscreenMode", "0");
}

void Configuration::setConfiguration(int width, int height, int fps, int mode) {
    this->setWidth(width);
    this->setHeight(height);
    this->setFrameRate(fps);
    this->setWindowMode(mode);

    std::string path = this->getPath();
    LPCSTR file = path.c_str();

    this->ini.SaveFile(file);
}

void Configuration::setWidth(int w) {
    const char* width = std::to_string(w).c_str();

    this->ini.SetValue(this->section, "ResolutionSizeX", width);
    this->ini.SetValue(this->section, "LastUserConfirmedResolutionSizeX", width);
    this->ini.SetValue(this->section, "DesiredScreenWidth", width);
    this->ini.SetValue(this->section, "LastUserConfirmedDesiredScreenWidth", width);
}

void Configuration::setHeight(int h) {
    const char* height = std::to_string(h).c_str();

    this->ini.SetValue(this->section, "ResolutionSizeY", height);
    this->ini.SetValue(this->section, "LastUserConfirmedResolutionSizeY", height);
    this->ini.SetValue(this->section, "DesiredScreenHeight", height);
    this->ini.SetValue(this->section, "LastUserConfirmedDesiredScreenHeight", height);
}

void Configuration::setFrameRate(int f) {
    std::string str = std::to_string(f) + ".000000";
    const char* fps = str.c_str();

    this->ini.SetValue(this->section, "FrameRateLimit", fps);
    this->ini.SetValue(this->section, "FrontendFrameRateLimit", fps);
}

void Configuration::setWindowMode(int m) {
    const char* mode = std::to_string(m).c_str();

    this->ini.SetValue(this->section, "LastConfirmedFullscreenMode", mode);
    this->ini.SetValue(this->section, "PreferredFullscreenMode", mode);
    this->ini.SetValue(this->section, "FullscreenMode", mode);
}
