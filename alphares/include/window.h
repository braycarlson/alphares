#pragma once

#ifdef _WIN32
    #include <io.h>
    #define access _access
#else
    #include <unistd.h>
#endif

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#include <winuser.h>

#include "../include/color.h"
#include "../include/configuration.h"
#include "../include/resources.h"
#include "../include/ui.h"
#include "../include/window.h"

#define WINDOW_STYLE ((WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)) ^ WS_THICKFRAME)
#define WM_USER_UPDATE_UI (WM_USER + 1)

class Window {
public:
    Window(HINSTANCE);
    ~Window();

    bool create(const wchar_t*, int, int);
    void center(DWORD, DWORD);
    void show(int);
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
    HWND hWnd = nullptr;
    HINSTANCE hInstance = nullptr;
    HFONT hFont = nullptr;
    HBRUSH hBrushBackground = nullptr;

    Configuration* configuration = nullptr;
    UserInterface* ui = nullptr;

    static Window* getObject(HWND);
    void registerClass(const wchar_t*);
    void applySettings();
    void revertSettings();
    void onCommand(WPARAM, LPARAM);
    void onCreate();
    void onDestroy();
};
