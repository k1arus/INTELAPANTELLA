#include "SerialBridge.h"
#include <windows.h>

const char CLASS_NAME[] = "RedScreenWindow";

bool showRed = false;
DWORD redUntil = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        HBRUSH brush;

        if (showRed) {
            brush = CreateSolidBrush(RGB(255, 0, 0));
        } else {
            brush = CreateSolidBrush(RGB(0, 0, 0));
        }

        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    if (!openSerial()) {
        MessageBox(nullptr, "COM3 open failed", "Error", MB_OK);
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Red Screen",
        WS_POPUP,
        0,
        0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

    auto triggerSuccess = [&]() {
        showRed = true;
        redUntil = GetTickCount() + 10000;

        InvalidateRect(hwnd, nullptr, TRUE);
    };

    MSG msg = {};

    bool keyWasDown = false;

    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (receivedSuccess()) {
            triggerSuccess();
        }

        bool keyDown =
            (GetAsyncKeyState('U') & 0x8000) ||
            (GetAsyncKeyState('F') & 0x8000);

        if (keyDown && !keyWasDown) {
            triggerSuccess();
        }

        keyWasDown = keyDown;

        if (showRed && GetTickCount() > redUntil) {
            showRed = false;

            InvalidateRect(hwnd, nullptr, TRUE);
        }

        Sleep(10);
    }
}

