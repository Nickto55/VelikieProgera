#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

void copy_matrix(double *m1, double *m2, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = (*(m2 + i));
}

void round_mat(double *m1, int size[]) {
    for (int i = 0; i < (size[0] * size[1]); i++)
        *(m1 + i) = round(*(m1 + i));
}

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

void deistvie(int chto_delaem, int size[], double *mass, double var[]) {
    if (chto_delaem != -1) {
        double mat[3][3];
        int size_mat[2] = {3, 3};
        double res[4][3];
        double center[3] = {0, 0, 1};

        double sum_x = 0, sum_y = 0;
        for(int i = 0; i < size[0]; i++) {
            sum_x += *(mass + i * size[1] + 0);
            sum_y += *(mass + i * size[1] + 1);
        }
        center[0] = sum_x / size[0];
        center[1] = sum_y / size[0];

        memset(mat, 0, sizeof(mat));
        mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
        mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
        mat[2][0] = -center[0]; mat[2][1] = -center[1]; mat[2][2] = 1;
        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        memset(mat, 0, sizeof(mat));
        if (chto_delaem == 0) {
            mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
            mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
            mat[2][0] = var[0]; mat[2][1] = var[1]; mat[2][2] = 1;
        }
        else if (chto_delaem == 1) {
            mat[0][0] = var[0]; mat[0][1] = 0; mat[0][2] = 0;
            mat[1][0] = 0; mat[1][1] = var[1]; mat[1][2] = 0;
            mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
        }
        else if (chto_delaem == 2) {
            double angle = var[0];
            mat[0][0] = cos(angle); mat[0][1] = sin(angle); mat[0][2] = 0;
            mat[1][0] = -sin(angle); mat[1][1] = cos(angle); mat[1][2] = 0;
            mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
        }

        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        memset(mat, 0, sizeof(mat));
        mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0;
        mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
        mat[2][0] = center[0]; mat[2][1] = center[1]; mat[2][2] = 1;
        multi_mass(&res[0][0], mass, size, &mat[0][0], size_mat);
        copy_matrix(mass, &res[0][0], size);

        normirovka(mass, size);
    }
}

void bresenhamLine(cairo_surface_t *surface, double x0, double y0, double x1, double y1, unsigned char r, unsigned char g, unsigned char b) {
    int width = cairo_image_surface_get_width(surface);
    int height = cairo_image_surface_get_height(surface);

    int x0_i = (int)round(x0);
    int y0_i = (int)round(y0);
    int x1_i = (int)round(x1);
    int y1_i = (int)round(y1);

    int dx = abs(x1_i - x0_i);
    int dy = abs(y1_i - y0_i);
    int sx = (x0_i < x1_i) ? 1 : -1;
    int sy = (y0_i < y1_i) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        if (x0_i >= 0 && x0_i < width && y0_i >= 0 && y0_i < height) {
            unsigned char *data = cairo_image_surface_get_data(surface);
            int stride = cairo_image_surface_get_stride(surface);
            unsigned char *pixel = data + (y0_i * stride) + (x0_i * 4);
            pixel[0] = b;
            pixel[1] = g;
            pixel[2] = r;
            pixel[3] = 255;
        }
        if (x0_i == x1_i && y0_i == y1_i) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0_i += sx; }
        if (e2 < dx) { err += dx; y0_i += sy; }
    }
}

double nachal_koord[4][3] = {{0, 190, 1}, {300, 275, 1}, {40, 10, 1}, {50, 400, 1}};
double krest[4][3];
double risunok[4][3];
int size[2] = {4, 3};

