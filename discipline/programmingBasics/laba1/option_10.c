# include <stdio.h>
# define LEN 1000
int main(void)
{
    char line[LEN];
    char* p = line;
    char* start_ptr = NULL;
    int counter = 0;
    int max = 0;
    gets(line);

    for (p = line; *p != '/0'; p++) {
        if (*p != ' ' && *p != '.' && *p != ',') {
            counter++;

            if (counter > max) {
                max = counter;
                start_ptr = p - max + 1;
            }
        }
        else {
            counter = 0;
        }
    }

    if (max == 0) {
        puts(line);
        return 0;
    }

    char *src = start_ptr + max;
    char *dst = start_ptr;

    while ((*dst++ = *src++)) {

    }

    puts(line);

    return 0;

}
