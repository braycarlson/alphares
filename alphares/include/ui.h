#pragma once

#include <windows.h>

#include "../include/color.h"
#include "../include/configuration.h"
#include "../include/resources.h"

class UserInterface {
public:
    UserInterface(HWND hwnd, HINSTANCE hInstance);
    ~UserInterface();

    void initialize();
    void fromConfiguration(Configuration*);
    void setFont(HFONT);

    void createControl();
    void createBrush();

    LRESULT onColorButton(WPARAM wParam, LPARAM lParam);
    LRESULT onColorEdit(WPARAM wParam);
    LRESULT onColorStatic(WPARAM wParam);
    LRESULT onDraw(WPARAM, LPARAM);

    HWND getWidthEdit();
    HWND getHeightEdit();
    HWND getFPSEdit();
    HWND getFullscreenRadio();
    HWND getWindowedFullscreenRadio();
    HWND getWindowedRadio();

private:
    HWND hWnd = nullptr;
    HINSTANCE hInstance = nullptr;

    HFONT hFont = nullptr;
    HBRUSH hBrushStatic = nullptr;
    HBRUSH hBrushEdit = nullptr;
    HBRUSH hBrushApplyButton = nullptr;
    HBRUSH hBrushRevertButton = nullptr;

    HWND apply_button = nullptr;
    HWND revert_button = nullptr;
    HWND readonly_checkbox = nullptr;
    HWND width_edit = nullptr;
    HWND height_edit = nullptr;
    HWND fps_edit = nullptr;
    HWND width_label = nullptr;
    HWND height_label = nullptr;
    HWND fps_label = nullptr;
    HWND radio_group = nullptr;
    HWND fullscreen_radio = nullptr;
    HWND windowed_fullscreen_radio = nullptr;
    HWND windowed_radio = nullptr;

    void createApplyButton();
    void createRevertButton();
    void createCheckbox();
    void createEdit();
    void createLabel();
    void createRadio();
};
