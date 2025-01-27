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

    bool is_writeable = this->configuration->unsetReadOnlyAttribute(path);

    if (!is_writeable) {
        MessageBoxW(
            hWnd,
            L"Unable to make the configuration file writable.",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return;
    }

    bool success = this->configuration->createBackup();

    if (!success) {
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
    bool success = this->configuration->revertToBackup();

    if (!success) {
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
                    window->ui->fromConfiguration(window->configuration);
                }

                return 0;

            case WM_CTLCOLORSTATIC:
                return window->ui->onColorStatic(wParam);

            case WM_CTLCOLOREDIT:
                return window->ui->onColorEdit(wParam);

            case WM_CTLCOLORBTN:
                return window->ui->onColorButton(wParam, lParam);

            case WM_DESTROY:
                window->onDestroy();
                return 0;

            case WM_DRAWITEM:
                return window->ui->onDraw(wParam, lParam);

            default:
                return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void Window::onCommand(WPARAM wParam, LPARAM lParam) {
    (void)lParam;

    switch (LOWORD(wParam)) {
        case IDC_APPLY_BUTTON:
            this->applySettings();
            break;

        case IDC_REVERT_BUTTON:
            this->revertSettings();
            break;

        default:
            break;
    }
}

void Window::onCreate() {
    NONCLIENTMETRICSW ncm = {};
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

    this->hFont = CreateFontIndirectW(&ncm.lfMessageFont);

    this->ui = new UserInterface(this->hWnd, this->hInstance);
    this->ui->setFont(this->hFont);
    this->ui->initialize();
    this->ui->fromConfiguration(this->configuration);
}

void Window::onDestroy() {
    DeleteObject(this->hFont);
    DeleteObject(this->hBrushBackground);
    delete this->ui;
    PostQuitMessage(0);
}
