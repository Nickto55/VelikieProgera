#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

int main() {
    char filename[256];
    printf("Введите имя файла, с расширением: ");
    scanf("%255s", filename);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return 1;
    }
    
    int current_char;
    int prev_char = 0;
    
    printf("Содержимое файла %s:\n\n", filename);
    
    while ((current_char = fgetc(file)) != EOF) {
        // Проверяем, является ли текущий символ началом десятичного числа
        if (isdigit(current_char)) {
            // Проверяем, не является ли это частью другой системы счисления
            int is_other_system = 0;
            
            // Проверяем шестнадцатеричные (0x, 0X)
            if (prev_char == '0' && (current_char == 'x' || current_char == 'X')) {
                is_other_system = 1;
            }
            // Проверяем двоичные (0b, 0B)
            else if (prev_char == '0' && (current_char == 'b' || current_char == 'B')) {
                is_other_system = 1;
            }
            // Проверяем восьмеричные (0 и цифры 0-7)
            else if (prev_char == '0' && current_char >= '0' && current_char <= '7') {
                is_other_system = 1;
            }
            
            if (!is_other_system) {
                // Это десятичное число
                printf(GREEN "%c", current_char);
                
                // Читаем все последующие цифры
                int next_char;
                while ((next_char = fgetc(file)) != EOF && isdigit(next_char)) {
                    putchar(next_char);
                }
                printf(RESET);
                
                // Вернем последний неподходящий символ обратно в поток
                if (next_char != EOF) {
                    ungetc(next_char, file);
                }
                prev_char = current_char;
                continue;
            }
        }
        
        putchar(current_char);
        prev_char = current_char;
    }
    
    fclose(file);
    return 0;
}