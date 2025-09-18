#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Структура для хранения строки и количества слов в ней
typedef struct {
    char *text;
    int word_count;
} Line;

// Функция подсчёта слов в строке
int count_words(const char *str) {
    int count = 0;
    int in_word = 0; // флаг: находимся внутри слова

    while (*str) {
        if (isspace((unsigned char)*str)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

// Функция сравнения для qsort: сортировка по количеству слов
int compare_lines(const void *a, const void *b) {
    Line *lineA = (Line *)a;
    Line *lineB = (Line *)b;
    return lineA->word_count - lineB->word_count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <входной_файл> <выходной_файл>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Ошибка открытия входного файла");
        return 1;
    }

    // Динамический массив строк
    Line *lines = NULL;
    size_t capacity = 10; // начальная ёмкость
    size_t count = 0;

    lines = malloc(capacity * sizeof(Line));
    if (!lines) {
        perror("Ошибка выделения памяти");
        fclose(in);
        return 1;
    }

    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t len;

    // Чтение всех строк
    while ((len = getline(&buffer, &bufsize, in)) != -1) {
        // Увеличиваем массив при необходимости
        if (count >= capacity) {
            capacity *= 2;
            Line *new_lines = realloc(lines, capacity * sizeof(Line));
            if (!new_lines) {
                perror("Ошибка перевыделения памяти");
                free(buffer);
                for (size_t i = 0; i < count; i++) {
                    free(lines[i].text);
                }
                free(lines);
                fclose(in);
                return 1;
            }
            lines = new_lines;
        }

        // Копируем строку (включая \n)
        lines[count].text = malloc(len + 1);
        if (!lines[count].text) {
            perror("Ошибка выделения памяти под строку");
            free(buffer);
            for (size_t i = 0; i < count; i++) {
                free(lines[i].text);
            }
            free(lines);
            fclose(in);
            return 1;
        }
        memcpy(lines[count].text, buffer, len + 1);

        // Подсчитываем количество слов
        lines[count].word_count = count_words(lines[count].text);
        count++;
    }

    free(buffer);
    fclose(in);

    // Сортируем строки по количеству слов
    qsort(lines, count, sizeof(Line), compare_lines);

    // Записываем результат в выходной файл
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        perror("Ошибка открытия выходного файла");
        for (size_t i = 0; i < count; i++) {
            free(lines[i].text);
        }
        free(lines);
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        fputs(lines[i].text, out);
        free(lines[i].text); // освобождаем память каждой строки
    }

    free(lines); // освобождаем массив структур
    fclose(out);

    printf("Файл '%s' успешно отсортирован по числу слов и сохранён в '%s'.\n", argv[1], argv[2]);
    return 0;
}
