#include "../include/configuration.h"

Configuration::Configuration() {
    std::wstring path = this->getPath();
    LPCWSTR file = path.c_str();

    std::filesystem::path directory = std::filesystem::path(path).parent_path();

    bool is_directory = std::filesystem::exists(directory);

    if (!is_directory) {
        std::filesystem::create_directories(directory);
    }

    bool is_file = std::filesystem::exists(file);

    if (!is_file) {
        std::wofstream ofs(file);
        ofs.close();
    }

    this->ini.SetSpaces(false);
    this->ini.SetUnicode();
    this->ini.LoadFile(file);
}

bool Configuration::createBackup() {
    std::wstring original = this->getPath();
    std::wstring backup = original + L"_backup";

    bool is_backup = std::filesystem::exists(backup);

    if (is_backup) {
        return true;
    }

    bool is_original = std::filesystem::exists(original);

    if (!is_original) {
        return false;
    }

    try {
        std::filesystem::copy_file(original, backup, std::filesystem::copy_options::overwrite_existing);
        this->setReadOnlyAttribute(backup);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}

bool Configuration::isBackup() {
    std::wstring original = this->getPath();
    std::wstring backup = original + L"_backup";

    return std::filesystem::exists(backup);
}

bool Configuration::revertToBackup() {
    if (!this->isBackup()) {
        return false;
    }

    std::wstring original = this->getPath();
    std::wstring backup = original + L"_backup";

    bool is_writeable = this->setWritableAttribute(original);

    if (!is_writeable) {
        return false;
    }

    try {
        std::filesystem::remove(original);
        std::filesystem::rename(backup, original);

        this->setWritableAttribute(original);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}

bool Configuration::setReadOnlyAttribute(const std::wstring& path) {
    LPCWSTR file = path.c_str();

    DWORD attributes = GetFileAttributesW(file);

    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (!(attributes & FILE_ATTRIBUTE_READONLY)) {
        if (!SetFileAttributesW(file, attributes | FILE_ATTRIBUTE_READONLY)) {
            return false;
        }
    }

    return true;
}

bool Configuration::setWritableAttribute(const std::wstring& path) {
    LPCWSTR file = path.c_str();

    DWORD attributes = GetFileAttributesW(file);

    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (attributes & FILE_ATTRIBUTE_READONLY) {
        if (!SetFileAttributesW(file, attributes & ~FILE_ATTRIBUTE_READONLY)) {
            return false;
        }
    }

    return true;
}

std::wstring Configuration::getPath() {
    wchar_t* appdata = _wgetenv(L"LOCALAPPDATA");

    if (appdata == nullptr) {
        return L"";
    }

    return std::wstring(appdata) + L"\\FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini";
}

std::wstring Configuration::getWidth() {
    const wchar_t* x = ini.GetValue(section, L"ResolutionSizeX", L"1920");
    return std::wstring(x);
}

std::wstring Configuration::getHeight() {
    const wchar_t* y = ini.GetValue(section, L"ResolutionSizeY", L"1080");
    return std::wstring(y);
}

std::wstring Configuration::getFrameRate() {
    const wchar_t* f = ini.GetValue(section, L"FrameRateLimit", L"0");
    std::wstring frameRate(f);
    size_t pos = frameRate.find(L".");
    return (pos != std::wstring::npos) ? frameRate.substr(0, pos) : frameRate;
}

std::wstring Configuration::getWindowMode() {
    const wchar_t* mode = ini.GetValue(section, L"FullscreenMode", L"0");
    return std::wstring(mode);
}

void Configuration::reloadConfiguration() {
    std::wstring path = this->getPath();
    LPCWSTR file = path.c_str();

    if (this->ini.LoadFile(file) < 0) {
        MessageBoxW(nullptr, L"Failed to reload configuration file.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void Configuration::setConfiguration(int width, int height, int fps, int mode) {
    this->createBackup();

    this->setWidth(width);
    this->setHeight(height);
    this->setFrameRate(fps);
    this->setWindowMode(mode);

    std::wstring path = this->getPath();
    LPCWSTR file = path.c_str();

    this->ini.SaveFile(file);
}

void Configuration::setWidth(int w) {
    std::wstring width = std::to_wstring(w);

    this->ini.SetValue(section, L"ResolutionSizeX", width.c_str());
    this->ini.SetValue(section, L"LastUserConfirmedResolutionSizeX", width.c_str());
    this->ini.SetValue(section, L"DesiredScreenWidth", width.c_str());
    this->ini.SetValue(section, L"LastUserConfirmedDesiredScreenWidth", width.c_str());
}

void Configuration::setHeight(int h) {
    std::wstring height = std::to_wstring(h);

    this->ini.SetValue(section, L"ResolutionSizeY", height.c_str());
    this->ini.SetValue(section, L"LastUserConfirmedResolutionSizeY", height.c_str());
    this->ini.SetValue(section, L"DesiredScreenHeight", height.c_str());
    this->ini.SetValue(section, L"LastUserConfirmedDesiredScreenHeight", height.c_str());
}

void Configuration::setFrameRate(int f) {
    std::wstring fps = std::to_wstring(f) + L".000000";

    this->ini.SetValue(section, L"FrameRateLimit", fps.c_str());
    this->ini.SetValue(section, L"FrontendFrameRateLimit", fps.c_str());
}

void Configuration::setWindowMode(int m) {
    std::wstring mode = std::to_wstring(m);

    this->ini.SetValue(section, L"LastConfirmedFullscreenMode", mode.c_str());
    this->ini.SetValue(section, L"PreferredFullscreenMode", mode.c_str());
    this->ini.SetValue(section, L"FullscreenMode", mode.c_str());
}
