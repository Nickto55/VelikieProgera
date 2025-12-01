#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

struct Node {
    char* word;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(const char* word) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->word = (char*)malloc((strlen(word) + 30));
    strcpy(newNode->word, word);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, const char* word) {
    if (root == NULL) {
        return createNode(word);
    }

    // Находим общий префикс (побайтовое сравнение для UTF-8)
    int i = 0;
    int rootLen = strlen(root->word);
    int wordLen = strlen(word);
    
    while (i < rootLen && i < wordLen && root->word[i] == word[i]) {
        i++;
    }

    // Если текущее слово является префиксом нового слова
    if (i == rootLen && i < wordLen) {
        // Новое слово продолжается после текущего - ветвим по следующему символу
        unsigned char nextChar = (unsigned char)word[i];
        // Разделяем по середине диапазона символов
        if (nextChar < 200) {
            root->left = insert(root->left, word);
        } else {
            root->right = insert(root->right, word);
        }
    }
    // Если новое слово является префиксом текущего
    else if (i == wordLen && i < rootLen) {
        // Новое слово короче - идём влево
        root->left = insert(root->left, word);
    }
    // Если слова различаются после общего префикса
    else if (i < rootLen && i < wordLen) {
        // Сравниваем по первому различающемуся байту (для UTF-8)
        if ((unsigned char)word[i] < (unsigned char)root->word[i]) {
            root->left = insert(root->left, word);
        } else {
            root->right = insert(root->right, word);
        }
    }
    // Если слова одинаковые (i == rootLen == wordLen), ничего не делаем
    
    return root;
}

int getWordLength(struct Node* node) {
    if (node == NULL) return 0;
    return strlen(node->word);
}

struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct Node* deleteNode(struct Node* root, int targetLength) {
    if (root == NULL) return root;

    root->left = deleteNode(root->left, targetLength);
    root->right = deleteNode(root->right, targetLength);

    if (getWordLength(root) == targetLength) {
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root->word);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root->word);
            free(root);
            return temp;
        }

        struct Node* temp = minValueNode(root->right);
        free(root->word);
        root->word = (char*)malloc((strlen(temp->word) + 1) * sizeof(char));
        strcpy(root->word, temp->word);
        root->right = deleteNode(root->right, getWordLength(temp));
    }
    return root;
}

struct Trunk {
    struct Trunk* prev;
    char* str;
};

void showTrunks(struct Trunk* p) {
    if (p == NULL) {
        return;
    }
    showTrunks(p->prev);
    printf("%s", p->str);
}

void printTree(struct Node* root, struct Trunk* prev, int isLeft) {
    if (root == NULL) {
        return;
    }

    char* prev_str = "    ";
    struct Trunk* trunk = (struct Trunk*)malloc(sizeof(struct Trunk));
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, 1);

    if (!prev) {
        trunk->str = "----";
    } else if (isLeft) {
        trunk->str = ".---";
        prev_str = "    ";
    } else {
        trunk->str = "`---";
        if (prev) prev->str = prev_str;
    }

    showTrunks(trunk);
    printf(" %s\n", root->word);

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";

    printTree(root->left, trunk, 0);
    free(trunk);
}

void deleteTree(struct Node* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root->word);
        free(root);
    }
}

struct Node* buildTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: Не удалось открыть файл %s.\n", filename);
        return NULL;
    }

    struct Node* root = NULL;
    char word[256];
    while (fscanf(file, "%255s", word) == 1) {
        root = insert(root, word);
    }
    fclose(file);
    return root;
}

int main() {
    // Устанавливаем локаль для корректной работы с русским языком
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    const char* filename = "input.txt";
    struct Node* root = buildTreeFromFile(filename);

    if (root != NULL) {
        printf("Начальное дерево:\n");
        printTree(root, NULL, 0);
        printf("\n");

        int lengthToDelete;
        printf("Введите длину слова для удаления: ");
        scanf("%d", &lengthToDelete);

        root = deleteNode(root, lengthToDelete);

        printf("\nДерево после удаления:\n");
        printTree(root, NULL, 0);
        printf("\n");

        deleteTree(root);
        printf("Дерево удалено.\n");
    }

    return 0;
}
