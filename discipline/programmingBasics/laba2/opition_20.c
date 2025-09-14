#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

int main() {
    FILE *input;
    SetConsoleOutputCP(CP_UTF8);

    char words[MAX_WORDS][MAX_WORD_LENGTH];
    char word[MAX_WORD_LENGTH];
    int word_count = 0;
    int i, j, k;
    char ch;
    int found;

    input = fopen("input_txt.txt", "r");
    if (input == NULL) {
        printf("Ошибка открытия файла!\n");
        return 1;
    }

    while ((ch = fgetc(input)) != EOF) {
        if (isalpha(ch)) {
            i = 0;
            while (isalpha(ch) && ch != EOF) {
                word[i] = tolower(ch);
                i++;
                ch = fgetc(input);
            }
            word[i] = '\0';

            found = 0;
            for (j = 0; j < word_count; j++) {
                if (strcmp(words[j], word) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found && word_count < MAX_WORDS) {
                strcpy(words[word_count], word);
                word_count++;
            }
        }
    }

    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                strcpy(word, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], word);
            }
        }
    }

    fclose(input);

    printf("Словарь в алфавитном порядке:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }

    return 0;
}
/*
 * #include <stdio.h>      // Подключаем библиотеку для работы с вводом/выводом
#include <string.h>     // Подключаем библиотеку для работы со строками
#include <ctype.h>      // Подключаем библиотеку для работы с символами

#define MAX_WORDS 1000          // Максимальное количество слов в словаре
#define MAX_WORD_LENGTH 100     // Максимальная длина одного слова

int main() {                    // Главная функция программы
    FILE *input;                // Указатель на входной файл
    char words[MAX_WORDS][MAX_WORD_LENGTH];  // Массив для хранения уникальных слов
    char word[MAX_WORD_LENGTH]; // Массив для временного хранения текущего слова
    int word_count = 0;         // Счетчик количества уникальных слов
    int i, j, k;                // Переменные для циклов
    char ch;                    // Переменная для хранения текущего символа
    int found;                  // Флаг для проверки наличия слова в словаре

    input = fopen("input.txt", "r");        // Открываем входной файл для чтения
    if (input == NULL) {                    // Проверяем, удалось ли открыть файл
        printf("Ошибка открытия файла!\n"); // Сообщаем об ошибке
        return 1;                           // Завершаем программу с кодом ошибки
    }

    while ((ch = fgetc(input)) != EOF) {    // Читаем файл посимвольно до конца
        if (isalpha(ch)) {                  // Если символ является буквой
            i = 0;                          // Обнуляем индекс для слова
            while (isalpha(ch) && ch != EOF) {  // Пока читаем буквы
                word[i] = tolower(ch);      // Преобразуем букву в нижний регистр и сохраняем
                i++;                        // Переходим к следующей позиции
                ch = fgetc(input);          // Читаем следующий символ
            }
            word[i] = '\0';                 // Добавляем завершающий ноль

            found = 0;                      // Сбрасываем флаг нахождения слова
            for (j = 0; j < word_count; j++) {  // Проходим по всем уже найденным словам
                if (strcmp(words[j], word) == 0) {  // Если слово уже есть в словаре
                    found = 1;              // Устанавливаем флаг
                    break;                  // Прерываем цикл
                }
            }

            if (!found && word_count < MAX_WORDS) {  // Если слово новое и есть место
                strcpy(words[word_count], word);     // Добавляем слово в словарь
                word_count++;                        // Увеличиваем счетчик слов
            }
        }
    }

    // Сортировка слов в алфавитном порядке (пузырьковая сортировка)
    for (i = 0; i < word_count - 1; i++) {       // Внешний цикл сортировки
        for (j = i + 1; j < word_count; j++) {   // Внутренний цикл сортировки
            if (strcmp(words[i], words[j]) > 0) {  // Если слова нужно поменять местами
                strcpy(word, words[i]);          // Меняем местами слова
                strcpy(words[i], words[j]);
                strcpy(words[j], word);
            }
        }
    }

    fclose(input);                           // Закрываем входной файл

    printf("Словарь в алфавитном порядке:\n");  // Выводим заголовок
    for (i = 0; i < word_count; i++) {       // Проходим по всем словам в словаре
        printf("%s\n", words[i]);            // Выводим каждое слово
    }

    return 0;                                // Завершаем программу успешно
}
 */