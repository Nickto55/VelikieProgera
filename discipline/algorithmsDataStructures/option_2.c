#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    int in_string = 0;
    int in_char = 0;
    int in_comment = 0;
    int in_line_comment = 0;
    int prev_char = 0;
    int current_char;

    printf("Содержимое файла %s:\n\n", filename);

    while ((current_char = fgetc(file)) != EOF) {

        // Обновление состояний строк и символов
        if (!in_comment && !in_line_comment) {
            if (current_char == '"' && prev_char != '\\') {
                in_string = !in_string;
            }
            if (current_char == '\'' && prev_char != '\\') {
                in_char = !in_char;
            }
        }

        // Обновление состояний комментариев
        if (!in_string && !in_char) {
            if (current_char == '/' && prev_char == '/') {
                in_line_comment = 1;
            }
            if (current_char == '*' && prev_char == '/') {
                in_comment = 1;
            }
            if (current_char == '/' && prev_char == '*') {
                in_comment = 0;
            }
            if (current_char == '\n') {
                in_line_comment = 0;
            }
        }

        // Подсветка целых десятичных констант
        if (!in_string && !in_char && !in_comment && !in_line_comment) {
            if (isdigit(current_char)) {
                // Проверка: не является ли частью идентификатора
                if (prev_char != '_' && !isalpha(prev_char)) {

                    // Проверка на 0
                    if (current_char == '0') {
                        int next_char = fgetc(file);
                        if (next_char == 'x' || next_char == 'X') {
                            // шестнадцатеричное — не подсвечиваем
                            putchar('0');
                            putchar(next_char);
                            while ((next_char = fgetc(file)) != EOF) {
                                if (isxdigit(next_char) || next_char == '_') {
                                    putchar(next_char);
                                } else {
                                    ungetc(next_char, file);
                                    break;
                                }
                            }
                            prev_char = next_char;
                            continue;
                        } else if (next_char >= '0' && next_char <= '7') {
                            // восьмеричное — не подсвечиваем
                            putchar('0');
                            putchar(next_char);
                            while ((next_char = fgetc(file)) != EOF) {
                                if (next_char >= '0' && next_char <= '7') {
                                    putchar(next_char);
                                } else {
                                    ungetc(next_char, file);
                                    break;
                                }
                            }
                            prev_char = next_char;
                            continue;
                        } else {
                            // это просто 0 — десятичное
                            ungetc(next_char, file);
                        }
                    }

                    // Подсветка десятичного числа
                    printf(GREEN);
                    putchar(current_char);

                    // Читаем цифры
                    while (isdigit(current_char = fgetc(file))) {
                        putchar(current_char);
                    }

                    // Читаем суффиксы: u, l, U, L
                    while (current_char == 'u' || current_char == 'U' ||
                           current_char == 'l' || current_char == 'L') {
                        putchar(current_char);
                        current_char = fgetc(file);
                    }

                    printf(RESET);
                    ungetc(current_char, file); // вернуть лишний символ
                    prev_char = '0'; // чтобы не сломать контекст
                    continue;
                }
            }
        }

        putchar(current_char);
        prev_char = current_char;
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
