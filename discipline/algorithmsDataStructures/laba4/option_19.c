/*тотально и внимательно обьясни код, как йстроенна функция, за что отвечает, как и что используеТ*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct Node {
    char* word;
    struct Node* left;
    struct Node* right;
};

int utf8_strlen(const char* s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;
        s++;
    }
    return len;
}

struct Node* createNode(const char* word) {
    struct Node* node = malloc(sizeof(struct Node));
    node->word = malloc(strlen(word) + 1);
    strcpy(node->word, word);
    node->left = node->right = NULL;
    return node;
}

struct Node* insert(struct Node* root, const char* word) {
    if (!root) return createNode(word);
    int cmp = strcmp(word, root->word);
    if (cmp < 0) root->left = insert(root->left, word);
    else if (cmp > 0) root->right = insert(root->right, word);
    return root;
}

struct Node* minValueNode(struct Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

struct Node* deleteNode(struct Node* root, int targetLen) {
    if (!root) return root;
    root->left = deleteNode(root->left, targetLen);
    root->right = deleteNode(root->right, targetLen);
    if (utf8_strlen(root->word) == targetLen) {
        if (!root->left) {
            struct Node* temp = root->right;
            free(root->word);
            free(root);
            return temp;
        } else if (!root->right) {
            struct Node* temp = root->left;
            free(root->word);
            free(root);
            return temp;
        }
        struct Node* temp = minValueNode(root->right);
        free(root->word);
        root->word = malloc(strlen(temp->word) + 1);
        strcpy(root->word, temp->word);
        root->right = deleteNode(root->right, utf8_strlen(temp->word));
    }
    return root;
}

void deleteTree(struct Node* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root->word);
        free(root);
    }
}

struct Trunk {
    struct Trunk* prev;
    char* str;
};

void showTrunks(struct Trunk* p) {
    if (p) {
        showTrunks(p->prev);
        printf("%s", p->str);
    }
}

void print_tree_helper(struct Node* root, struct Trunk* prev, int isLeft) {
    if (!root) return;
    char* prevStr = "    ";
    struct Trunk* trunk = malloc(sizeof(struct Trunk));
    trunk->prev = prev;
    trunk->str = "    ";

    print_tree_helper(root->right, trunk, 1);

    if (!prev) {
        trunk->str = "----";
    } else if (isLeft) {
        trunk->str = ".---";
        prevStr = "    ";
    } else {
        trunk->str = "`---";
        if (prev) prev->str = (char*)prevStr;
    }

    showTrunks(trunk);
    printf(" %s\n", root->word);

    if (prev && isLeft) prev->str = (char*)"    ";

    trunk->str = "   |";
    print_tree_helper(root->left, trunk, 0);

    free(trunk);
}

void print_tree(struct Node* root) {
    print_tree_helper(root, NULL, 0);
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
    setlocale(LC_ALL, "ru_RU.UTF-8");
    const char* filename = "input.txt";
    struct Node* root = buildTreeFromFile(filename);
    if (!root) return 1;
    printf("основное дерево:\n");
    print_tree(root);
    int len;
    printf("\nВведите длину словf: ");
    scanf("%d", &len);
    root = deleteNode(root, len);
    printf("\nДерево после удаления:\n");
    print_tree(root);
    deleteTree(root);
    return 0;
}
