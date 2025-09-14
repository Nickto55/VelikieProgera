#include <stdio.h>
#include <string.h>
#include <windows.h>

int main() {
    FILE *input, *output;

    SetConsoleOutputCP(CP_UTF8);

    char line[1000];
    char word[100];
    char longest[100];
    int i, j, k;
    int max_len, word_len, start_pos, end_pos;

    input = fopen("input_txt.txt", "r");
    output = fopen("output.txt", "w");

    if (input == NULL || output == NULL) {
        printf("Ошибка открытия файлов!\n");
        return 1;
    }

    while (fgets(line, 1000, input) != NULL) {
        max_len = 0;
        start_pos = 0;

        for (i = 0; line[i] != '\0'; i++) {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == ',' || line[i] == '.') {
                continue;
            }

            j = i;
            k = 0;

            while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' &&
                   line[j] != ',' && line[j] != '.') {
                word[k] = line[j];
                k++;
                j++;
                   }
            word[k] = '\0';

            word_len = strlen(word);
            if (word_len > max_len) {
                max_len = word_len;
                strcpy(longest, word);
                start_pos = i;
                end_pos = j;
            }

            i = j - 1;
        }

        if (max_len > 0) {
            for (i = 0, j = 0; line[i] != '\0'; i++) {
                if (i >= start_pos && i < end_pos) {
                    continue;
                }
                line[j] = line[i];
                j++;
            }
            line[j] = '\0';
        }

        fputs(line, output);
    }

    fclose(input);
    fclose(output);

    printf("Обработка завершена!\n");
    return 0;
}

// Пояснение:
/*
 * #include <stdio.h>      // Подключаем библиотеку для работы с вводом/выводом
#include <string.h>     // Подключаем библиотеку для работы со строками

int main() {            // Главная функция программы
    FILE *input, *output;           // Объявляем указатели на файлы
    char line[1000];                // Массив для хранения строки
    char word[100];                 // Массив для хранения слова
    char longest[100];              // Массив для хранения самого длинного слова
    int i, j, k;                    // Переменные для циклов
    int max_len, word_len, start_pos, end_pos;  // Переменные для длин и позиций

    input = fopen("input.txt", "r");        // Открываем входной файл для чтения
    output = fopen("output.txt", "w");      // Открываем выходной файл для записи

    if (input == NULL || output == NULL) {  // Проверяем, удалось ли открыть файлы
        printf("Ошибка открытия файлов!\n"); // Сообщаем об ошибке
        return 1;                           // Завершаем программу с кодом ошибки
    }

    while (fgets(line, 1000, input) != NULL) {  // Читаем файл построчно
        max_len = 0;                            // Обнуляем максимальную длину
        start_pos = 0;                          // Обнуляем начальную позицию

        for (i = 0; line[i] != '\0'; i++) {     // Проходим по всей строке
            if (line[i] == ' ' || line[i] == '\n' || line[i] == ',' || line[i] == '.') {
                continue;                       // Пропускаем разделители
            }

            j = i;                              // Запоминаем начало слова
            k = 0;                              // Обнуляем индекс для слова

            while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' &&
                   line[j] != ',' && line[j] != '.') {  // Пока не встретим разделитель
                word[k] = line[j];              // Копируем символ слова
                k++;                            // Увеличиваем индекс слова
                j++;                            // Переходим к следующему символу
            }
            word[k] = '\0';                     // Добавляем завершающий ноль

            word_len = strlen(word);            // Получаем длину текущего слова
            if (word_len > max_len) {           // Если слово длиннее максимального
                max_len = word_len;             // Обновляем максимальную длину
                strcpy(longest, word);          // Копируем слово в longest
                start_pos = i;                  // Запоминаем позицию начала
                end_pos = j;                    // Запоминаем позицию конца
            }

            i = j - 1;                          // Перемещаем индекс к концу слова
        }

        if (max_len > 0) {                      // Если было найдено слово
            for (i = 0, j = 0; line[i] != '\0'; i++) {  // Проходим по строке
                if (i >= start_pos && i < end_pos) {    // Если символы самого длинного слова
                    continue;                   // Пропускаем их
                }
                line[j] = line[i];              // Копируем символ
                j++;                            // Переходим к следующей позиции
            }
            line[j] = '\0';                     // Добавляем завершающий ноль
        }

        fputs(line, output);                    // Записываем строку в выходной файл
    }

    fclose(input);                              // Закрываем входной файл
    fclose(output);                             // Закрываем выходной файл

    printf("Обработка завершена!\n");           // Сообщаем об успешном завершении
    return 0;                                   // Завершаем программу успешно
}
 */