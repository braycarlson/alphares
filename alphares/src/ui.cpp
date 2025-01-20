#include "../include/ui.h"

UserInterface::UserInterface(HWND hWnd, HINSTANCE hInstance) {
    this->hWnd = hWnd;
    this->hInstance = hInstance;
}

void UserInterface::initialize() {
    this->createApplyButton();
    this->createRevertButton();
    this->createCheckbox();
    this->createEdit();
    this->createLabel();
    this->createRadio();
}

void UserInterface::createApplyButton() {
    this->apply_button = CreateWindowExW(
        0,
        L"Button",
        L"Apply",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
        10, 185, 100, 25,
        this->hWnd,
        (HMENU)IDC_APPLY_BUTTON,
        this->hInstance,
        0
    );

    SendMessageW(
        this->apply_button,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::createRevertButton() {
    int width = 100;
    int gap = 10;
    int x = 10 + width + gap;

    this->revert_button = CreateWindowExW(
        0,
        L"Button",
        L"Revert",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
        x, 185, width, 25,
        this->hWnd,
        (HMENU)IDC_REVERT_BUTTON,
        this->hInstance,
        0
    );

    SendMessageW(
        this->revert_button,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::createCheckbox() {
    this->readonly_checkbox = CreateWindowExW(
        0,
        L"Button",
        L"Read-only",
        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
        10, 160, 90, 15,
        this->hWnd,
        (HMENU)IDC_RO_CHECKBOX,
        this->hInstance,
        0
    );

    SendMessageW(
        this->readonly_checkbox,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessageW(
        this->readonly_checkbox,
        BM_SETCHECK,
        BST_CHECKED,
        0
    );
}

void UserInterface::createEdit() {
    this->width_edit = CreateWindowExW(
        0,
        L"Edit",
        L"1920",
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        10, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_WIDTH_EDIT,
        this->hInstance,
        0
    );

    SendMessageW(
        this->width_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessageW(
        this->width_edit,
        EM_SETLIMITTEXT,
        4,
        0
    );

    this->height_edit = CreateWindowExW(
        0,
        L"Edit",
        L"1080",
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        85, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_HEIGHT_EDIT,
        this->hInstance,
        0
    );

    SendMessageW(
        this->height_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessageW(
        this->height_edit,
        EM_SETLIMITTEXT,
        4,
        0
    );

    this->fps_edit = CreateWindowExW(
        0,
        L"Edit",
        L"0",
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        159, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_FPS_EDIT,
        this->hInstance,
        0
    );

    SendMessageW(
        this->fps_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessageW(
        this->fps_edit,
        EM_SETLIMITTEXT,
        3,
        0
    );
}

void UserInterface::createLabel() {
    this->width_label = CreateWindowExW(
        0,
        L"Static",
        L"Width",
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        10, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_WIDTH_LABEL,
        this->hInstance,
        0
    );

    SendMessageW(
        this->width_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->height_label = CreateWindowExW(
        0,
        L"Static",
        L"Height",
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        85, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_HEIGHT_LABEL,
        this->hInstance,
        0
    );

    SendMessageW(
        this->height_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->fps_label = CreateWindowExW(
        0,
        L"Static",
        L"FPS",
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        159, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_FPS_LABEL,
        this->hInstance,
        0
    );

    SendMessageW(
        this->fps_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::createRadio() {
    this->radio_group = CreateWindowExW(
        0,
        L"Button",
        L"Window Mode",
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX | BS_CENTER,
        10, 60, 209, 85,
        this->hWnd,
        (HMENU)IDC_GROUP_RADIO,
        this->hInstance,
        0
    );

    SendMessageW(
        this->radio_group,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->fullscreen_radio = CreateWindowExW(
        0,
        L"Button",
        L"Fullscreen",
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP,
        20, 80, 90, 15,
        this->hWnd,
        (HMENU)IDC_FS_RADIO,
        this->hInstance,
        0
    );

    SendMessageW(
        this->fullscreen_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->windowed_fullscreen_radio = CreateWindowExW(
        0,
        L"Button",
        L"Windowed Fullscreen",
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP,
        20, 100, 140, 15,
        this->hWnd,
        (HMENU)IDC_WFS_RADIO,
        this->hInstance,
        0
    );

    SendMessageW(
        this->windowed_fullscreen_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->windowed_radio = CreateWindowExW(
        0,
        L"Button",
        L"Windowed",
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP,
        20, 120, 100, 15,
        this->hWnd,
        (HMENU)IDC_W_RADIO,
        this->hInstance,
        0
    );

    SendMessageW(
        this->windowed_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::fromConfiguration(Configuration* configuration) {
    std::wstring width = configuration->getWidth();
    std::wstring height = configuration->getHeight();
    std::wstring fps = configuration->getFrameRate();
    std::wstring windowMode = configuration->getWindowMode();

    SetWindowTextW(this->width_edit, width.c_str());
    SetWindowTextW(this->height_edit, height.c_str());
    SetWindowTextW(this->fps_edit, fps.c_str());

    SendMessageW(this->fullscreen_radio, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessageW(this->windowed_fullscreen_radio, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessageW(this->windowed_radio, BM_SETCHECK, BST_UNCHECKED, 0);

    if (windowMode == L"0") {
        SendMessageW(this->fullscreen_radio, BM_SETCHECK, BST_CHECKED, 0);
    } else if (windowMode == L"1") {
        SendMessageW(this->windowed_fullscreen_radio, BM_SETCHECK, BST_CHECKED, 0);
    } else {
        SendMessageW(this->windowed_radio, BM_SETCHECK, BST_CHECKED, 0);
    }

    SendMessageW(this->readonly_checkbox, BM_SETCHECK, BST_UNCHECKED, 0);

    std::wstring path = configuration->getPath();
    LPCWSTR file = path.c_str();

    DWORD attributes = GetFileAttributesW(file);

    if (attributes & FILE_ATTRIBUTE_READONLY) {
        SendMessageW(this->readonly_checkbox, BM_SETCHECK, BST_CHECKED, 0);
    } else {
        SendMessageW(this->readonly_checkbox, BM_SETCHECK, BST_UNCHECKED, 0);
    }
}

HWND UserInterface::getWidthEdit() {
    return this->width_edit;
}

HWND UserInterface::getHeightEdit() {
    return this->height_edit;
}

HWND UserInterface::getFPSEdit() {
    return this->fps_edit;
}

HWND UserInterface::getFullscreenRadio() {
    return this->fullscreen_radio;
}

HWND UserInterface::getWindowedFullscreenRadio() {
    return this->windowed_fullscreen_radio;
}

HWND UserInterface::getWindowedRadio() {
    return this->windowed_radio;
}

void UserInterface::setFont(HFONT font) {
    this->hFont = font;
}
