#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Сортировка вставками (вместо сортировки выбором)
void insertion_sort(int n, int mass[]) {
    for (int i = 1; i < n; i++) {
        int key = mass[i];
        int j = i - 1;

        // Сдвигаем элементы, большие key, вправо
        while (j >= 0 && mass[j] > key) {
            mass[j + 1] = mass[j];
            j--;
        }
        mass[j + 1] = key;
    }
}

// Вспомогательная функция для Heapsort: просеивание вниз
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

// Heapsort (пирамидальная сортировка)
void heapsort(int n, int mass[]) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(mass, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = mass[0];
        mass[0] = mass[i];
        mass[i] = temp;
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

    printf("Введите размер массива: ");
    scanf("%d", &n);

    int* mass = malloc(n * sizeof(int));
    if (!mass) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    printf("Выберите способ заполнения массива:\n1 - Ввод с клавиатуры\n2 - Случайные числа\n3 - Чтение из файла\nВаш выбор: ");
    scanf("%d", &fill);
    bool ok = true;

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

    printf("Исходный массив:\n");
    vdmass(mass, n);

    printf("Выберите метод сортировки:\n1 - Вставками\n2 - Heapsort\nВаш выбор: ");
    scanf("%d", &method);

    switch (method) {
        case 1:
            insertion_sort(n, mass);
            printf("Отсортировано вставками:\n");
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

    vdmass(mass, n);

    if (mssorted(mass, n)) {
        printf("Проверка: массив отсортирован верно.\n");
    } else {
        printf("Проверка: массив отсортирован неверно!\n");
    }

    free(mass);
    return 0;
}
