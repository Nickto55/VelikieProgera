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

// Вспомогательная функция для Heapsort: просеивание вниз
void heapify(int arr[], int n, int i) {
    int largest = i;          // Инициализируем наибольший как корень
    int left = 2 * i + 1;     // Левый потомок
    int right = 2 * i + 2;    // Правый потомок

    // Если левый потомок больше корня
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Если правый потомок больше текущего наибольшего
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Если наибольший не корень — меняем и рекурсивно просеиваем
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

// Heapsort (пирамидальная сортировка)
void heapsort(int n, int mass[]) {
    // Построение max-heap (начинаем с последнего внутреннего узла)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(mass, n, i);

    // Извлечение элементов из кучи один за другим
    for (int i = n - 1; i > 0; i--) {
        // Перемещаем текущий корень (максимум) в конец
        int temp = mass[0];
        mass[0] = mass[i];
        mass[i] = temp;

        // Восстанавливаем кучу для уменьшенного размера (i элементов)
        heapify(mass, i, 0);
    }
}

// Проверка отсортированности массива по возрастанию
bool mssorted(int mass[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (mass[i] > mass[i + 1]) {
            return false;
        }
    }
    return true;
}

// Функция для вывода массива 
void vdmass(int mass[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", mass[i]);
    }
    printf("\n");
}

// Чтение массива из файла 
bool read_mass_file(const char* filename, int mass[], int n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        return false;
    }
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%d", &mass[i]) != 1) {
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
        input_mass(mass, n);
    } else if (fill == 2) {
        random_mass(mass, n);
    } else if (fill == 3) {
        printf("Введите имя файла: ");
        scanf("%s", filename);
        ok = read_mass_file(filename, mass, n);
        if (!ok) {
            free(mass);
            return 1;
        }
    } else {
        printf("Неверный выбор!\n");
        free(mass);
        return 1;
    }

    // Вывод исходного массива
    printf("Исходный массив:\n");
    vdmass(mass, n);

    // Выбор метода сортировки
    printf("Выберите метод сортировки:\n1 - Выбором\n2 - Heapsort\nВаш выбор: ");
    scanf("%d", &method);

    // Сортировка выбранным методом
    switch (method) {
        case 1:
            sort_vybor(n, mass);
            printf("Отсортировано выбором:\n");
            break;
        case 2:
            heapsort(n, mass);
            printf("Отсортировано Heapsort:\n");
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
