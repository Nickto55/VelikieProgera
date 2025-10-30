#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000

int main() {
    FILE *input, *output;


    char line[MAX_LINE_LENGTH];
    char result[MAX_LINE_LENGTH];
    int i, j;
    int in_single_comment = 0;
    int in_multi_comment = 0;
    int in_string = 0;
    int in_char = 0;

    input = fopen("input_txt.txt", "r");
    output = fopen("output.txt", "w");

    if (input == NULL || output == NULL) {
        printf("Ошибка открытия файлов!\n");
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) {
        in_single_comment = 0;
        memset(result, 0, MAX_LINE_LENGTH);

        for (i = 0, j = 0; line[i] != '\0'; i++) {
            if (!in_single_comment && !in_multi_comment) {
                if (!in_string && !in_char && line[i] == '"' &&
                    (i == 0 || line[i-1] != '\\')) {
                    in_string = 1;
                }
                else if (!in_string && !in_char && line[i] == '\'' &&
                         (i == 0 || line[i-1] != '\\')) {
                    in_char = 1;
                }
                else if (!in_string && !in_char && line[i] == '/' && line[i+1] == '/') {
                    in_single_comment = 1;
                    i++;
                    continue;
                }
                else if (!in_string && !in_char && line[i] == '/' && line[i+1] == '*') {
                    in_multi_comment = 1;
                    i++;
                    continue;
                }
                else if (in_string && line[i] == '"' && line[i-1] != '\\') {
                    in_string = 0;
                }
                else if (in_char && line[i] == '\'' && line[i-1] != '\\') {
                    in_char = 0;
                }
            }
            else if (in_multi_comment && line[i] == '*' && line[i+1] == '/') {
                in_multi_comment = 0;
                i++;
                continue;
            }

            if (!in_single_comment && !in_multi_comment) {
                result[j] = line[i];
                j++;
            }
        }

        if (!in_single_comment && !in_multi_comment) {
            result[j] = '\0';
            fputs(result, output);
        } else {
            result[j] = '\0';
            if (strlen(result) > 0) {
                fputs(result, output);
                fputs("\n", output);
            }
        }
    }

    fclose(input);
    fclose(output);

    printf("Обработка завершена!\n");
    return 0;
}

/*
 * #include <stdio.h>      // Подключаем библиотеку для работы с вводом/выводом
#include <string.h>     // Подключаем библиотеку для работы со строками

#define MAX_LINE_LENGTH 1000    // Максимальная длина строки

int main() {                    // Главная функция программы
    FILE *input, *output;       // Указатели на входной и выходной файлы
    char line[MAX_LINE_LENGTH]; // Массив для хранения текущей строки
    char result[MAX_LINE_LENGTH]; // Массив для хранения результата
    int i, j;                   // Переменные для циклов
    int in_single_comment = 0;  // Флаг однострочного комментария //
    int in_multi_comment = 0;   // Флаг многострочного комментария /*
    int in_string = 0;          // Флаг нахождения в строке ""
    int in_char = 0;            // Флаг нахождения в символе ''

    input = fopen("input.txt", "r");        // Открываем входной файл для чтения
    output = fopen("output.txt", "w");      // Открываем выходной файл для записи

    if (input == NULL || output == NULL) {  // Проверяем открытие файлов
        printf("Ошибка открытия файлов!\n"); // Сообщаем об ошибке
        return 1;                           // Завершаем программу с ошибкой
    }

    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) {  // Читаем файл построчно
        in_single_comment = 0;              // Сбрасываем флаг однострочного комментария
        memset(result, 0, MAX_LINE_LENGTH); // Очищаем массив результата

        // Обрабатываем каждый символ в строке
        for (i = 0, j = 0; line[i] != '\0'; i++) {  // Проходим по строке
            if (!in_single_comment && !in_multi_comment) {  // Если не в комментарии
                // Проверяем начало строки
                if (!in_string && !in_char && line[i] == '"' &&
                    (i == 0 || line[i-1] != '\\')) {  // Начало строки "
                    in_string = 1;                  // Устанавливаем флаг строки
                }
                // Проверяем начало символа
                else if (!in_string && !in_char && line[i] == '\'' &&
                         (i == 0 || line[i-1] != '\\')) {  // Начало символа '
                    in_char = 1;                  // Устанавливаем флаг символа
                }
                // Проверяем начало однострочного комментария
                else if (!in_string && !in_char && line[i] == '/' && line[i+1] == '/') {
                    in_single_comment = 1;        // Устанавливаем флаг комментария
                    i++;                          // Пропускаем второй символ /
                    continue;                     // Переходим к следующей итерации
                }
                // Проверяем начало многострочного комментария
                else if (!in_string && !in_char && line[i] == '/' && line[i+1] == '*') {
                    in_multi_comment = 1;         // Устанавливаем флаг комментария
                    i++;                          // Пропускаем второй символ *
                    continue;                     // Переходим к следующей итерации
                }
                // Проверяем конец строки
                else if (in_string && line[i] == '"' && line[i-1] != '\\') {
                    in_string = 0;                // Сбрасываем флаг строки
                }
                // Проверяем конец символа
                else if (in_char && line[i] == '\'' && line[i-1] != '\\') {
                    in_char = 0;                  // Сбрасываем флаг символа
                }
            }
            // Проверяем конец многострочного комментария
            else if (in_multi_comment && line[i] == '*' && line[i+1] == '/') {
                in_multi_comment = 0;             // Сбрасываем флаг комментария
                i++;                              // Пропускаем второй символ /
                continue;                         // Переходим к следующей итерации
            }

            // Если не в комментарии, копируем символ в результат
            if (!in_single_comment && !in_multi_comment) {
                result[j] = line[i];              // Копируем символ
                j++;                              // Переходим к следующей позиции
            }
        }

        // Завершаем обработку строки
        if (!in_single_comment && !in_multi_comment) {  // Если не в комментарии
            result[j] = '\0';                   // Добавляем завершающий ноль
            fputs(result, output);              // Записываем результат в файл
        } else {
            result[j] = '\0';                   // Добавляем завершающий ноль
            if (strlen(result) > 0) {           // Если есть содержимое
                fputs(result, output);          // Записываем результат
                fputs("\n", output);            // Добавляем перевод строки
            }
        }
    }

    fclose(input);                              // Закрываем входной файл
    fclose(output);                             // Закрываем выходной файл

    printf("Обработка завершена!\n");           // Сообщаем об успешном завершении
    return 0;                                   // Завершаем программу успешно
}
 */

