#define _USE_MATH_DEFINES
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Глобальные переменные X11
Display *display;
Window window;
GC gc;
Pixmap backBuffer;
int screenWidth = 800;
int screenHeight = 600;

// Функция для установки пикселя
void SetPixel(int x, int y, unsigned long color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        XSetForeground(display, gc, color);
        XDrawPoint(display, backBuffer, gc, x, y);
    }
}

// RGB макрос для создания цвета
unsigned long RGB(int r, int g, int b) {
    return ((unsigned long)(r & 0xFF) << 16) | ((unsigned long)(g & 0xFF) << 8) | (b & 0xFF);
}

// Функция для умножения матриц
void multi_mass(double *out, double *m1, int size1[], double *m2, int size2[]) {
    for (int i = 0; i < size1[0]; i++) {
        for (int j = 0; j < size2[1]; j++) {
            out[i * size2[1] + j] = 0;
            for (int k = 0; k < size1[1]; k++) {
                out[i * size2[1] + j] += m1[i * size1[1] + k] * m2[k * size2[1] + j];
            }
        }
    }
}

// Функция для копирования одной матрицы в другую
void copy_matrix(double *m1, double *m2, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        m1[i] = m2[i];
}

// Функция для округления элементов матрицы
void round_mat(double *m1, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        m1[i] = round(m1[i]);
}

// Функция для нормализации матрицы (однородные координаты)
void normirovka(double *mass, int size[]) {
    for (int i = 0; i < size[0]; i++) {
        if (mass[i * size[1] + 2] == 0)
            mass[i * size[1] + 2] = 1;
        else if (mass[i * size[1] + 2] != 1) {
            mass[i * size[1] + 0] /= mass[i * size[1] + 2];
            mass[i * size[1] + 1] /= mass[i * size[1] + 2];
            mass[i * size[1] + 2] = 1;
        }
    }
}

