#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef DEBUG
    #define LOG(x) printf("DEBUG: %s\n", x)
#else
    #define LOG(x)
#endif

int main() {
    char buffer[BUFFER_SIZE];
    int a = 5;
    int b = 10;
    int max_val = MAX(a, b);

    // Это обычный комментарий
    /*
     * А это
     * многострочный
     * комментарий
     */

    printf("Максимум: %d\n", max_val);

    #ifdef DEBUG
        LOG("Программа завершена");
    #endif

    return 0;
}
