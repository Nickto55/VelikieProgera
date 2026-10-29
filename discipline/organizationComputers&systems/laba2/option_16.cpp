#include <conio.h>
#include <dos.h>
#include <stdio.h>

#define X1 25
#define Y1 12
#define X2 55
#define Y2 23

#define T 600
#define S 1

char *bg_eng[8] = {
    "BLACK", "BLUE", "GREEN", "CYAN",
    "RED", "MAGENTA", "BROWN", "LIGHTGRAY"};

char *fg_rus[16] = {
    "ЧЕРНЫЙ", "СИНИЙ", "ЗЕЛЕНЫЙ", "ГОЛУБОЙ",
    "КРАСНЫЙ", "ФИОЛЕТОВЫЙ", "КОРИЧНЕВЫЙ", "СВЕТЛО-СЕРЫЙ",
    "ТЕМНО-СЕРЫЙ", "ЯРКО-СИНИЙ", "ЯРКО-ЗЕЛЕНЫЙ", "ЯРКО-ГОЛУБОЙ",
    "ЯРКО-КРАСНЫЙ", "ЯРКО-ФИОЛЕТОВЫЙ", "ЖЕЛТЫЙ", "БЕЛЫЙ"};

void scroll_down(int x1, int y1, int x2, int y2, int lines, int attr)
{
    union REGS r;
    r.h.ah = 0x07;
    r.h.al = (char)lines;
    r.h.bh = (char)attr;
    r.h.ch = (char)(y1 - 1);
    r.h.cl = (char)(x1 - 1);
    r.h.dh = (char)(y2 - 1);
    r.h.dl = (char)(x2 - 1);
    int86(0x10, &r, &r);
}

int main(void)
{
    int bg, fg, i;

    textattr(0x07);
    window(X1, Y1, X2, Y2);
    clrscr();

    for (bg = 0; bg <= 7; bg++)
        for (fg = 0; fg <= 15; fg++)
        {
            delay(T);
            scroll_down(X1, Y1, X2, Y2, S, (bg << 4) | LIGHTGRAY);
            textbackground(bg);
            textcolor(fg);
            gotoxy(1, 1);
            clreol();
            cprintf("Background: %s", bg_eng[bg]);
            gotoxy(1, 2);
            clreol();
            cprintf("Символ: %s", fg_rus[fg]);
        }

    for (i = 0;; i++)
    {
        if (kbhit())
            break;

        bg = (i / 16) % 8;
        fg = i % 16;
        delay(T);
        scroll_down(X1, Y1, X2, Y2, S, (bg << 4) | LIGHTGRAY);
        textbackground(bg);
        textcolor(fg);
        gotoxy(1, 1);
        clreol();
        cprintf("Сообщение N%d", i);
        gotoxy(1, 2);
        clreol();
        cprintf("%s / %s", bg_eng[bg], fg_rus[fg]);
    }

    getch();
    textattr(0x07);
    window(1, 1, 80, 25);
    clrscr();
    return 0;
}
