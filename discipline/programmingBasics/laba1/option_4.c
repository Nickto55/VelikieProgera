#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Функция для удаления слов длиннее N букв
void removeLongWords(char *str, int N) {
    char *src = str;   // указатель на исходную строку (читаем отсюда)
    char *dst = str;   // указатель на начало строки (пишем сюда)
    int inWord = 0;    // флаг: находимся ли внутри слова
    int wordLen = 0;   // длина текущего слова
    char *wordStart = NULL; // начало текущего слова

    while (*src != '\0') {
        if (isalpha(*src)) { // если символ — буква
            if (!inWord) {
                inWord = 1;
                wordStart = src; // запоминаем начало слова
                wordLen = 1;
            } else {
                wordLen++;
            }
        } else { // не буква — разделитель
            if (inWord) {
                // закончилось слово — проверяем длину
                if (wordLen <= N) {
                    // копируем слово в dst
                    for (char *p = wordStart; p < src; p++) {
                        *dst++ = *p;
                    }
                    // добавляем пробел, если дальше есть слова
                    if (src[0] != '\0' && !isspace(src[0])) {
                        *dst++ = ' ';
                    }
                }
                inWord = 0;
            }
            // пропускаем лишние пробелы, но оставляем один
            if (!inWord && isspace(*src)) {
                char *next = src + 1;
                while (*next != '\0' && isspace(*next)) next++;
                if (*next != '\0' && isalpha(*next)) {
                    *dst++ = ' ';
                }
            }
        }
        src++;
    }

    // обработка последнего слова, если строка заканчивается им
    if (inWord && wordLen <= N) {
        for (char *p = wordStart; p < src; p++) {
            *dst++ = *p;
        }
    }

    *dst = '\0'; // завершаем строку
}

// Основная функция
int main() {
    char str[1000];
    int N;

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);

    // Удаляем символ новой строки, если есть
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    printf("Введите N (максимальная длина слова): ");
    scanf("%d", &N);

    // Удаляем слова длиннее N
    removeLongWords(str, N);

    printf("Результат: %s\n", str);

    return 0;
}
