/*19.Определить, есть ли в списке действительных чисел элементы, превосходящие
сумму всех остальных элементов списка.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    double data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int size;
} List;

List* create_list() {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

void append(List* list, double value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
}

double get(List* list, int index) {
    Node* current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void free_list(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int has_element_exceeding_sum(List* list) {
    if (list->size <= 1) return 0;

    double sum = 0.0;
    Node* current = list->head;
    while (current != NULL) {
        sum += current->data;
        current = current->next;
    }

    current = list->head;
    int index = 0;
    while (current != NULL) {
        if (current->data > sum - current->data) {
            printf("Элемент %.2f превосходит сумму остальных элементов.\n", current->data);
            return 1;
        }
        current = current->next;
        index++;
    }
    return 0;
}

void print_list(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("%.2f ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    int n;
    printf("Введите длину списка: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Список пуст.\n");
        return 0;
    }

    List* list = create_list();

    int choice;
    printf("Выберите способ заполнения:\n1 - ввод вручную\n2 - генерация случайных чисел\n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Введите %d действительных чисел:\n", n);
        for (int i = 0; i < n; i++) {
            double value;
            scanf("%lf", &value);
            append(list, value);
        }
    } else {
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            double value = ((double)rand() / RAND_MAX) * 100.0;
            append(list, value);
        }
    }

    printf("Список:\n");
    print_list(list);

    if (!has_element_exceeding_sum(list)) {
        printf("Нет элементов, превосходящих сумму остальных.\n");
    }

    free_list(list);

    return 0;
}