// Алгоритм Брезенхема для рисования линии
void bresenhamLine(double x0, double y0, double x1, double y1, unsigned long color = 0xFF0000) {
    double dx = fabs(x1 - x0);
    double sx = x0 < x1 ? 1 : -1;
    double dy = -fabs(y1 - y0);
    double sy = y0 < y1 ? 1 : -1;
    double err = dx + dy, e2;

    while (1) {
        SetPixel((int)x0, (int)y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Находит центр масс четырёхугольника (центроид)
void koord_peresech(double *out, double *quadr, int *size) {
    double x1 = quadr[0], x2 = quadr[3], x3 = quadr[6], x4 = quadr[9];
    double y1 = quadr[1], y2 = quadr[4], y3 = quadr[7], y4 = quadr[10];

    out[0] = (x1 + x2 + x3 + x4) / 4.0;
    out[1] = (y1 + y2 + y3 + y4) / 4.0;
    out[2] = 1;
}

// Выполняет преобразование: сдвиг, масштабирование, поворот
void deistvie(int chto_delaem, int size[], double *mass, double var[]) {
    if (chto_delaem == -1) return;

    double mat[3][3];
    int size_mat[2] = {3, 3};
    double res[4][3]; // 4 точки × 3 координаты
    double center[3] = {0, 0, 1};

    // Найти центр
    koord_peresech(center, mass, size);

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
}

// Заливка многоугольника методом сканирующей линии
void fillPolygon(double points[][3], int numPoints, unsigned long fillColor) {
    if (numPoints < 3) return;

    // Находим minY и maxY
    double yMin = points[0][1], yMax = points[0][1];
    for (int i = 1; i < numPoints; i++) {
        if (points[i][1] < yMin) yMin = points[i][1];
        if (points[i][1] > yMax) yMax = points[i][1];
    }

    // Проходим по каждой строке
    for (int y = (int)floor(yMin); y <= (int)ceil(yMax); y++) {
        double intersections[10];
        int count = 0;

        // Проверяем каждое ребро
        for (int i = 0; i < numPoints; i++) {
            int j = (i + 1) % numPoints;

            double x0 = points[i][0], y0 = points[i][1];
            double x1 = points[j][0], y1 = points[j][1];

            if ((y0 <= y && y < y1) || (y1 <= y && y < y0)) {
                if (y0 != y1) {
                    double x = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
                    intersections[count++] = x;
                }
            }
        }

        if (count == 0) continue;

        // Сортируем по X
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (intersections[i] > intersections[j]) {
                    double temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        // Закрашиваем от первого до последнего пересечения
        double xStart = intersections[0];
        double xEnd = intersections[count - 1];

        for (int x = (int)floor(xStart); x <= (int)ceil(xEnd); x++) {
            SetPixel(x, y, fillColor);
        }
    }
}

// Глобальные данные
double nachal_koord[4][3] = {{50, 50, 1}, {300, 50, 1}, {300, 275, 1}, {50, 400, 1}};

double krest[4][3];
double risunok[4][3];
int size[2] = {4, 3}; // 4 точки, 3 координаты (x, y, w)

// Функция отрисовки
void draw() {
    // Очистка буфера (белый фон)
    XSetForeground(display, gc, 0xFFFFFF);
    XFillRectangle(display, backBuffer, gc, 0, 0, screenWidth, screenHeight);

    // Копируем текущие координаты и округляем
    copy_matrix(&risunok[0][0], &krest[0][0], size);
    round_mat(&risunok[0][0], size);

    // Сначала — ЗАЛИВКА
    fillPolygon(risunok, 4, RGB(200, 230, 255)); // Светло-голубой

    // Потом — КОНТУР (линии поверх заливки)
    bresenhamLine(risunok[0][0], risunok[0][1], risunok[1][0], risunok[1][1], RGB(255, 0, 0)); // верх
    bresenhamLine(risunok[1][0], risunok[1][1], risunok[2][0], risunok[2][1], RGB(0, 255, 0)); // право
    bresenhamLine(risunok[2][0], risunok[2][1], risunok[3][0], risunok[3][1], RGB(0, 0, 255)); // низ
    bresenhamLine(risunok[3][0], risunok[3][1], risunok[0][0], risunok[0][1], RGB(255, 255, 0)); // лево

    // Копируем буфер на экран
    XCopyArea(display, backBuffer, window, gc, 0, 0, screenWidth, screenHeight, 0, 0);
    XFlush(display);
}

// Основная функция
int main() {
    // Открываем дисплей
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Не удалось открыть дисплей X11\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);
    
    // Создаём окно
    window = XCreateSimpleWindow(display, rootWindow, 
                                  100, 100, screenWidth, screenHeight, 1,
                                  BlackPixel(display, screen), 
                                  WhitePixel(display, screen));

    XStoreName(display, window, "Graphics Lab 3");

    // Выбираем события
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Создаём контекст графики
    gc = XCreateGC(display, window, 0, NULL);

    // Создаём буфер для двойной буферизации (Pixmap)
    backBuffer = XCreatePixmap(display, window, screenWidth, screenHeight, 
                               DefaultDepth(display, screen));
    
    // Заполняем буфер белым
    XSetForeground(display, gc, WhitePixel(display, screen));
    XFillRectangle(display, backBuffer, gc, 0, 0, screenWidth, screenHeight);

    // Подписываемся на событие закрытия окна
    Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(display, window, &wmDelete, 1);

    // Показываем окно
    XMapWindow(display, window);

    // Копируем начальные координаты
    copy_matrix(&krest[0][0], &nachal_koord[0][0], size);

    // Основной цикл событий
    XEvent event;
    bool running = true;
    bool needRedraw = true;

    while (running) {
        while (XPending(display) > 0) {
            XNextEvent(display, &event);

            switch (event.type) {
                case Expose:
                    needRedraw = true;
                    break;

                case KeyPress: {
                    KeySym key = XLookupKeysym(&event.xkey, 0);
                    double var[2] = {0, 0};
                    int chto_delaem = -1;

                    switch (key) {
                        case XK_w: case XK_W: var[0] = 0; var[1] = -5; chto_delaem = 0; break;
                        case XK_s: case XK_S: var[0] = 0; var[1] = 5; chto_delaem = 0; break;
                        case XK_a: case XK_A: var[0] = -5; var[1] = 0; chto_delaem = 0; break;
                        case XK_d: case XK_D: var[0] = 5; var[1] = 0; chto_delaem = 0; break;
                        case XK_x: case XK_X: var[0] = 0.95; var[1] = 0.95; chto_delaem = 1; break;
                        case XK_z: case XK_Z: var[0] = 1.05; var[1] = 1.05; chto_delaem = 1; break;
                        case XK_q: case XK_Q: var[0] = -M_PI / 36; chto_delaem = 2; break;
                        case XK_e: case XK_E: var[0] = M_PI / 36; chto_delaem = 2; break;
                        case XK_r: case XK_R:
                            copy_matrix(&krest[0][0], &nachal_koord[0][0], size);
                            needRedraw = true;
                            break;
                        case XK_Escape:
                            running = false;
                            break;
                        default: break;
                    }

                    if (chto_delaem != -1) {
                        deistvie(chto_delaem, size, &krest[0][0], var);
                        needRedraw = true;
                    }
                    break;
                }

                case ClientMessage:
                    running = false;
                    break;
            }
        }

        if (needRedraw) {
            draw();
            needRedraw = false;
        }

        usleep(10000); // 10ms задержка для снижения нагрузки на CPU
    }

    // Очистка ресурсов
    XFreePixmap(display, backBuffer);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
