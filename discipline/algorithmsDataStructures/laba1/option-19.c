#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

#define MAX_VARIABLES 1000

char double_variables[MAX_VARIABLES][256];
int variable_count = 0;

// Добавить переменную типа double
void add_variable(const char* name) {
    if (variable_count < MAX_VARIABLES) {
        strcpy(double_variables[variable_count], name);
        variable_count++;
    }
}

// Проверить, является ли переменная типа double
int is_double_var(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(double_variables[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

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
    int in_single_line_comment = 0;
    int in_multi_line_comment = 0;
    int in_string = 0;
    int in_char_literal = 0;
    char word_buffer[256];
    int buffer_pos = 0;
    int after_double = 0; // Флаг, что мы только что встретили слово "double"
    
    printf("Содержимое файла %s:\n\n", filename);
    
    while ((current_char = fgetc(file)) != EOF) {
        // Обработка выхода из строк и символьных литералов
        if (!in_single_line_comment && !in_multi_line_comment) {
            if (current_char == '"' && !in_char_literal && prev_char != '\\') {
                in_string = !in_string;
            }
            if (current_char == '\'' && !in_string && prev_char != '\\') {
                in_char_literal = !in_char_literal;
            }
        }
        
        // Обработка комментариев
        if (!in_string && !in_char_literal) {
            if (!in_multi_line_comment && !in_single_line_comment) {
                if (prev_char == '/' && current_char == '/') {
                    in_single_line_comment = 1;
                }
                else if (prev_char == '/' && current_char == '*') {
                    in_multi_line_comment = 1;
                }
            }
            else if (in_multi_line_comment && prev_char == '*' && current_char == '/') {
                in_multi_line_comment = 0;
            }
            else if (in_single_line_comment && current_char == '\n') {
                in_single_line_comment = 0;
            }
        }
        
        // Если мы не в комментариях, строках или символьных литералах
        if (!in_single_line_comment && !in_multi_line_comment && !in_string && !in_char_literal) {
            // Собираем слова (идентификаторы и ключевые слова)
            if (isalpha(current_char) || current_char == '_') {
                if (buffer_pos < 255) {
                    word_buffer[buffer_pos++] = current_char;
                }
            } else if (isdigit(current_char) && buffer_pos > 0) {
                // Цифры могут быть частью идентификатора только после букв/подчеркивания
                if (buffer_pos < 255) {
                    word_buffer[buffer_pos++] = current_char;
                }
            } else {
                // Конец слова
                if (buffer_pos > 0) {
                    word_buffer[buffer_pos] = '\0';
                    
                    if (strcmp(word_buffer, "double") == 0) {
                        printf(YELLOW "double" RESET);
                        after_double = 1;
                    } else if (after_double) {
                        // Это имя переменной типа double - подсвечиваем зеленым
                        add_variable(word_buffer);
                        printf(GREEN "%s" RESET, word_buffer);
                        after_double = 0;
                    } else if (is_double_var(word_buffer)) {
                        // Это переменная типа double, подсвечиваем зеленым
                        printf(GREEN "%s" RESET, word_buffer);
                    } else {
                        printf("%s", word_buffer);
                    }
                    buffer_pos = 0;
                }
                
                // Сбрасываем флаг after_double при встрече точки с запятой, запятой или других разделителей
                if (current_char == ';' || current_char == ',' || current_char == '=' || 
                    current_char == '[' || current_char == ']' || current_char == '(' || 
                    current_char == ')' || current_char == '{' || current_char == '}') {
                    after_double = 0;
                }
                
                putchar(current_char);
            }
        } else {
            // В комментариях, строках и т.д. просто выводим символы
            if (buffer_pos > 0) {
                word_buffer[buffer_pos] = '\0';
                printf("%s", word_buffer);
                buffer_pos = 0;
                after_double = 0;
            }
            putchar(current_char);
        }
        
        prev_char = current_char;
    }
    
    // Выводим последнее слово, если оно осталось
    if (buffer_pos > 0) {
        word_buffer[buffer_pos] = '\0';
        if (strcmp(word_buffer, "double") == 0) {
            printf(YELLOW "double" RESET);
        } else if (is_double_var(word_buffer)) {
            printf(GREEN "%s" RESET, word_buffer);
        } else {
            printf("%s", word_buffer);
        }
    }
    
    fclose(file);
    return 0;
}