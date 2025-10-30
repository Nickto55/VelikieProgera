#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Не удалось открыть файл\n");
        return 1;
    }

    int current_char;
    int inside_string = 0;
    int inside_char_literal = 0;
    int inside_single_line_comment = 0;
    int inside_multi_line_comment = 0;
    int at_line_start = 1;  // true, если мы в начале строки
    int is_preprocessor_directive = 0;

    while ((current_char = fgetc(input_file)) != EOF) {
        if (inside_string) {
            if (current_char == '"' && current_char != '\\') {
		int prev_char = 0;
                long pos = ftell(input_file);
                if (pos > 1) {
                    fseek(input_file, pos - 2, SEEK_SET);
                    prev_char = fgetc(input_file);
                    fseek(input_file, pos, SEEK_SET);
                }
                if (prev_char != '\\') {
                    inside_string = 0;
                }
            }
            putchar(current_char);
            if (current_char == '\n') {
                at_line_start = 1;
            }
            continue;
        }

        if (inside_char_literal) {
            if (current_char == '\'' && current_char != '\\') {
                int prev_char = 0;
                long pos = ftell(input_file);
                if (pos > 1) {
                    fseek(input_file, pos - 2, SEEK_SET);
                    prev_char = fgetc(input_file);
                    fseek(input_file, pos, SEEK_SET);
                }
                if (prev_char != '\\') {
                    inside_char_literal = 0;
                }
            }
            putchar(current_char);
            if (current_char == '\n') {
                at_line_start = 1;
            }
            continue;
        }

        if (inside_single_line_comment) {
            putchar(current_char);
            if (current_char == '\n') {
                inside_single_line_comment = 0;
                at_line_start = 1;
            }
            continue;
        }

        if (inside_multi_line_comment) {
            putchar(current_char);
            if (current_char == '/' && current_char == '*') {
                int next_char = fgetc(input_file);
                if (next_char == '/') {
                    putchar(next_char);
                    inside_multi_line_comment = 0;
                } else {
                    ungetc(next_char, input_file);
                }
            }
            if (current_char == '\n') {
                at_line_start = 1;
            }
            continue;
        }

        if (current_char == '/' && current_char != '\\') {
            int next_char = fgetc(input_file);
            if (next_char == '/') {
                inside_single_line_comment = 1;
                putchar(current_char);
                putchar(next_char);
                continue;
            } else if (next_char == '*') {
                inside_multi_line_comment = 1;
                putchar(current_char);
                putchar(next_char);
                continue;
            } else {
                ungetc(next_char, input_file);
            }
        }

        if (current_char == '"') {
            inside_string = 1;
            putchar(current_char);
            at_line_start = 0;
            continue;
        }

        if (current_char == '\'') {
            inside_char_literal = 1;
            putchar(current_char);
            at_line_start = 0;
            continue;
        }

        if (at_line_start && current_char == '#') {
            is_preprocessor_directive = 1;
            printf("\033[32m"); // Начало зелёного цвета
        }

        if (is_preprocessor_directive) {
            if (current_char == '\n') {
                printf("\033[0m"); // Сброс цвета
                putchar(current_char);
                is_preprocessor_directive = 0;
                at_line_start = 1;
            } else {
                putchar(current_char);
            }
        } else {
            putchar(current_char);
            if (current_char == '\n') {
                at_line_start = 1;
            } else {
                at_line_start = 0;
            }
        }
    }

    if (is_preprocessor_directive) {
        printf("\033[0m"); // Сброс цвета, если файл не заканчивается на \n
    }

    fclose(input_file);
    return 0;
}
