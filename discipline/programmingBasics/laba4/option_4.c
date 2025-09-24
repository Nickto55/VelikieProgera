#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define RED "\033[1;31m"
#define RESET "\033[0m"

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[4096];
    int in_multiline_comment = 0;  // флаг для /* ... */

    printf("Содержимое файла %s:\n\n", filename);

    while (fgets(line, sizeof(line), file)) {
        char *ptr = line;
        int is_preprocessor = 0;

        // Если мы внутри многострочного комментария, ищем его конец
        if (in_multiline_comment) {
            char *end_comment = strstr(ptr, "*/");
            if (end_comment) {
                in_multiline_comment = 0;
                ptr = end_comment + 2;
            } else {
                // Вся строка — внутри комментария
                printf("%s", line);
                continue;
            }
        }

        // Пропускаем пробелы и табуляции в начале строки
        while (*ptr && (*ptr == ' ' || *ptr == '\t')) {
            ptr++;
        }

        // Проверяем, начинается ли оставшаяся часть с #
        if (*ptr == '#') {
            // Убедимся, что # не внутри комментария в этой же строке
            // Но так как мы уже вышли из многострочного комментария,
            // и # в начале — это директива
            is_preprocessor = 1;
        }

        // Также проверим, нет ли /* в этой строке до #
        // (маловероятно, но для надёжности)
        // На практике: если # в начале — это директива, даже если позже будет /*
        // Так что эту проверку можно опустить.

        // Проверка на начало многострочного комментария в этой строке
        // (для следующих строк)
        char *start_comment = strstr(line, "/*");
        char *end_comment = strstr(line, "*/");

        // Обновляем флаг многострочного комментария
        if (start_comment && (!end_comment || start_comment < end_comment)) {
            in_multiline_comment = 1;
        }

        // Вывод строки с выделением
        if (is_preprocessor) {
            printf(RED "%s" RESET, line);
        } else {
            printf("%s", line);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <filename.c>\n", argv[0]);
        return 1;
    }
    process_file(argv[1]);
    return 0;
}
