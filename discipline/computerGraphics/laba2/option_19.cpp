#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define PI 3.14159265359

void draw_line_bresenham(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        SDL_RenderDrawPoint(renderer, x0, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void translate_line(float *x0, float *y0, float *x1, float *y1, int dx, int dy) {
    *x0 += dx;
    *y0 += dy;
    *x1 += dx;
    *y1 += dy;
}

void rotate_line(float *x0, float *y0, float *x1, float *y1, float cx, float cy, float angle) {
    float rad = angle * PI / 180.0;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    
    float nx0 = cx + (*x0 - cx) * cos_a - (*y0 - cy) * sin_a;
    float ny0 = cy + (*x0 - cx) * sin_a + (*y0 - cy) * cos_a;
    float nx1 = cx + (*x1 - cx) * cos_a - (*y1 - cy) * sin_a;
    float ny1 = cy + (*x1 - cx) * sin_a + (*y1 - cy) * cos_a;
    
    *x0 = nx0;
    *y0 = ny0;
    *x1 = nx1;
    *y1 = ny1;
}

void scale_lines(float *x0, float *y0, float *x1, float *y1, float *x2, float *y2, float *x3, float *y3, float scale) {
    float cx = (*x0 + *x1 + *x2 + *x3) / 4.0;
    float cy = (*y0 + *y1 + *y2 + *y3) / 4.0;
    
    *x0 = cx + (*x0 - cx) * scale;
    *y0 = cy + (*y0 - cy) * scale;
    *x1 = cx + (*x1 - cx) * scale;
    *y1 = cy + (*y1 - cy) * scale;
    *x2 = cx + (*x2 - cx) * scale;
    *y2 = cy + (*y2 - cy) * scale;
    *x3 = cx + (*x3 - cx) * scale;
    *y3 = cy + (*y3 - cy) * scale;
}

int main() {
    srand(time(NULL));
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return 1;
    }
    
    TTF_Init();
    
    SDL_Window* window = SDL_CreateWindow("Graphical Editor",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("Window creation error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    float x0 = 415, y0 = 50;
    float x1 = 350, y1 = 240;
    float xb0 = 140, yb0 = 100;
    float xb1 = 550, yb1 = 100;
    
    SDL_Event e;
    int quit = 0;
    
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                    case SDLK_UP:
                        translate_line(&x0, &y0, &x1, &y1, 0, -5);
                        translate_line(&xb0, &yb0, &xb1, &yb1, 0, -5);
                        break;
                    case SDLK_DOWN:
                        translate_line(&x0, &y0, &x1, &y1, 0, 5);
                        translate_line(&xb0, &yb0, &xb1, &yb1, 0, 5);
                        break;
                    case SDLK_LEFT:
                        translate_line(&x0, &y0, &x1, &y1, -5, 0);
                        translate_line(&xb0, &yb0, &xb1, &yb1, -5, 0);
                        break;
                    case SDLK_RIGHT:
                        translate_line(&x0, &y0, &x1, &y1, 5, 0);
                        translate_line(&xb0, &yb0, &xb1, &yb1, 5, 0);
                        break;
                    case SDLK_q:
                        {
                            float cx = (xb0 + xb1 + x0 + x1) / 4.0;
                            float cy = (yb0 + yb1 + y0 + y1) / 4.0;
                            rotate_line(&x0, &y0, &x1, &y1, cx, cy, 5);
                            rotate_line(&xb0, &yb0, &xb1, &yb1, cx, cy, 5);
                        }
                        break;
                    case SDLK_e:
                        {
                            float cx = (xb0 + xb1 + x0 + x1) / 4.0;
                            float cy = (yb0 + yb1 + y0 + y1) / 4.0;
                            rotate_line(&x0, &y0, &x1, &y1, cx, cy, -5);
                            rotate_line(&xb0, &yb0, &xb1, &yb1, cx, cy, -5);
                        }
                        break;
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                        scale_lines(&x0, &y0, &x1, &y1, &xb0, &yb0, &xb1, &yb1, 1.1);
                        break;
                    case SDLK_MINUS:
                        scale_lines(&x0, &y0, &x1, &y1, &xb0, &yb0, &xb1, &yb1, 0.9);
                        break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        draw_line_bresenham(renderer, (int)x0, (int)y0, (int)x1, (int)y1, 0, 128, 0);
        draw_line_bresenham(renderer, (int)xb0, (int)yb0, (int)xb1, (int)yb1, 128, 0, 0);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
