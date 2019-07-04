/*
 * Copyright (C) 2019 Brayden Carlson
 *
 * You may use, distribute and modify this
 * code under the terms of the MIT license.
 */

#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>
#include <Uxtheme.h>
#include <string>

#include "resources.h"
#include "simpleini/SimpleIni.h"

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

void CenterWindow(HWND window, DWORD style, DWORD exStyle) {
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    RECT client_rect;

    GetClientRect(window, &client_rect);
    AdjustWindowRectEx(&client_rect, style, FALSE, exStyle);

    int client_width = client_rect.right - client_rect.left;
    int client_height = client_rect.bottom - client_rect.top;

    SetWindowPos(window, NULL,
        screen_width / 2 - client_width / 2,
        screen_height / 2 - client_height / 2,
        client_width, client_height, 0);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"alphares";
    const wchar_t WINDOW_NAME[] = L"alphares";

    WNDCLASS wc = { };
    MSG message = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(43, 45, 92));
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDR_ICON));
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) {
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX^WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 250, 179,
        NULL,
        NULL,
        hInstance,
        NULL);

    CenterWindow(hwnd, WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX^WS_THICKFRAME, 0);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&message, NULL, 0, 0)) {
        if (!IsDialogMessage(hwnd, &message)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
    return 0;
}

std::string GetPath() {
    char* localappdata;
    size_t length;
    errno_t error =_dupenv_s(&localappdata, &length, "LOCALAPPDATA");

	if (localappdata != 0) {
		std::string file = "/FortniteGame/Saved/Config/WindowsClient/GameUserSettings.ini";
		std::string path = localappdata + file;
		free(localappdata);

		return path;
	}
}

void SetConfiguration(std::string path, int user_width, int user_height, int user_fps) {
    std::string width_string = std::to_string(user_width);
    std::string height_string = std::to_string(user_height);
	std::string fps_string = std::to_string(user_fps) + ".000000";
    const char* width = width_string.c_str();
    const char* height = height_string.c_str();
	const char* fps = fps_string.c_str();

	// Load file
	LPCSTR file = path.c_str();
    CSimpleIniA ini;
    ini.SetSpaces(false);
    ini.SetUnicode();
    ini.LoadFile(file);

	// Define section
	const char* section = "/Script/FortniteGame.FortGameUserSettings";

	// Set resolution
    ini.SetValue(section, "ResolutionSizeX", width);
    ini.SetValue(section, "ResolutionSizeY", height);
    ini.SetValue(section, "LastUserConfirmedResolutionSizeX", width);
    ini.SetValue(section, "LastUserConfirmedResolutionSizeY", height);
    ini.SetValue(section, "DesiredScreenWidth", width);
    ini.SetValue(section, "DesiredScreenHeight", height);
    ini.SetValue(section, "LastUserConfirmedDesiredScreenWidth", width);
    ini.SetValue(section, "LastUserConfirmedDesiredScreenHeight", height);

	// Set frame rate limit
	ini.SetValue(section, "FrameRateLimit", fps);

	// Save file
    ini.SaveFile(file);
}

void SetWindowMode(std::string path, int user_state) {
	std::string state_string = std::to_string(user_state);
	const char* state = state_string.c_str();

	// Define section
	const char* section = "/Script/FortniteGame.FortGameUserSettings";

	// Load file
	LPCSTR file = path.c_str();
	CSimpleIniA ini;
	ini.SetSpaces(false);
	ini.SetUnicode();
	ini.LoadFile(file);

	// Set window mode
	ini.SetValue(section, "LastConfirmedFullscreenMode", state);
	ini.SetValue(section, "PreferredFullscreenMode", state);
	ini.SetValue(section, "FullscreenMode", state);

	// Save file
	ini.SaveFile(file);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(ncm);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);

    HINSTANCE hInstance;
    static HFONT hFont = CreateFontIndirect(&ncm.lfMessageFont);
    static HBRUSH hBrushStatic = CreateSolidBrush(RGB(43, 45, 92));
    static HBRUSH hBrushEdit = CreateSolidBrush(RGB(35, 35, 79));
    static HBRUSH hBrushButton = CreateSolidBrush(RGB(93, 107, 238));

    switch (message) {
    case WM_CREATE:
        hInstance = GetModuleHandle(nullptr);

		// Width Label
        CreateWindowEx(
            NULL,
            TEXT("Static"),
            TEXT("Width"),
            WS_CHILD | WS_VISIBLE | ES_CENTER,
            10, 12, 60, 20,
            hwnd,
            (HMENU)IDC_WIDTH_LABEL,
            hInstance,
            NULL);

        SendMessage(
            GetDlgItem(hwnd, IDC_WIDTH_LABEL),
            WM_SETFONT,
            (WPARAM)hFont,
            TRUE);

		// Height Label
        CreateWindowEx(
            NULL,
            TEXT("Static"),
            TEXT("Height"),
            WS_CHILD | WS_VISIBLE | ES_CENTER,
            85, 12, 60, 20,
            hwnd,
            (HMENU)IDC_HEIGHT_LABEL,
            hInstance,
            NULL);

        SendMessage(
            GetDlgItem(hwnd, IDC_HEIGHT_LABEL),
            WM_SETFONT,
            (WPARAM)hFont,
            TRUE);

		// FPS Label
		CreateWindowEx(
			NULL,
			TEXT("Static"),
			TEXT("FPS"),
			WS_CHILD | WS_VISIBLE | ES_CENTER,
			159, 12, 60, 20,
			hwnd,
			(HMENU)IDC_FPS_LABEL,
			hInstance,
			NULL);

		SendMessage(
			GetDlgItem(hwnd, IDC_FPS_LABEL),
			WM_SETFONT,
			(WPARAM)hFont,
			TRUE);

		// Edit Width Box
		CreateWindowEx(
			NULL,
			TEXT("Edit"),
			TEXT("1920"),
			WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
			10, 32, 60, 15,
			hwnd,
			(HMENU)IDC_WIDTH_EDIT,
			hInstance,
			NULL);

        SendMessage(
            GetDlgItem(hwnd, IDC_WIDTH_EDIT),
            WM_SETFONT,
            (WPARAM)hFont,
            TRUE);

        SendMessage(
            GetDlgItem(hwnd, IDC_WIDTH_EDIT),
            EM_SETLIMITTEXT,
            4, 0);

		// Edit Height Box
        CreateWindowEx(
            NULL,
            TEXT("Edit"),
            TEXT("1080"),
            WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
            85, 32, 60, 15,
            hwnd,
            (HMENU)IDC_HEIGHT_EDIT,
            hInstance,
            NULL);

        SendMessage(
            GetDlgItem(hwnd, IDC_HEIGHT_EDIT),
            WM_SETFONT,
            (WPARAM)hFont,
            TRUE);

        SendMessage(
            GetDlgItem(hwnd, IDC_HEIGHT_EDIT),
            EM_SETLIMITTEXT,
            4, 0);

		// Edit FPS Box
		CreateWindowEx(
			NULL,
			TEXT("Edit"),
			TEXT("240"),
			WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER | WS_TABSTOP,
			159, 32, 60, 15,
			hwnd,
			(HMENU)IDC_FPS_EDIT,
			hInstance,
			NULL);

		SendMessage(
			GetDlgItem(hwnd, IDC_FPS_EDIT),
			WM_SETFONT,
			(WPARAM)hFont,
			TRUE);

		SendMessage(
			GetDlgItem(hwnd, IDC_FPS_EDIT),
			EM_SETLIMITTEXT,
			3, 0);

		// Fullscreen Checkbox
		CreateWindowEx(
			NULL,
			TEXT("Button"),
			TEXT("Fullscreen"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 56, 90, 15,
			hwnd,
			(HMENU)IDC_FS_CHECKBOX,
			hInstance,
			NULL);

		SendMessage(
			GetDlgItem(hwnd, IDC_FS_CHECKBOX),
			WM_SETFONT,
			(WPARAM)hFont,
			TRUE);

		SendMessage(
			GetDlgItem(hwnd, IDC_FS_CHECKBOX),
			BM_SETCHECK,
			BST_CHECKED,
			0);


		// Read-only Checkbox
		CreateWindowEx(
			NULL,
			TEXT("Button"),
			TEXT("Read-only"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 76, 90, 15,
			hwnd,
			(HMENU)IDC_RO_CHECKBOX,
			hInstance,
			NULL);

		SendMessage(
			GetDlgItem(hwnd, IDC_RO_CHECKBOX),
			WM_SETFONT,
			(WPARAM)hFont,
			TRUE);

		SendMessage(
			GetDlgItem(hwnd, IDC_RO_CHECKBOX),
			BM_SETCHECK,
			BST_CHECKED,
			0);

		// Apply Button
        CreateWindowEx(
            NULL,
            TEXT("Button"),
            TEXT("Apply"),
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            10, 100, 209, 25,
            hwnd,
            (HMENU)IDC_APPLY_BUTTON,
            hInstance,
            NULL);

        SendMessage(
            GetDlgItem(hwnd, IDC_APPLY_BUTTON),
            WM_SETFONT,
            (WPARAM)hFont,
            TRUE);

        break;

	// Set background and text color for static controls
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(93, 107, 238));
        SetBkColor(hdcStatic, RGB(43, 45, 92));
        return (INT_PTR)hBrushStatic;
    }

	// Set background and text color for edit controls
    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wParam;
        SetTextColor(hdcEdit, RGB(255, 255, 255));
        SetBkColor(hdcEdit, RGB(35, 35, 79));
        return (INT_PTR)hBrushEdit;
    }

	// Set background and text color for button controls
    case WM_CTLCOLORBTN:
    {
        HDC hdcButton = (HDC)wParam;
        SetTextColor(hdcButton, RGB(255, 255, 255));
        SetBkColor(hdcButton, RGB(93, 107, 238));
        return (INT_PTR)hBrushButton;
    }

    case WM_COMMAND:
		// Apply the settings to the configuration file
        if (LOWORD(wParam) == IDC_APPLY_BUTTON) {
            std::string path = GetPath();
            LPCSTR file = path.c_str();
            BOOL ro_checked = IsDlgButtonChecked(hwnd, IDC_RO_CHECKBOX);
			BOOL fs_checked = IsDlgButtonChecked(hwnd, IDC_FS_CHECKBOX);
            struct stat buffer;

			DWORD attributes = GetFileAttributesA(file);

			if (attributes & FILE_ATTRIBUTE_READONLY) {
				attributes &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributesA(file, attributes);
			}

            if (stat(file, &buffer) == 0) {
                BOOL success;

                int width = GetDlgItemInt(
                    hwnd,
                    IDC_WIDTH_EDIT,
                    &success,
                    FALSE);

                int height = GetDlgItemInt(
                    hwnd,
                    IDC_HEIGHT_EDIT,
                    &success,
                    FALSE);

				int fps = GetDlgItemInt(
					hwnd,
					IDC_FPS_EDIT,
					&success,
					FALSE);

                if (success) {
                    SetConfiguration(path, width, height, fps);

                    MessageBoxA(
                        hwnd,
                        "Your settings were successfully saved.",
                        "Success",
                        MB_OK);
                } else {
                    MessageBoxA(
                        hwnd,
                        "Please enter a resolution.",
                        "Warning",
                        MB_OK | MB_ICONWARNING);
                }

				// Set the fullscreen option(s) in the configuration file
				if (fs_checked) {
					// 0 sets the Window Mode to Fullscreen
					SetWindowMode(file, 0);
				}
				else {
					// 1 sets the Window Mode to Windowed Fullscreen
					SetWindowMode(file, 1);
				}

				// Set the configuration file to read-only
                if (ro_checked) {
                    SetFileAttributesA(file, FILE_ATTRIBUTE_READONLY);
                }
            } else {
                MessageBoxA(
                    hwnd,
                    "There was an error finding your configuration file.",
                    "Error",
                    MB_OK | MB_ICONERROR);
            }
        }
        break;

    case WM_DRAWITEM:
        if (wParam == IDC_APPLY_BUTTON) {
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

            DrawTextA(
                pdis->hDC,
                "Apply",
                5,
                &pdis->rcItem,
                DT_CENTER | DT_SINGLELINE | DT_VCENTER);

            return TRUE;
        }

        break;

    case WM_DESTROY:
        DeleteObject(hFont);
        DeleteObject(hBrushStatic);
        DeleteObject(hBrushEdit);
        DeleteObject(hBrushButton);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    }
    return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
