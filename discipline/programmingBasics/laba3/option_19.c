/*Скопировать текстовый файл, отсортировав строки по числу слов в строке.*/

/*
cd ~/vs_code_projects/VelikieProgera/discipline/programmingBasics/laba3 && gcc option_19.c -o option_19  && ./option_19 input.txt && nano input.txt && nano output.txt 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *line;
    int word_count;
} LineInfo;

int count_words(const char *str) {
    int count = 0;
    int in_WORD = 0;
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

int compare_lines(const void *a, const void *b) {
    LineInfo *la = (LineInfo *)a;
    LineInfo *lb = (LineInfo *)b;
    return la->word_count - lb->word_count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Не удалось открыть input файл");
        return 1;
    }

    int line_count = 0;
    char buffer[10000];
    while (fgets(buffer, sizeof(buffer), input)) {
        line_count++;
    }
    rewind(input);

    LineInfo *lines = malloc(line_count * sizeof(LineInfo));
    if (!lines) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        fclose(input);
        return 1;
    }

    for (int i = 0; i < line_count; i++) {
        if (!fgets(buffer, sizeof(buffer), input)) break;

        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        lines[i].line = malloc(strlen(buffer) + 1);
        if (!lines[i].line) {
            fprintf(stderr, "Ошибка выделения памяти для строки.\n");
            fclose(input);
            for (int j = 0; j < i; j++) free(lines[j].line);
            free(lines);
            return 1;
        }
        strcpy(lines[i].line, buffer);
        lines[i].word_count = count_words(lines[i].line);
    }
    fclose(input);

    qsort(lines, line_count, sizeof(LineInfo), compare_lines);

    FILE *output = fopen("output.txt", "w");
    if (!output) {
        perror("Не удалось создать output.txt");
        for (int i = 0; i < line_count; i++) free(lines[i].line);
        free(lines);
        return 1;
    }

    for (int i = 0; i < line_count; i++) {
        fprintf(output, "%s\n", lines[i].line);
        free(lines[i].line);
    }
    fclose(output);
    free(lines);

    printf("Файл успешно отсортирован и сохранён в output.txt\n");
    return 0;	
}
