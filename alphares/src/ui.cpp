#include "ui.h"

UserInterface::UserInterface(HWND hWnd, HINSTANCE hInstance) {
    this->hWnd = hWnd;
    this->hInstance = hInstance;
}

void UserInterface::initialize() {
    this->createButton();
    this->createCheckbox();
    this->createEdit();
    this->createLabel();
    this->createRadio();
}

void UserInterface::createButton() {
    this->apply_button = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Apply"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
        10, 185, 209, 25,
        this->hWnd,
        (HMENU)IDC_APPLY_BUTTON,
        this->hInstance,
        0
    );

    SendMessage(
        this->apply_button,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::createCheckbox() {
    this->readonly_checkbox = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Read-only"),
        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
        10, 160, 90, 15,
        this->hWnd,
        (HMENU)IDC_RO_CHECKBOX,
        this->hInstance,
        0
    );

    SendMessage(
        this->readonly_checkbox,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessage(
        this->readonly_checkbox,
        BM_SETCHECK,
        BST_CHECKED,
        0
    );
}

void UserInterface::createEdit() {
    this->width_edit = CreateWindowEx(
        0,
        TEXT("Edit"),
        TEXT("1920"),
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        10, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_WIDTH_EDIT,
        this->hInstance,
        0
    );

    SendMessage(
        this->width_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessage(
        this->width_edit,
        EM_SETLIMITTEXT,
        4,
        0
    );

    this->height_edit = CreateWindowEx(
        0,
        TEXT("Edit"),
        TEXT("1080"),
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        85, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_HEIGHT_EDIT,
        this->hInstance,
        0
    );

    SendMessage(
        this->height_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessage(
        this->height_edit,
        EM_SETLIMITTEXT,
        4,
        0
    );

    this->fps_edit = CreateWindowEx(
        0,
        TEXT("Edit"),
        TEXT("0"),
        WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
        159, 32, 60, 15,
        this->hWnd,
        (HMENU)IDC_FPS_EDIT,
        this->hInstance,
        0
    );

    SendMessage(
        this->fps_edit,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    SendMessage(
        this->fps_edit,
        EM_SETLIMITTEXT,
        3,
        0
    );
}

void UserInterface::createLabel() {
    this->width_label = CreateWindowEx(
        0,
        TEXT("Static"),
        TEXT("Width"),
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        10, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_WIDTH_LABEL,
        this->hInstance,
        0
    );

    SendMessage(
        this->width_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->height_label = CreateWindowEx(
        0,
        TEXT("Static"),
        TEXT("Height"),
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        85, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_HEIGHT_LABEL,
        this->hInstance,
        0
    );

    SendMessage(
        this->height_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->fps_label = CreateWindowEx(
        0,
        TEXT("Static"),
        TEXT("FPS"),
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        159, 12, 60, 20,
        this->hWnd,
        (HMENU)IDC_FPS_LABEL,
        this->hInstance,
        0
    );

    SendMessage(
        this->fps_label,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
}

void UserInterface::createRadio() {
    this->radio_group = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Window Mode"),
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX | BS_CENTER,
        10, 60, 209, 85,
        this->hWnd,
        (HMENU)IDC_GROUP_RADIO,
        this->hInstance,
        0
    );

    SendMessage(
        this->radio_group,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->fullscreen_radio = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Fullscreen"),
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP,
        20, 80, 90, 15,
        this->hWnd,
        (HMENU)IDC_FS_RADIO,
        this->hInstance,
        0
    );

    SendMessage(
        this->fullscreen_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->windowed_fullscreen_radio = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Windowed Fullscreen"),
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP,
        20, 100, 140, 15,
        this->hWnd,
        (HMENU)IDC_WFS_RADIO,
        this->hInstance,
        0
    );

    SendMessage(
        this->windowed_fullscreen_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );

    this->windowed_radio = CreateWindowEx(
        0,
        TEXT("Button"),
        TEXT("Windowed"),
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP,
        20, 120, 100, 15,
        this->hWnd,
        (HMENU)IDC_W_RADIO,
        this->hInstance,
        0
    );

    SendMessage(
        this->windowed_radio,
        WM_SETFONT,
        (WPARAM)this->hFont,
        TRUE
    );
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
