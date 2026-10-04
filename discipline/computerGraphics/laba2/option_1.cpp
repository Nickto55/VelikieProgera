#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

Display *display;
Window window;
GC gc;
int screen;
int running = 1;

void multi_mass(double* out, double* m1, int size1[], double* m2, int size2[]) {
    for (int i = 0; i < size1[0]; i++) {
        for (int j = 0; j < size2[1]; j++) {
            *(out + (i * size2[1]) + j) = 0;
            for (int k = 0; k < size1[1]; k++) {
                *(out + (i * size2[1]) + j) += *(m1 + (i * size1[1]) + k) * (*(m2 + (k * size2[1]) + j));
            }
        }
    }
}

void copy_matrix(double* m1, double* m2, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = (*(m2 + i));
}

void round_mat(double* m1, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = std::round(*(m1 + i));
}

void normirovka(double* mass, int size[]) {
    for (int i = 0; i < size[0]; i++) {
        if (*(mass + (i * size[1]) + 2) == 0)
            *(mass + (i * size[1]) + 2) = 1;
        else if (*(mass + (i * size[1]) + 2) != 1) {
            *(mass + (i * size[1]) + 0) /= *(mass + (i * size[1]) + 2);
            *(mass + (i * size[1]) + 1) /= *(mass + (i * size[1]) + 2);
            *(mass + (i * size[1]) + 2) = 1;
        }
    }
}

void bresenhamLine(Display *dpy, Window win, GC gc_draw, double x0, double y0, double x1, double y1, unsigned long color) {
    double dx = std::abs(x1 - x0);
    double sx = x0 < x1 ? 1 : -1;
    double dy = -std::abs(y1 - y0);
    double sy = y0 < y1 ? 1 : -1;
    double err = dx + dy, e2;

    XSetForeground(dpy, gc_draw, color);

    while (1) {
        XDrawPoint(dpy, win, gc_draw, (int)x0, (int)y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    XFlush(dpy);
}

void find_center(double* center, double* mass, int size[]) {
    center[0] = center[1] = 0.0;
    for (int i = 0; i < size[0]; i++) {
        center[0] += mass[i * size[1] + 0];
        center[1] += mass[i * size[1] + 1];
    }
    center[0] /= size[0];
    center[1] /= size[0];
    center[2] = 1.0;
}

void deistvie(int chto_delaem, int size[], double* mass, double var[]) {
    if (chto_delaem == -1) return;

    double mat[3][3];
    int size_mat[2] = { 3, 3 };
    double res[3][3];
    double center[3];

    find_center(center, mass, size);

    mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
    mat[2][0] = -center[0]; mat[2][1] = -center[1]; mat[2][2] = 1;
    multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
    copy_matrix(mass, &res[0][0], size);

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
        mat[0][0] = std::cos(angle); mat[0][1] = std::sin(angle); mat[0][2] = 0;
        mat[1][0] = -std::sin(angle); mat[1][1] = std::cos(angle); mat[1][2] = 0;
        mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
    }

    multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
    copy_matrix(mass, &res[0][0], size);

    mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
    mat[2][0] = center[0]; mat[2][1] = center[1]; mat[2][2] = 1;
    multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
    copy_matrix(mass, &res[0][0], size);

    normirovka(mass, size);
}

double nachal_koord[3][3] = {
    {100, 100, 1},
    {300, 150, 1},
    {200, 300, 1}
};
double krest[3][3];
double risunok[3][3];
int size[2] = {3, 3};

void redraw() {
    XClearWindow(display, window);

    copy_matrix(&risunok[0][0], &krest[0][0], size);
    round_mat(&risunok[0][0], size);

    bresenhamLine(display, window, gc, risunok[0][0], risunok[0][1], risunok[1][0], risunok[1][1], BlackPixel(display, screen));
    bresenhamLine(display, window, gc, risunok[1][0], risunok[1][1], risunok[2][0], risunok[2][1], BlackPixel(display, screen));
    bresenhamLine(display, window, gc, risunok[2][0], risunok[2][1], risunok[0][0], risunok[0][1], BlackPixel(display, screen));

    XFlush(display);
}

int main() {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::fprintf(stderr, "Невозможно открыть дисплей\n");
        return 1;
    }

    screen = DefaultScreen(display);

    window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        100, 100, 800, 600,
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask | ClientMessage);
    XStoreName(display, window, "TransformApp");

    gc = XCreateGC(display, window, 0, NULL);

    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    copy_matrix(&krest[0][0], &nachal_koord[0][0], size);

    XMapWindow(display, window);

    // Основной цикл обработки событий
    while (running) {
        XEvent event;
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                redraw();
                break;
            case KeyPress: {
                double var[2] = { 0, 0 };
                int chto_delaem = -1;
                
                char buffer[32];
                KeySym keysym;
                XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, NULL);

                switch (keysym) {
                    case XK_w: case XK_W: var[0] = 0; var[1] = -5; chto_delaem = 0; break;
                    case XK_s: case XK_S: var[0] = 0; var[1] = 5; chto_delaem = 0; break;
                    case XK_a: case XK_A: var[0] = -5; var[1] = 0; chto_delaem = 0; break;
                    case XK_d: case XK_D: var[0] = 5; var[1] = 0; chto_delaem = 0; break;
                    case XK_x: case XK_X: var[0] = 0.95; var[1] = 0.95; chto_delaem = 1; break;
                    case XK_z: case XK_Z: var[0] = 1.05; var[1] = 1.05; chto_delaem = 1; break;
                    case XK_q: case XK_Q: var[0] = -M_PI / 36; chto_delaem = 2; break;
                    case XK_e: case XK_E: var[0] = M_PI / 36; chto_delaem = 2; break;
                    case XK_r: case XK_R: copy_matrix(&krest[0][0], &nachal_koord[0][0], size); redraw(); break;
                    case XK_Escape: running = 0; break;
                    default: break;
                }

                if (chto_delaem != -1) {
                    deistvie(chto_delaem, size, &krest[0][0], var);
                    redraw();
                }
                break;
            }
            case ClientMessage:
                if (event.xclient.data.l[0] == wmDeleteMessage) {
                    running = 0;
                }
                break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
