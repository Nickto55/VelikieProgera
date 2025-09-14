#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void ydali_stroky(char *str) {
    if (!str || *str == '\0') return;

    char *start = str;

    // Пропускаем начальные пробелы
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // Находим конец строки
    char *end = start + strlen(start) - 1;
    
    // Пропускаем конечные пробелы
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    // Удаляем множественные пробелы внутри строки
    char *src = start;
    char *dst = start;

    while (*src) {
        if (isspace((unsigned char)*src)) {
            if (dst == start || !isspace((unsigned char)*(dst - 1))) {
                *dst++ = ' '; // Заменяем все пробелы на один пробел
            }
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';

    // Копируем обратно в начало, если было смещение
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int main() {
    FILE *fpin;
    FILE *fpout;

    fpin = fopen("result.txt", "r");
    if (fpin == NULL) {
        perror("Ошибка открытия result.txt");
        return 1;
    }

    fpout = fopen("vyvod.txt", "w"); // Исправлено имя файла
    if (fpout == NULL) {
        perror("Ошибка открытия vyvod.txt");
        fclose(fpin);
        return 1;
    }

    char line[1024];
    int line_count = 0;

    while (fgets(line, sizeof(line), fpin)) {
        // Удаляем символ новой строки
        line[strcspn(line, "\n")] = '\0';
        
        ydali_stroky(line);
        fprintf(fpout, "%s\n", line);
        line_count++;
    }

    fclose(fpin);
    fclose(fpout);

    printf("Обработано %d строк. Результат в 'vyvod.txt'\n", line_count);
    return 0;
}
