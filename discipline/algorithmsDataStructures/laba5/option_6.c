#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

#define V 10  // Количество вершин в графе
#define E 17  // Количество рёбер в графе
#define INF INT_MAX

// Структура для представления ребра
typedef struct Edge {
    int src, dest, weight;
} Edge;

// Инициализация массива рёбер согласно варианту 19
void initEdges(Edge edges[]) {
    edges[0] = (Edge){1, 8, 3};
    edges[1] = (Edge){1, 2, 5};
    edges[2] = (Edge){1, 7, 19};
    edges[3] = (Edge){1, 4, 6};
    edges[4] = (Edge){2, 3, 5};
    edges[5] = (Edge){2, 5, 4};
    edges[6] = (Edge){2, 8, 3};
    edges[7] = (Edge){3, 7, 2};
    edges[8] = (Edge){4, 9, 6};
    edges[9] = (Edge){4, 5, 1};
    edges[10] = (Edge){5, 6, 9};
    edges[11] = (Edge){5, 7, 6};
    edges[12] = (Edge){6, 7, 4};
    edges[13] = (Edge){6, 1, 4};
    edges[14] = (Edge){7, 8, 4};
    edges[15] = (Edge){7, 9, 1};
    edges[16] = (Edge){8, 5, 4};
    
}

// Функция для восстановления и печати пути
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d ", j);
        return;
    }
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// Функция для генерации изображения графа
void generateGraphImage(Edge edges[], int edgeCount, int shortestPath[], int pathLength) {
    FILE *file = fopen("graph.dot", "w");
    if (file == NULL) {
        perror("Ошибка при создании файла graph.dot");
        return;
    }
    
    fprintf(file, "digraph G {\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=lightblue];\n");
    
    for (int i = 0; i < edgeCount; i++) {
        int isOnPath = 0;
        for (int j = 0; j < pathLength - 1; j++) {
            if ((shortestPath[j] == edges[i].src && shortestPath[j + 1] == edges[i].dest)) {
                isOnPath = 1;
                break;
            }
        }
        
        fprintf(file, "    %d -> %d [label=\"%d\"", edges[i].src, edges[i].dest, edges[i].weight);
        if (isOnPath) {
            fprintf(file, ", color=\"red\", penwidth=3.0");
        }
        fprintf(file, "];\n");
    }
    
    fprintf(file, "}\n");
    fclose(file);
    
    // Кроссплатформенная команда для генерации изображения
    #ifdef _WIN32
        system("dot -Tpng graph.dot -o graph.png 2>nul");
        printf("\nГраф сохранен в файл graph.png\n");
        // Опционально: открыть изображение
        // system("start graph.png");
    #else
        system("dot -Tpng graph.dot -o graph.png 2>/dev/null");
        printf("\nГраф сохранен в файл graph.png\n");
    #endif
}

// Алгоритм Беллмана-Форда
void BellmanFord(Edge edges[], int start, int end) {
    int dist[V];
    int parent[V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[start] = 0;
    
    // Релаксация рёбер V-1 раз
    for (int i = 1; i < V; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Вывод массивов dist и parent
    printf("\nМассив dist: ");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF)
            printf("INF ");
        else
            printf("%d ", dist[i]);
    }
    printf("\n");
    
    printf("Массив parent: ");
    for (int i = 0; i < V; i++) {
        printf("%d ", parent[i]);
    }
    printf("\n");
    
    // Проверка на отрицательные циклы
    for (int i = 0; i < E; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;
        int weight = edges[i].weight;
        if (dist[u] != INF && dist[u] + weight < dist[v]) {
            printf("Граф содержит отрицательный цикл!\n");
            return;
        }
    }
    
    printf("\n=== АЛГОРИТМ БЕЛЛМАНА-ФОРДА ===\n");
    if (dist[end] == INF) {
        printf("Нет пути от вершины %d до вершины %d\n", start, end);
    } else {
        printf("Кратчайший путь от вершины %d до вершины %d: ", start, end);
        printPath(parent, end);
        printf("\nДлина пути: %d\n", dist[end]);
        
        // Восстановление пути для визуализации
        int shortestPath[V];
        int pathLength = 0;
        int j = end;
        while (j != -1) {
            shortestPath[pathLength++] = j;
            j = parent[j];
        }
        
        // Переворачиваем путь
        for (int k = 0; k < pathLength / 2; k++) {
            int temp = shortestPath[k];
            shortestPath[k] = shortestPath[pathLength - 1 - k];
            shortestPath[pathLength - 1 - k] = temp;
        }
        
        generateGraphImage(edges, E, shortestPath, pathLength);
    }
}

// Алгоритм Флойда-Уоршелла
void FloydWarshall(Edge edges[], int start, int end) {
    int dist[V][V];
    int next[V][V];
    
    // Инициализация матриц расстояний и путей
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = INF;
            next[i][j] = -1;
        }
        dist[i][i] = 0;
    }
    
    // Заполнение начальных расстояний из рёбер
    for (int i = 0; i < E; i++) {
        dist[edges[i].src][edges[i].dest] = edges[i].weight;
        next[edges[i].src][edges[i].dest] = edges[i].dest;
    }
    
    // Основной алгоритм Флойда-Уоршелла
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && 
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    printf("\n=== АЛГОРИТМ ФЛОЙДА-УОРШЕЛЛА ===\n");
    if (dist[start][end] == INF) {
        printf("Нет пути от вершины %d до вершины %d\n", start, end);
        return;
    }
    
    printf("Кратчайший путь от вершины %d до вершины %d: ", start, end);
    
    // Восстановление пути
    int shortestPath[V];
    int pathLength = 0;
    int current = start;
    
    while (current != end) {
        shortestPath[pathLength++] = current;
        current = next[current][end];
        if (current == -1 && shortestPath[pathLength-1] != end) {
            printf("Нет пути от вершины %d до вершины %d\n", start, end);
            return;
        }
    }
    shortestPath[pathLength++] = end;
    
    for (int i = 0; i < pathLength; ++i) {
        printf("%d ", shortestPath[i]);
    }
    printf("\nДлина пути: %d\n", dist[start][end]);
    
    generateGraphImage(edges, E, shortestPath, pathLength);
}

int main() {
    Edge edges[E];
    initEdges(edges);
    
    int start_node = 1;  // Начальная вершина
    int end_node = 7;    // Конечная вершина
    
    printf("========================================\n");
    printf("вариант 19\n");
    printf("Поиск кратчайшего пути из вершины %d в вершину %d\n", start_node, end_node);
    printf("========================================\n");
    
    // Применяем алгоритм Беллмана-Форда
    BellmanFord(edges, start_node, end_node);
    
    printf("\n");
    
    // Применяем алгоритм Флойда-Уоршелла
    FloydWarshall(edges, start_node, end_node);
    
    printf("\n========================================\n");
    
    return 0;
}
