#include <iostream>
#include <string>

#include "color.h"
#include "resources.h"
#include "window.h"

Window::Window(HINSTANCE hInstance) : hWnd(nullptr), hInstance(hInstance) {
    this->configuration = new Configuration();
}

Window::~Window() {
    delete this->configuration;
}

Window* Window::getObject(HWND hWnd) {
    return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

void Window::center(DWORD style, DWORD exStyle) {
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    RECT client_rect;

    GetClientRect(this->hWnd, &client_rect);
    AdjustWindowRectEx(&client_rect, style, FALSE, exStyle);

    int client_width = client_rect.right - client_rect.left;
    int client_height = client_rect.bottom - client_rect.top;

    SetWindowPos(this->hWnd, 0,
        screen_width / 2 - client_width / 2,
        screen_height / 2 - client_height / 2,
        client_width, client_height, 0
    );
}

bool Window::create(const wchar_t* title, int width, int height) {
    this->registerClass(L"alphares");

    this->hWnd = CreateWindowEx(
        0,
        L"alphares",
        title,
        WINDOW_STYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        this->hInstance,
        this
    );

    return hWnd != nullptr;
}

void Window::registerClass(const wchar_t* name) {
    this->hBrushBackground = CreateSolidBrush(Color::PURPLE);

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Window::WindowProc;
    wcex.hInstance = this->hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = this->hBrushBackground;
    wcex.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDR_ICON));
    wcex.lpszClassName = name;

    if (!RegisterClassEx(&wcex)) {
        return;
    }
}

