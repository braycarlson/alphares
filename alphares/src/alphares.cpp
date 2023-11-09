#include "window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)pCmdLine;

    Window window(hInstance);

    if (!window.create(L"alphares", 240, 250)) {
        return 0;
    }

    window.center(WINDOW_STYLE, 0);
    window.show(nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
