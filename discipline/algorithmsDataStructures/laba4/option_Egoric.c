/*Найдите высоту дерева H и удалите из него все вершины на уровне H/2.
Использовать алгоритм построения АВЛ-дерева.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для узла АВЛ-дерева
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Функция для получения высоты узла
int getHeight(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Функция для вычисления максимального из двух чисел
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Функция для создания нового узла
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

// Правый поворот
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Левый поворот
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Получение баланс-фактора узла
int getBalance(Node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Вставка узла в АВЛ-дерево
Node* insert(Node* node, int data) {
    if (node == NULL)
        return createNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    // Левый левый случай
    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    // Правый правый случай
    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    // Левый правый случай
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Правый левый случай
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Функция для полного удаления дерева
void deleteTree(Node* root) {
    if (root == NULL) return;
    
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

// Функция для вычисления высоты дерева
int treeHeight(Node* root) {
    if (root == NULL)
        return 0;
    
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    
    return 1 + max(leftHeight, rightHeight);
}

// Функция для удаления узлов на определенном уровне
Node* deleteNodesAtLevel(Node* root, int targetLevel, int currentLevel) {
    if (root == NULL)
        return NULL;
    
    if (currentLevel == targetLevel) {
        Node* leftChild = root->left;
        Node* rightChild = root->right;
        
        free(root);
        
        if (leftChild != NULL)
            deleteNodesAtLevel(leftChild, targetLevel, currentLevel);
        if (rightChild != NULL)
            deleteNodesAtLevel(rightChild, targetLevel, currentLevel);
        
        return NULL;
    }
    
    root->left = deleteNodesAtLevel(root->left, targetLevel, currentLevel + 1);
    root->right = deleteNodesAtLevel(root->right, targetLevel, currentLevel + 1);
    
    return root;
}

// Функция для обхода дерева в порядке in-order
void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

// Функция для чтения данных из файла
Node* readFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }
    
    Node* root = NULL;
    int value;
    
    printf("Чтение данных из файла %s:\n", filename);
    while (fscanf(file, "%d", &value) == 1) {
        printf("%d ", value);
        root = insert(root, value);
    }
    printf("\n");
    
    fclose(file);
    return root;
}

// Внешняя функция из printtree.c для отображения дерева
// Предполагается, что эта функция определена в printtree.c
extern void printTree(Node* root);

// Простая замена если printtree.c недоступен
void simplePrintTree(Node* root, int level) {
    if (root == NULL) return;
    
    simplePrintTree(root->right, level + 1);
    
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d\n", root->data);
    
    simplePrintTree(root->left, level + 1);
}

int main() {
    Node* root = NULL;
    
    printf("Лабораторная работа: АВЛ-деревья\n");
    printf("================================\n");
    
    // Всегда используем data.txt
    printf("Используем файл: data.txt\n");
    root = readFromFile("data.txt");
    
    if (root == NULL) {
        printf("\nСоздаем тестовый файл data.txt...\n");
        
        // Создаем файл с тестовыми данными
        FILE* file = fopen("data.txt", "w");
        if (file != NULL) {
            fprintf(file, "50 30 70 20 40 60 80 10 25 35 45\n");
            fclose(file);
            printf("Файл data.txt создан!\n");
            
            // Пробуем снова
            root = readFromFile("data.txt");
        }
    }
    
    if (root == NULL) {
        printf("Не удалось построить дерево. Завершение программы.\n");
        return 1;
    }
    
    // Вычисляем высоту дерева
    int height = treeHeight(root);
    printf("\nВысота дерева H: %d\n", height);
    
    // Выводим дерево до удаления
    printf("\nДерево до удаления узлов на уровне H/2 = %d:\n", height/2);
    simplePrintTree(root, 0);
    
    printf("\nIn-order обход до удаления: ");
    inOrder(root);
    printf("\n");
    
    // Удаляем узлы на уровне H/2
    int levelToDelete = height / 2;
    printf("\nУдаляем все вершины на уровне H/2 = %d\n", levelToDelete);
    
    root = deleteNodesAtLevel(root, levelToDelete, 1);
    
    // Выводим дерево после удаления
    printf("\nДерево после удаления вершин на уровне H/2:\n");
    simplePrintTree(root, 0);
    
    printf("\nIn-order обход после удаления: ");
    inOrder(root);
    printf("\n");
    
    // Полностью удаляем дерево
    deleteTree(root);
    printf("\nДерево полностью удалено из памяти.\n");
    
    return 0;
}
