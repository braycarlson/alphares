#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <configuration.h>
#include <ui.h>
#include <windows.h>

#define WINDOW_STYLE ((WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)) ^ WS_THICKFRAME)

class Window {
public:
    Window(HINSTANCE hInstance);
    ~Window();

    bool create(const wchar_t* title, int width, int height);
    void center(DWORD style, DWORD exStyle);
    void show(int cmdShow);
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND hWnd = nullptr;
    HINSTANCE hInstance = nullptr;
    HFONT hFont = nullptr;
    HBRUSH hBrushBackground = nullptr;
    HBRUSH hBrushStatic = nullptr;
    HBRUSH hBrushEdit = nullptr;
    HBRUSH hBrushButton = nullptr;

    Configuration* configuration = nullptr;
    UserInterface* ui = nullptr;

    static Window* getObject(HWND hwnd);
    void registerClass(const wchar_t* name);
    LRESULT onColorButton(WPARAM wParam);
    LRESULT onColorEdit(WPARAM wParam);
    LRESULT onColorStatic(WPARAM wParam);
    LRESULT onDraw(WPARAM wParam, LPARAM lParam);
    void onCommand(WPARAM wParam, LPARAM lParam);
    void onCreate();
    void onDestroy();
};
