#include "../include/color.h"
#include "../include/resources.h"
#include "../include/window.h"

Window::Window(HINSTANCE hInstance) : hWnd(nullptr), hInstance(hInstance) {
    this->configuration = new Configuration();
}

Window::~Window() {
    delete this->configuration;
}

void Window::applySettings() {
    std::wstring path = this->configuration->getPath();
    LPCWSTR file = path.c_str();

    if (!this->configuration->setWritableAttribute(path)) {
        MessageBoxW(
            hWnd,
            L"Unable to make the configuration file writable.",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return;
    }

    if (!this->configuration->createBackup()) {
        MessageBoxW(
            hWnd,
            L"Unable to create a backup of the configuration file.",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return;
    }

    struct _stat buffer;

    if (_wstat(file, &buffer) == 0) {
        BOOL width_success, height_success, fps_success;

        int width = GetDlgItemInt(hWnd, IDC_WIDTH_EDIT, &width_success, FALSE);
        int height = GetDlgItemInt(hWnd, IDC_HEIGHT_EDIT, &height_success, FALSE);
        int fps = GetDlgItemInt(hWnd, IDC_FPS_EDIT, &fps_success, FALSE);

        if (width_success && height_success && fps_success) {
            BOOL fs_checked = IsDlgButtonChecked(hWnd, IDC_FS_RADIO);
            BOOL wfs_checked = IsDlgButtonChecked(hWnd, IDC_WFS_RADIO);

            WindowMode mode = fs_checked
                ? MODE_FULLSCREEN
                : wfs_checked ? MODE_WINDOWED_FULLSCREEN : MODE_WINDOWED;

            this->configuration->setConfiguration(width, height, fps, mode);

            MessageBoxW(
                hWnd,
                L"The settings were successfully saved.",
                L"Success",
                MB_OK
            );

            this->configuration->reloadConfiguration();

            PostMessageW(this->hWnd, WM_USER_UPDATE_UI, 0, (LPARAM)this);
        } else {
            MessageBoxW(
                hWnd,
                L"Please fill out every field.",
                L"Warning",
                MB_OK | MB_ICONWARNING
            );
        }

        if (IsDlgButtonChecked(hWnd, IDC_RO_CHECKBOX)) {
            SetFileAttributesW(file, FILE_ATTRIBUTE_READONLY);
        }
    } else {
        MessageBoxW(
            hWnd,
            L"Configuration file could not be found.",
            L"Error",
            MB_OK | MB_ICONERROR
        );
    }
}

void Window::revertSettings() {
    if (!this->configuration->revertToBackup()) {
        MessageBoxW(
            hWnd,
            L"Unable to revert settings.",
            L"Error",
            MB_OK | MB_ICONERROR
        );
        return;
    }

    MessageBoxW(
        hWnd,
        L"The settings were reverted to the original state.",
        L"Success",
        MB_OK
    );

    this->configuration->reloadConfiguration();

    PostMessageW(this->hWnd, WM_USER_UPDATE_UI, 0, (LPARAM)this);
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

    this->hWnd = CreateWindowExW(
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

void Window::registerClass(const wchar_t* lpszClassName) {
    this->hBrushBackground = CreateSolidBrush(Color::PURPLE);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Window::WindowProc;
    wcex.hInstance = this->hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = this->hBrushBackground;
    wcex.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDR_ICON));
    wcex.lpszClassName = lpszClassName;

    if (!RegisterClassExW(&wcex)) {
        throw std::runtime_error("Unable to register window class.");
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

            case WM_USER_UPDATE_UI:
                if (lParam) {
                    Window* window = reinterpret_cast<Window*>(lParam);
                    window->ui->fromConfiguration(window->configuration);
                }

                return 0;

            case WM_CTLCOLORSTATIC:
                return window->onColorStatic(wParam);

            case WM_CTLCOLOREDIT:
                return window->onColorEdit(wParam);

            case WM_CTLCOLORBTN:
                return window->onColorButton(wParam, lParam);

            case WM_DESTROY:
                window->onDestroy();
                return 0;

            case WM_DRAWITEM:
                return window->onDraw(wParam, lParam);

            default:
                return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::onColorButton(WPARAM wParam, LPARAM lParam) {
    HDC hdcButton = (HDC)wParam;
    HWND hButton = (HWND)lParam;

    if (GetDlgCtrlID(hButton) == IDC_APPLY_BUTTON) {
        SetTextColor(hdcButton, Color::WHITE);
        SetBkColor(hdcButton, Color::LIGHT_PURPLE);
        return (LRESULT)this->hBrushApplyButton;
    }

    if (GetDlgCtrlID(hButton) == IDC_REVERT_BUTTON) {
        SetTextColor(hdcButton, Color::WHITE);
        SetBkColor(hdcButton, Color::GRAY);
        return (LRESULT)this->hBrushRevertButton;
    }

    return (LRESULT)this->hBrushApplyButton;
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
        this->applySettings();
    }

    if (LOWORD(wParam) == IDC_REVERT_BUTTON) {
        this->revertSettings();
    }
}

void Window::onCreate() {
    NONCLIENTMETRICSW ncm = {};
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

    this->hFont = CreateFontIndirectW(&ncm.lfMessageFont);

    this->hBrushStatic = CreateSolidBrush(Color::PURPLE);
    this->hBrushEdit = CreateSolidBrush(Color::DARK_PURPLE);
    this->hBrushApplyButton = CreateSolidBrush(Color::LIGHT_PURPLE);
    this->hBrushRevertButton = CreateSolidBrush(Color::GRAY);

    this->ui = new UserInterface(this->hWnd, this->hInstance);
    this->ui->setFont(this->hFont);
    this->ui->initialize();

    this->ui->fromConfiguration(this->configuration);
}

LRESULT Window::onDraw(WPARAM wParam, LPARAM lParam) {
    if (wParam == IDC_APPLY_BUTTON) {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

        DrawTextW(
            pdis->hDC,
            L"Apply",
            5,
            &pdis->rcItem,
            DT_CENTER | DT_SINGLELINE | DT_VCENTER
        );

        return TRUE;
    }

    if (wParam == IDC_REVERT_BUTTON) {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

        DrawTextW(
            pdis->hDC,
            L"Revert",
            6,
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
    DeleteObject(this->hBrushApplyButton);
    DeleteObject(this->hBrushRevertButton);
    DeleteObject(this->hBrushBackground);

    delete this->ui;

    PostQuitMessage(0);
}
