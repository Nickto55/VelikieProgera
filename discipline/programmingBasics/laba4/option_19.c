#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

int is_preprocessor_directive(const char *line) {
    const char *p = line;
    while (*p == ' ' || *p == '\t') p++;
    return (*p == '#');
}

void print_with_highlight(const char *line) {
    if (is_preprocessor_directive(line)) {
        printf("\033[3;31m%s\033[0m", line); // Красный цвет для директив
    } else {
        printf("%s", line);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла.c>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return 1;
    }

    char line[MAX_LINE];
    int line_num = 1;

    while (fgets(line, sizeof(line), file)) {
        printf("%3d: ", line_num++);
        print_with_highlight(line);
    }

    fclose(file);
    return 0;
}
