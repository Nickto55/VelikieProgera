#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAXLEN 1000

int main(void) {
    HANDLE hStdout;
    FILE *fp;

    SetConsoleOutputCP(CP_UTF8);


    char line[MAXLEN];
    char word[MAXLEN];
    int i;
    int flag;
    int prev_flag;
    char *ptr;
    WORD foregroundColor0;
    WORD foregroundColor1;
    WORD foregroundColor;
    WORD backgroundColor;
    WORD textAttribute;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    foregroundColor0 = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;
    foregroundColor1 = FOREGROUND_INTENSITY | FOREGROUND_RED;
    backgroundColor = BACKGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

    SetConsoleTextAttribute(hStdout, foregroundColor0 | backgroundColor);

    fp = fopen("option.c", "rt");
    if(fp == NULL)
        return 1;

    while(!feof(fp)) {
        ptr = fgets(line, MAXLEN, fp);
        if(ptr == NULL)
            break;

        i = 0;
        prev_flag = flag = 0;
        word[0] = '\0';

        while(*ptr != 0) {
            prev_flag = flag;

            if(*ptr == '#' && (i == 0 || word[i-1] == ' ' || word[i-1] == '\t' || word[i-1] == '\n'))
                flag = 1;
            else if(*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
                flag = 0;
            else
                flag = prev_flag;

            if(flag != prev_flag && i != 0) {
                word[i] = '\0';

                foregroundColor = (prev_flag == 1) ? foregroundColor1 : foregroundColor0;
                textAttribute = foregroundColor | backgroundColor;
                SetConsoleTextAttribute(hStdout, textAttribute);

                printf("%s", word);
                i = 0;
            }

            word[i++] = *ptr++;
        }

        if(i != 0) {
            word[i] = '\0';

            foregroundColor = (prev_flag == 1) ? foregroundColor1 : foregroundColor0;
            textAttribute = foregroundColor | backgroundColor;
            SetConsoleTextAttribute(hStdout, textAttribute);

            printf("%s", word);
        }
    }
    printf("\n");

    SetConsoleTextAttribute(hStdout, 7);

    fclose(fp);
    return 0;
}
/*
#include <stdio.h>      // Подключаем библиотеку для работы с вводом/выводом
#include <string.h>     // Подключаем библиотеку для работы со строками
#include <windows.h>    // Подключаем библиотеку для работы с консолью Windows
#define MAXLEN 1000     // Максимальная длина строки

int main(void) {        // Главная функция программы
    HANDLE hStdout;     // Дескриптор консольного окна
    FILE *fp;           // Указатель на входной файл
    char line[MAXLEN];  // Массив для хранения строки из файла
    char word[MAXLEN];  // Массив для хранения текущего слова
    int i;              // Индекс для массива word
    int flag;           // Флаг: 1 - директива препроцессора, 0 - обычный текст
    int prev_flag;      // Предыдущее значение флага
    char *ptr;          // Указатель на текущую позицию в строке
    WORD foregroundColor0;  // Цвет обычного текста (сине-зеленый)
    WORD foregroundColor1;  // Цвет директив препроцессора (красный)
    WORD foregroundColor;   // Текущий цвет символов
    WORD backgroundColor;   // Цвет фона
    WORD textAttribute;     // Атрибут текста (цвет символов и фона)

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);  // Получаем дескриптор консоли

    // Определяем цвета: обычный текст - сине-зеленый, директивы - красный
    foregroundColor0 = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;
    foregroundColor1 = FOREGROUND_INTENSITY | FOREGROUND_RED;
    backgroundColor = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

    // Устанавливаем начальный цвет текста
    SetConsoleTextAttribute(hStdout, foregroundColor0 | backgroundColor);

    fp = fopen("input.c", "rt");    // Открываем файл с программой на Си
    if(fp == NULL)                  // Проверяем успешность открытия
        return 1;                   // Если ошибка, завершаем программу

    while(!feof(fp)) {              // Пока не достигнут конец файла
        ptr = fgets(line, MAXLEN, fp);  // Читаем строку из файла
        if(ptr == NULL)             // Если строка пустая
            break;                  // Прерываем цикл

        i = 0;                      // Обнуляем индекс массива word
        prev_flag = flag = 0;       // Сбрасываем флаги
        word[0] = '\0';             // Очищаем массив word

        while(*ptr != 0) {          // Пока не достигнут конец строки
            prev_flag = flag;       // Сохраняем предыдущее значение флага

            // Проверяем, является ли текущий символ началом директивы препроцессора
            if(*ptr == '#' && (i == 0 || word[i-1] == ' ' || word[i-1] == '\t' || word[i-1] == '\n'))
                flag = 1;           // Устанавливаем флаг директивы препроцессора
            else if(*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
                flag = 0;           // Сбрасываем флаг при разделителях
            else
                flag = prev_flag;   // Сохраняем предыдущее значение флага

            // Если флаг изменился и есть накопленный текст
            if(flag != prev_flag && i != 0) {
                word[i] = '\0';     // Завершаем текущее слово

                // Выбираем цвет в зависимости от типа текста
                foregroundColor = (prev_flag == 1) ? foregroundColor1 : foregroundColor0;
                textAttribute = foregroundColor | backgroundColor;
                SetConsoleTextAttribute(hStdout, textAttribute);  // Устанавливаем цвет

                printf("%s", word); // Выводим накопленный текст
                i = 0;              // Обнуляем индекс для нового слова
            }

            word[i++] = *ptr++;     // Добавляем символ в массив и переходим дальше
        }

        // Проверяем, остался ли непечатный текст в конце строки
        if(i != 0) {
            word[i] = '\0';         // Завершаем слово

            // Устанавливаем цвет для оставшегося текста
            foregroundColor = (prev_flag == 1) ? foregroundColor1 : foregroundColor0;
            textAttribute = foregroundColor | backgroundColor;
            SetConsoleTextAttribute(hStdout, textAttribute);

            printf("%s", word);     // Выводим оставшийся текст
        }
    }
    printf("\n");                   // Добавляем перевод строки в конце

    SetConsoleTextAttribute(hStdout, 7);  // Восстанавливаем стандартные цвета

    fclose(fp);                     // Закрываем файл
    return 0;                       // Завершаем программу успешно
}
 */