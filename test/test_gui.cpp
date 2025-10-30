#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>

// Простое Windows приложение для тестирования графики
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Очистка фона
            SetBkColor(hdc, RGB(255, 255, 255));
            Rectangle(hdc, 0, 0, 800, 600);
            
            // Рисование простых фигур
            SetPixel(hdc, 100, 100, RGB(255, 0, 0));
            MoveToEx(hdc, 50, 50, NULL);
            LineTo(hdc, 200, 150);
            
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TestWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Ошибка регистрации класса!", "Ошибка", MB_OK);
        return 1;
    }
    
    HWND hwnd = CreateWindowEx(
        0, "TestWindow", "Тест Windows GUI", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );
    
    if (hwnd == NULL) {
        MessageBox(NULL, "Ошибка создания окна!", "Ошибка", MB_OK);
        return 1;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}