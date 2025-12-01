#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define M_PI 3.14159265358979323846

struct Vec3 {
    double x, y, w;
    Vec3(double x = 0, double y = 0, double w = 1) : x(x), y(y), w(w) {}
};

void mult_matrix(double* result, const double* a, const double* b, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; ++i) {
        for (int j = 0; j < cols_b; ++j) {
            result[i * cols_b + j] = 0;
            for (int k = 0; k < cols_a; ++k) {
                result[i * cols_b + j] += a[i * cols_a + k] * b[k * cols_b + j];
            }
        }
    }
}

void copy_data(double* dest, const double* src, int n) {
    std::memcpy(dest, src, n * sizeof(double));
}

void round_elements(double* arr, int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = std::round(arr[i]);
    }
}

void normalize_coords(double* points, int num_points) {
    for (int i = 0; i < num_points; ++i) {
        double w = points[i * 3 + 2];
        if (w != 0 && w != 1) {
            points[i * 3] /= w;
            points[i * 3 + 1] /= w;
            points[i * 3 + 2] = 1.0;
        }
    }
}

void draw_line_gl(double x0, double y0, double x1, double y1, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void calculate_centroid(double& cx, double& cy, const double* points, int num_points) {
    cx = 0; cy = 0;
    for (int i = 0; i < num_points; ++i) {
        cx += points[i * 3];
        cy += points[i * 3 + 1];
    }
    cx /= num_points;
    cy /= num_points;
}

void apply_transform(int type, double* points, int num_points, double* params) {
    if (type == -1) return;

    double temp[9];
    double matrix[9];
    double center_x, center_y;
    calculate_centroid(center_x, center_y, points, num_points);

    // Translate to origin
    double trans_to_origin[9] = {1,0,0, 0,1,0, -center_x,-center_y,1};
    mult_matrix(temp, points, trans_to_origin, num_points, 3, 3);
    copy_data(points, temp, num_points * 3);

    // Apply transformation
    if (type == 0) { // Translation
        matrix[0] = 1; matrix[1] = 0; matrix[2] = 0;
        matrix[3] = 0; matrix[4] = 1; matrix[5] = 0;
        matrix[6] = params[0]; matrix[7] = params[1]; matrix[8] = 1;
    } else if (type == 1) { // Scale
        matrix[0] = params[0]; matrix[1] = 0; matrix[2] = 0;
        matrix[3] = 0; matrix[4] = params[1]; matrix[5] = 0;
        matrix[6] = 0; matrix[7] = 0; matrix[8] = 1;
    } else if (type == 2) { // Rotate
        double c = std::cos(params[0]);
        double s = std::sin(params[0]);
        matrix[0] = c; matrix[1] = s; matrix[2] = 0;
        matrix[3] = -s; matrix[4] = c; matrix[5] = 0;
        matrix[6] = 0; matrix[7] = 0; matrix[8] = 1;
    }

    mult_matrix(temp, points, matrix, num_points, 3, 3);
    copy_data(points, temp, num_points * 3);

    // Translate back
    double trans_back[9] = {1,0,0, 0,1,0, center_x,center_y,1};
    mult_matrix(temp, points, trans_back, num_points, 3, 3);
    copy_data(points, temp, num_points * 3);

    normalize_coords(points, num_points);
}

double initial_coords[9] = {
    100, 100, 1,
    300, 150, 1,
    200, 300, 1
};
double current_coords[9];
double render_coords[9];

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = DefaultRootWindow(display);
    int depth = CopyFromParent;
    XVisualInfo* visual = glXChooseVisual(display, screen, (int[]){GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None});
    Colormap colormap = XCreateColormap(display, root, visual->visual, AllocNone);
    XSetWindowAttributes attrs;
    attrs.colormap = colormap;
    attrs.border_pixel = 0;
    attrs.event_mask = KeyPressMask | ExposureMask;

    Window win = XCreateWindow(display, root, 0, 0, 800, 600, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWBorderPixel | CWEventMask, &attrs);
    XStoreName(display, win, "Triangle Transform");
    XMapWindow(display, win);

    GLXContext context = glXCreateContext(display, visual, NULL, GL_TRUE);
    glXMakeCurrent(display, win, context);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    copy_data(current_coords, initial_coords, 9);

    bool should_redraw = true;
    bool running = true;
    while (running) {
        if (should_redraw) {
            glClear(GL_COLOR_BUFFER_BIT);
            copy_data(render_coords, current_coords, 9);
            round_elements(render_coords, 9);

            draw_line_gl(render_coords[0], render_coords[1], render_coords[3], render_coords[4], 1.0f, 0.0f, 0.0f);
            draw_line_gl(render_coords[3], render_coords[4], render_coords[6], render_coords[7], 0.0f, 1.0f, 0.0f);
            draw_line_gl(render_coords[6], render_coords[7], render_coords[0], render_coords[1], 0.0f, 0.0f, 1.0f);

            glXSwapBuffers(display, win);
            should_redraw = false;
        }

        if (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);
            if (event.type == KeyPress) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                double vars[2];
                int action = -1;

                switch (key) {
                    case 'w': case 'W': vars[0] = 0; vars[1] = -5; action = 0; break;
                    case 's': case 'S': vars[0] = 0; vars[1] = 5; action = 0; break;
                    case 'a': case 'A': vars[0] = -5; vars[1] = 0; action = 0; break;
                    case 'd': case 'D': vars[0] = 5; vars[1] = 0; action = 0; break;
                    case 'x': case 'X': vars[0] = 0.95; vars[1] = 0.95; action = 1; break;
                    case 'z': case 'Z': vars[0] = 1.05; vars[1] = 1.05; action = 1; break;
                    case 'q': case 'Q': vars[0] = -M_PI / 36; action = 2; break;
                    case 'e': case 'E': vars[0] = M_PI / 36; action = 2; break;
                    case 'r': case 'R': copy_data(current_coords, initial_coords, 9); should_redraw = true; break;
                    case 65307: running = false; break;
                }

                if (action != -1) {
                    apply_transform(action, current_coords, 3, vars);
                    should_redraw = true;
                }
            } else if (event.type == Expose) {
                should_redraw = true;
            }
        }
    }

    glXDestroyContext(display, context);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}
