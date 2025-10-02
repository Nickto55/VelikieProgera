#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Сортировка выбором (полная)
void sort_vybor(int n, int mass[]) {
    for (int i = 0; i < n - 1; i++) {
        int minPosition = i;
        for (int j = i + 1; j < n; j++) {
            if (mass[j] < mass[minPosition]) {
                minPosition = j;
            }
        }
        int tmp = mass[i];
        mass[i] = mass[minPosition];
        mass[minPosition] = tmp;
    }
}

// Сортировка Шелла 
void ShellSort(int n, int mass[])
{
    int i, j, step;
    int tmp;
    for (step = n / 2; step > 0; step /= 2)
        for (i = step; i < n; i++)
        {
            tmp = mass[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp < mass[j - step])
                    mass[j] = mass[j - step];
                else
                    break;
            }
            mass[j] = tmp;
        }
}

// Проверка отсортированности массива по возрастанию
bool mssorted(int mass[], int n)
{
    for(int i = 0; i < n - 1; i++){
        if (mass[i] > mass[i + 1]){
            return false;
        }
    }
    return true;
}

// Функция для вывода массива 
void vdmass(int mass[], int n)
{
    for(int i = 0; i < n; i++){
        printf("%d ", mass[i]);
    }
    printf("\n");
}

// Чтение массива из файла 
bool read_mass_file(const char* filename, int mass[], int n){
    FILE* file = fopen(filename, "r");
    if (!file){
        printf("Ошибка открытия файла %s\n", filename);
        return false;
    }
    for (int i = 0; i < n; i++){
        if(fscanf(file, "%d", &mass[i]) != 1){
            printf("Ошибка чтения файла\n");
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

// Заполнение массива случайными числами
void random_mass(int mass[], int n) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        mass[i] = rand() % 100;
    }
}

// Ввод массива с клавиатуры
void input_mass(int mass[], int n) {
    printf("Введите %d элементов массива:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &mass[i]);
    }
}

int main() {
    int n, method, fill;
    char filename[100];

    // Ввод размера массива
    printf("Введите размер массива: ");
    scanf("%d", &n);

    // Выделение памяти под массив
    int* mass = malloc(n * sizeof(int));
    if (!mass) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    // Выбор способа заполнения массива
    printf("Выберите способ заполнения массива:\n1 - Ввод с клавиатуры\n2 - Случайные числа\n3 - Чтение из файла\nВаш выбор: ");
    scanf("%d", &fill);
    bool ok = true;

    // Заполнение массива выбранным способом
    if (fill == 1) {
        // Ввод с клавиатуры
        input_mass(mass, n);
    } else if (fill == 2) {
        // Генерация случайных чисел
        random_mass(mass, n);
    } else if (fill == 3) {
        // Чтение из файла
        printf("Введите имя файла: ");
        scanf("%s", filename);
        ok = read_mass_file(filename, mass, n);
        if (!ok) {
            free(mass);
            return 1;
        }
    } else {
        // Некорректный выбор
        printf("Неверный выбор!\n");
        free(mass);
        return 1;
    }

    // Вывод исходного массива
    printf("Исходный массив:\n");
    vdmass(mass, n);

    // Выбор метода сортировки
    printf("Выберите метод сортировки:\n1 - Выбором\n2 - Шелла\nВаш выбор: ");
    scanf("%d", &method);

    // Сортировка выбранным методом
    switch (method) {
        case 1:
            sort_vybor(n, mass);
            printf("Отсортировано выбором:\n");
            break;
        case 2:
            ShellSort(n, mass);
            printf("Отсортировано Шеллом:\n");
            break;
        default:
            printf("Неверный выбор!\n");
            free(mass);
            return 1;
    }

    // Вывод отсортированного массива
    vdmass(mass, n);

    // Проверка корректности сортировки
    if (mssorted(mass, n)) {
        printf("Проверка: массив отсортирован верно.\n");
    } else {
        printf("Проверка: массив отсортирован неверно!\n");
    }

    // Освобождение памяти
    free(mass);
    return 0;
}

