#include <windows.h>

#include "../include/window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)pCmdLine;

    Window window(hInstance);

    if (!window.create(L"alphares", 240, 250)) {
        return 0;
    }

    window.center(WINDOW_STYLE, 0);
    window.show(nCmdShow);

    MSG msg = {};

    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
