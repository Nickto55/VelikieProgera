#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>

// Функция для умножения матриц
void multi_mass(double *out, double *m1, int size1[], double *m2, int size2[]) {
    for (int i = 0; i < size1[0]; i++) {
        for (int j = 0; j < size2[1]; j++) {
            *(out + (i * size2[1]) + j) = 0;
            for (int k = 0; k < size1[1]; k++) {
                *(out + (i * size2[1]) + j) += *(m1 + (i * size1[1]) + k) * (*(m2 + (k * size2[1]) + j));
            }
        }
    }
}

// Функция для копирования одной матрицы в другую
void copy_matrix(double *m1, double *m2, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = (*(m2 + i));
}

// Функция для округления элементов матрицы
void round_mat(double *m1, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = round(*(m1 + i));
}

// Функция для нормализации матрицы
void normirovka(double *mass, int size[]) {
    for (int i = 0; i < size[0]; i++) {
        if (*(mass + (i * size[1]) + 2) == 0)
            *(mass + (i * size[1]) + 2) = 1;
        else if (*(mass + (i * size[1]) + 2) != 1) {
            *(mass + (i * size[1]) + 0) = (*(mass + (i * size[1]) + 0) / *(mass + (i * size[1]) + 2));
            *(mass + (i * size[1]) + 1) = (*(mass + (i * size[1]) + 1) / *(mass + (i * size[1]) + 2));
            *(mass + (i * size[1]) + 2) = 1;
        }
    }
}

// Алгоритм Брезенхема для рисования линии (GDI)
void bresenhamLine(HDC hdc, double x0, double y0, double x1, double y1, COLORREF color = RGB(255, 0, 0)) {
    double dx = fabsf(x1 - x0);
    double sx = x0 < x1 ? 1 : -1;
    double dy = -fabsf(y1 - y0);
    double sy = y0 < y1 ? 1 : -1;
    double err = dx + dy, e2;

    while (1) {
        SetPixel(hdc, (int)x0, (int)y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Находит точку пересечения двух отрезков (как центр масс)
void koord_peresech(double *out, double *krest, int *size) {
    double x1 = *(krest + 0), x2 = *(krest + 3), x3 = *(krest + 6), x4 = *(krest + 9);
    double y1 = *(krest + 1), y2 = *(krest + 4), y3 = *(krest + 7), y4 = *(krest + 10);

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) return; // Параллельны

    *(out + 0) = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
    *(out + 1) = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;
    *(out + 2) = 1;
}

// Выполняет преобразование: сдвиг, масштабирование, поворот
void deistvie(int chto_delaem, int size[], double *mass, double var[]) {
    if (chto_delaem != -1) {
        double mat[3][3];
        int size_mat[2] = {3, 3};
        double res[4][3]; // Максимум 4 точки
        double center[3] = {0, 0, 1};

        // Найти центр как точку пересечения диагоналей
        koord_peresech(&center[0], mass, size);

        // Сдвинуть к центру
        mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
        mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
        mat[2][0] = -center[0]; mat[2][1] = -center[1]; mat[2][2] = 1;
        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        // Применить преобразование
        if (chto_delaem == 0) { // Сдвиг
            mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
            mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
            mat[2][0] = var[0]; mat[2][1] = var[1]; mat[2][2] = 1;
        }
        else if (chto_delaem == 1) { // Масштабирование
            mat[0][0] = var[0]; mat[0][1] = 0; mat[0][2] = 0;
            mat[1][0] = 0; mat[1][1] = var[1]; mat[1][2] = 0;
            mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
        }
        else if (chto_delaem == 2) { // Поворот
            double angle = var[0];
            mat[0][0] = cos(angle); mat[0][1] = sin(angle); mat[0][2] = 0;
            mat[1][0] = -sin(angle); mat[1][1] = cos(angle); mat[1][2] = 0;
            mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
        }

        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        // Вернуть из центра
        mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
        mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
        mat[2][0] = center[0]; mat[2][1] = center[1]; mat[2][2] = 1;
        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        normirovka(mass, size);
        koord_peresech(&center[0], mass, size);
    }
}

// Глобальные данные
double nachal_koord[4][3] = {{0, 190, 1}, {300, 275, 1}, {40, 10, 1}, {50, 400, 1}};
double krest[4][3];
double risunok[4][3];
int size[2] = {4, 3};

// Обработчик окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static bool needRedraw = true;

    switch (uMsg) {
        case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Очистка фона — ключевое изменение!
    SetBkColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, OPAQUE);
    Rectangle(hdc, 0, 0, 1900, 1200);

    // Настройка для рисования линий
    SetBkMode(hdc, TRANSPARENT);

    // Копируем текущие координаты и округляем
    copy_matrix(&risunok[0][0], &krest[0][0], size);
    round_mat(&risunok[0][0], size);

    // Рисуем первую линию (Брезенхем)
    bresenhamLine(hdc, risunok[0][0], risunok[0][1], risunok[1][0], risunok[1][1], RGB(255, 0, 0));

    // Рисуем вторую линию (GDI LineTo)
    MoveToEx(hdc, (int)risunok[2][0], (int)risunok[2][1], NULL);
    LineTo(hdc, (int)risunok[3][0], (int)risunok[3][1]);

    EndPaint(hwnd, &ps);
    needRedraw = false;
    break;
}

        case WM_KEYDOWN: {
            double var[2] = {0, 0};
            int chto_delaem = -1;

            switch (wParam) {
                case 'W': case 'w': var[0] = 0; var[1] = -5; chto_delaem = 0; break;
                case 'S': case 's': var[0] = 0; var[1] = 5; chto_delaem = 0; break;
                case 'A': case 'a': var[0] = -5; var[1] = 0; chto_delaem = 0; break;
                case 'D': case 'd': var[0] = 5; var[1] = 0; chto_delaem = 0; break;
                case 'X': case 'x': var[0] = 0.95; var[1] = 0.95; chto_delaem = 1; break;
                case 'Z': case 'z': var[0] = 1.05; var[1] = 1.05; chto_delaem = 1; break;
                case 'Q': case 'q': var[0] = -M_PI / 36; chto_delaem = 2; break;
                case 'E': case 'e': var[0] = M_PI / 36; chto_delaem = 2; break;
                case 'R': case 'r': copy_matrix(&krest[0][0], &nachal_koord[0][0], size); needRedraw = true; break;
                case VK_ESCAPE: PostQuitMessage(0); break;
                default: break;
            }

            if (chto_delaem != -1) {
                deistvie(chto_delaem, size, &krest[0][0], var);
                needRedraw = true;
                InvalidateRect(hwnd, NULL, FALSE); // Перерисовать
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TransformApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна!", L"Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        0,
        L"TransformApp",
        L"Преобразования фигур (WSAD: сдвиг, QE: поворот, ZX: масштаб, R: сброс, ESC: выход)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Не удалось создать окно!", L"Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Копируем начальные координаты
    copy_matrix(&krest[0][0], &nachal_koord[0][0], size);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