int main() {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "No display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root_window = RootWindow(display, screen);

    int window_width = 800;
    int window_height = 600;

    Window window = XCreateSimpleWindow(
        display,
        root_window,
        0, 0,
        window_width, window_height,
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

    const char *window_title = "titr";
    XStoreName(display, window, window_title);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XMapWindow(display, window);

    cairo_surface_t *xlib_surface = cairo_xlib_surface_create(display, window, DefaultVisual(display, screen), window_width, window_height);
    cairo_t *cr = cairo_create(xlib_surface);

    cairo_surface_t *image_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, window_width, window_height);
    cairo_t *image_cr = cairo_create(image_surface);

    copy_matrix(&krest[0][0], &nachal_koord[0][0], size);

    int needRedraw = 1;
    int running = 1;

    while (running) {
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
                case Expose:
                    needRedraw = 1;
                    break;

                case ConfigureNotify:
                    if (event.xconfigure.width != window_width || event.xconfigure.height != window_height) {
                        window_width = event.xconfigure.width;
                        window_height = event.xconfigure.height;

                        cairo_destroy(cr);
                        cairo_surface_destroy(xlib_surface);
                        cairo_destroy(image_cr);
                        cairo_surface_destroy(image_surface);

                        xlib_surface = cairo_xlib_surface_create(display, window, DefaultVisual(display, screen), window_width, window_height);
                        cr = cairo_create(xlib_surface);
                        image_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, window_width, window_height);
                        image_cr = cairo_create(image_surface);

                        needRedraw = 1;
                    }
                    break;

                case KeyPress: {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    double var[2] = {0, 0};
                    int chto_delaem = -1;

                    switch (keysym) {
                        case XK_w: case XK_W: var[0] = 0; var[1] = -5; chto_delaem = 0; break;
                        case XK_s: case XK_S: var[0] = 0; var[1] = 5; chto_delaem = 0; break;
                        case XK_a: case XK_A: var[0] = -5; var[1] = 0; chto_delaem = 0; break;
                        case XK_d: case XK_D: var[0] = 5; var[1] = 0; chto_delaem = 0; break;
                        case XK_x: case XK_X: var[0] = 0.95; var[1] = 0.95; chto_delaem = 1; break;
                        case XK_z: case XK_Z: var[0] = 1.05; var[1] = 1.05; chto_delaem = 1; break;
                        case XK_q: case XK_Q: var[0] = -M_PI / 36; chto_delaem = 2; break;
                        case XK_e: case XK_E: var[0] = M_PI / 36; chto_delaem = 2; break;
                        case XK_r: case XK_R: copy_matrix(&krest[0][0], &nachal_koord[0][0], size); needRedraw = 1; break;
                        case XK_Escape: running = 0; break;
                        default: break;
                    }

                    if (chto_delaem != -1) {
                        deistvie(chto_delaem, size, &krest[0][0], var);
                        needRedraw = 1;
                    }
                    break;
                }

                case ClientMessage:
                    if (event.xclient.data.l[0] == wm_delete_window) {
                        running = 0;
                    }
                    break;
            }
        }

        if (needRedraw) {
            cairo_destroy(image_cr);
            image_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, window_width, window_height);
            image_cr = cairo_create(image_surface);
            cairo_set_source_rgb(image_cr, 1.0, 1.0, 1.0);
            cairo_paint(image_cr);

            copy_matrix(&risunok[0][0], &krest[0][0], size);
            round_mat(&risunok[0][0], size);

            cairo_set_source_rgb(image_cr, 1.0, 0.0, 0.0);
            cairo_set_line_width(image_cr, 2.0);
            cairo_move_to(image_cr, risunok[0][0], risunok[0][1]);
            cairo_line_to(image_cr, risunok[1][0], risunok[1][1]);
            cairo_stroke(image_cr);

            bresenhamLine(image_surface, risunok[2][0], risunok[2][1], risunok[3][0], risunok[3][1], 255, 0, 0);

            cairo_destroy(cr);
            cr = cairo_create(xlib_surface);
            cairo_set_source_surface(cr, image_surface, 0, 0);
            cairo_paint(cr);

            cairo_surface_flush(xlib_surface);
            XFlush(display);
            needRedraw = 0;
        }

        usleep(1000);
    }

    cairo_destroy(cr);
    cairo_surface_destroy(xlib_surface);
    cairo_destroy(image_cr);
    cairo_surface_destroy(image_surface);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