void Window::show(int cmdShow) {
    ShowWindow(this->hWnd, cmdShow);
    UpdateWindow(this->hWnd);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->hWnd = hWnd;
    } else {
        window = getObject(hWnd);
    }

    if (window) {
        switch (uMsg) {
        case WM_COMMAND:
            window->onCommand(wParam, lParam);
            return 0;

        case WM_CREATE:
            window->onCreate();
            return 0;

        case WM_CTLCOLORSTATIC:
            return window->onColorStatic(wParam);

        case WM_CTLCOLOREDIT:
            return window->onColorEdit(wParam);

        case WM_CTLCOLORBTN:
            return window->onColorButton(wParam);

        case WM_DESTROY:
            window->onDestroy();
            return 0;

        case WM_DRAWITEM:
            return window->onDraw(wParam, lParam);

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::onColorButton(WPARAM wParam) {
    HDC hdcButton = (HDC)wParam;
    SetTextColor(hdcButton, Color::WHITE);
    SetBkColor(hdcButton, Color::LIGHT_PURPLE);
    return (LRESULT)this->hBrushButton;
}

LRESULT Window::onColorEdit(WPARAM wParam) {
    HDC hdcEdit = (HDC)wParam;
    SetTextColor(hdcEdit, Color::WHITE);
    SetBkColor(hdcEdit, Color::DARK_PURPLE);
    return (LRESULT)this->hBrushEdit;
}

LRESULT Window::onColorStatic(WPARAM wParam) {
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, Color::LIGHT_PURPLE);
    SetBkColor(hdcStatic, Color::PURPLE);
    return (LRESULT)this->hBrushStatic;
}

void Window::onCommand(WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;

    if (LOWORD(wParam) == IDC_APPLY_BUTTON) {
        std::string path = this->configuration->getPath();
        LPCSTR file = path.c_str();

        struct stat buffer;

        // Get the current file attributes
        DWORD attributes = GetFileAttributesA(file);

        // Unset the read-only attribute, if it is set
        if (attributes & FILE_ATTRIBUTE_READONLY) {
            attributes &= ~FILE_ATTRIBUTE_READONLY;
            SetFileAttributesA(file, attributes);
        }

        if (stat(file, &buffer) == 0) {
            BOOL width_success;
            BOOL height_success;
            BOOL fps_success;

            int width = GetDlgItemInt(
                hWnd,
                IDC_WIDTH_EDIT,
                &width_success,
                FALSE
            );

            int height = GetDlgItemInt(
                hWnd,
                IDC_HEIGHT_EDIT,
                &height_success,
                FALSE
            );

            int fps = GetDlgItemInt(
                hWnd,
                IDC_FPS_EDIT,
                &fps_success,
                FALSE
            );

            if (width_success && height_success && fps_success) {
                // Get the state of the "Window Mode" radio buttons
                BOOL fs_checked = IsDlgButtonChecked(hWnd, IDC_FS_RADIO);
                BOOL wfs_checked = IsDlgButtonChecked(hWnd, IDC_WFS_RADIO);

                int mode = 0;

                // Set the "Window Mode" setting in the configuration file
                if (fs_checked) {
                    // Fullscreen
                    mode = 0;
                }
                else if (wfs_checked) {
                    // Windowed Fullscreen
                    mode = 1;
                }
                else {
                    // Windowed
                    mode = 2;
                }

                this->configuration->setConfiguration(width, height, fps, mode);

                MessageBoxA(
                    hWnd,
                    "Your settings were successfully saved.",
                    "Success",
                    MB_OK
                );
            }
            else {
                MessageBoxA(
                    hWnd,
                    "Please fill out every field.",
                    "Warning",
                    MB_OK | MB_ICONWARNING
                );
            }

            // Get the state of the read-only checkbox
            BOOL ro_checked = IsDlgButtonChecked(hWnd, IDC_RO_CHECKBOX);

            // Set the configuration file to read-only
            if (ro_checked) {
                SetFileAttributesA(file, FILE_ATTRIBUTE_READONLY);
            }
        }
        else {
            MessageBoxA(
                hWnd,
                "Configuration file could not be found.",
                "Error",
                MB_OK | MB_ICONERROR
            );
        }
    }
}

void Window::onCreate() {
    NONCLIENTMETRICS ncm = {};
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

    this->hFont = CreateFontIndirect(&ncm.lfMessageFont);
    this->hBrushStatic = CreateSolidBrush(Color::PURPLE);
    this->hBrushEdit = CreateSolidBrush(Color::DARK_PURPLE);
    this->hBrushButton = CreateSolidBrush(Color::LIGHT_PURPLE);

    std::wstring wwidth = this->configuration->getWidth();
    std::wstring wheight = this->configuration->getHeight();
    std::wstring wfps = this->configuration->getFrameRate();
    std::string wmode = this->configuration->getWindowMode();

    size_t width_length = wwidth.length();
    size_t height_length = wheight.length();

    LPCWSTR width = width_length >= 5 ? TEXT("1920") : wwidth.c_str();
    LPCWSTR height = height_length >= 5 ? TEXT("1080") : wheight.c_str();
    LPCWSTR fps = wfps.c_str();

    this->ui = new UserInterface(this->hWnd, this->hInstance);
    this->ui->setFont(this->hFont);
    this->ui->initialize();

    HWND width_edit = this->ui->getWidthEdit();
    HWND height_edit = this->ui->getHeightEdit();
    HWND fps_edit = this->ui->getFPSEdit();

    SetWindowText(width_edit, width);
    SetWindowText(height_edit, height);
    SetWindowText(fps_edit, fps);

    HWND fullscreen_radio = this->ui->getFullscreenRadio();
    HWND windowed_fullscreen_radio = this->ui->getWindowedFullscreenRadio();
    HWND windowed_radio = this->ui->getWindowedRadio();

    if (wmode == "0") {
        SendMessage(
            fullscreen_radio,
            BM_SETCHECK,
            BST_CHECKED,
            0
        );
    }
    else if (wmode == "1") {
        SendMessage(
            windowed_fullscreen_radio,
            BM_SETCHECK,
            BST_CHECKED,
            0
        );
    }
    else {
        SendMessage(
            windowed_radio,
            BM_SETCHECK,
            BST_CHECKED,
            0
        );
    }
}

LRESULT Window::onDraw(WPARAM wParam, LPARAM lParam) {
    // Draw the text on the "Apply" button
    if (wParam == IDC_APPLY_BUTTON) {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

        DrawTextA(
            pdis->hDC,
            "Apply",
            5,
            &pdis->rcItem,
            DT_CENTER | DT_SINGLELINE | DT_VCENTER
        );

        return TRUE;
    }

    return FALSE;
}

void Window::onDestroy() {
    DeleteObject(this->hFont);
    DeleteObject(this->hBrushStatic);
    DeleteObject(this->hBrushEdit);
    DeleteObject(this->hBrushButton);
    DeleteObject(this->hBrushBackground);

    delete this->ui;

    PostQuitMessage(0);
}
