#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_VARS 1000
#define MAX_NAME_LEN 128
#define MAX_FILES 100

// Структура для хранения переменной и файлов
typedef struct {
    char name[MAX_NAME_LEN];
    char files[MAX_FILES][256];
    int file_count;
} VariableEntry;

VariableEntry table[MAX_VARS];
int var_count = 0;

// Проверка, является ли слово встроенным типом
int is_type_word(const char *word) {
    const char *types[] = {
        "int", "char", "float", "double", "long", "short",
        "unsigned", "signed", "void", "struct", "union", "enum"
    };
    int n = sizeof(types) / sizeof(types[0]);
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(word, types[i]) == 0)
            return 1;
    }
    return 0;
}

// Добавить переменную в таблицу
void add_variable(const char *var_name, const char *filename) {
    int i, j;
    for (i = 0; i < var_count; i++) {
        if (strcmp(table[i].name, var_name) == 0) {
            // Проверим, не добавлен ли уже этот файл
            for (j = 0; j < table[i].file_count; j++) {
                if (strcmp(table[i].files[j], filename) == 0)
                    return;
            }
            if (table[i].file_count < MAX_FILES) {
                strcpy(table[i].files[table[i].file_count], filename);
                table[i].file_count++;
            }
            return;
        }
    }

    // Создать новую запись
    if (var_count < MAX_VARS) {
        strcpy(table[var_count].name, var_name);
        strcpy(table[var_count].files[0], filename);
        table[var_count].file_count = 1;
        var_count++;
    }
}

// Удалить однострочные комментарии (//)
void remove_comments(char *line) {
    char *ptr = line;
    while (*ptr) {
        if (*ptr == '/' && *(ptr + 1) == '/') {
            *ptr = '\0';
            break;
        }
        ptr++;
    }
}

// Обработать один файл
void process_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Ошибка: не удалось открыть %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        // Удалим символы новой строки
        int len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[len-1] = '\0';
        if (len > 1 && line[len-2] == '\r')
            line[len-2] = '\0';

        remove_comments(line);

        // Пропустить пустые строки, препроцессор и макросы
        if (strlen(line) == 0 || line[0] == '#')
            continue;

        // Пропустить static
        if (strstr(line, "static") != NULL)
            continue;

        // Пропустить объявления функций (есть '(')
        if (strchr(line, '(') != NULL)
            continue;

        // Пропустить строки с телами блоков
        if (strchr(line, '{') || strchr(line, '}'))
            continue;

        // Разбор строки на токены
        char temp[1024];
        strcpy(temp, line);
        char *token = strtok(temp, " \t;=[]");
        char *prev = NULL;

        while (token != NULL) {
            // Пропустим extern, const, volatile
            if (strcmp(token, "extern") == 0 ||
                strcmp(token, "const") == 0 ||
                strcmp(token, "volatile") == 0) {
                token = strtok(NULL, " \t;=[]");
                continue;
            }

            if (is_type_word(token)) {
                char *next = strtok(NULL, " \t;=[]");
                if (next != NULL && strlen(next) > 0) {
                    // Проверим, что next — не ключевое слово и начинается с буквы
                    if (next[0] >= 'a' && next[0] <= 'z' ||
                        next[0] >= 'A' && next[0] <= 'Z' ||
                        next[0] == '_') {
                        if (!is_type_word(next)) {
                            add_variable(next, filename);
                        }
                    }
                }
                break; // обрабатываем только первую переменную в строке
            }

            token = strtok(NULL, " \t;=[]");
        }
    }

    fclose(fp);
}

// Вывод таблицы
void print_table() {
    if (var_count == 0) {
        printf("Внешние переменные не найдены.\n");
        return;
    }

    printf("\nТаблица внешних переменных:\n");
    printf("%-20s %s\n", "Имя переменной", "Файлы");
    printf("----------------------------------------\n");

    int i, j;
    for (i = 0; i < var_count; i++) {
        printf("%-20s ", table[i].name);
        for (j = 0; j < table[i].file_count; j++) {
            if (j > 0) printf(", ");
            printf("%s", table[i].files[j]);
        }
        printf("\n");
    }
}

// Главная функция
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s файл1.c [файл2.c ...]\n", argv[0]);
        return 1;
    }

    int i;
    for (i = 1; i < argc; i++) {
        process_file(argv[i]);
    }

    print_table();
    return 0;
}
