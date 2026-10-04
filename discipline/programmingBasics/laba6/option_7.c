#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINE 1024

int is_excluded_word(const char* word, const char** exclusions, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word, exclusions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char* remove_words(const char* str, int num_exclusions, ...) {
    if (!str) return NULL;

    char* work_str = strdup(str);
    if (!work_str) return NULL;

    const char** exclusions = malloc(num_exclusions * sizeof(char*));
    if (!exclusions) {
        free(work_str);
        return NULL;
    }

    va_list args;
    va_start(args, num_exclusions);
    for (int i = 0; i < num_exclusions; i++) {
        exclusions[i] = va_arg(args, const char*);
    }
    va_end(args);

    char* result = malloc(strlen(work_str) + 1);
    if (!result) {
        free(work_str);
        free((void*)exclusions);
        return NULL;
    }

    result[0] = '\0';
    char* token = strtok(work_str, " \t\n\r");
    int first = 1;

    while (token != NULL) {
        if (!is_excluded_word(token, exclusions, num_exclusions)) {
            if (!first) strcat(result, " ");
            strcat(result, token);
            first = 0;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(work_str);
    free((void*)exclusions);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s слово1 [слово2 ...]\n", argv[0]);
        return 1;
    }

    int num_exclusions = argc - 1;
    
    char line[MAX_LINE];
    printf("Введите строку: ");
    if (!fgets(line, sizeof(line), stdin)) return 1;
    
    char* nl = strchr(line, '\n');
    if (nl) *nl = '\0';

    // Вызов функции с аргументами командной строки как исключениями
    char* result = remove_words(line, num_exclusions, 
                                argv[1], argv[2], argv[3], argv[4], 
                                argv[5], argv[6], argv[7], argv[8],
                                argv[9], argv[10]); // Поддержка до 10 аргументов
    
    if (result) {
        printf("Результат: %s\n", result);
        free(result);
    }
    return 0;
}
