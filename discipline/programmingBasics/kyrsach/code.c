#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_QUESTIONS 100
#define MAX_ANSWERS 10
#define MAX_LINE 512
#define MAX_NAME 100

typedef struct {
    int id;
    char question[512];
    char options[MAX_ANSWERS][256];
    int correct_answer;
    char category[50];
    char difficulty[20]; // easy, medium, hard
    int times_asked;
    int times_correct;
} Question;

Question questions[MAX_QUESTIONS];
int total_questions = 0;

// === Прототипы функций ===
void load_questions();
void save_test_result(const char* name, int score, int total);
void show_stats_per_question();
void start_test(int mode);

int main() {
    srand(time(NULL));
    load_questions();

    int choice;
    while (1) {
        printf("\n--- Меню ---\n");
        printf("1. Пройти тест\n");
        printf("2. Посмотреть статистику по вопросам\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        if (choice == 1)
            start_test(0); // выбор режима внутри
        else if (choice == 2)
            show_stats_per_question();
        else if (choice == 0)
            break;
        else
            printf("Неверный выбор.\n");
    }

    return 0;
}

void load_questions() {
    FILE *f = fopen("questions.txt", "r");
    if (!f) {
        printf("Файл questions.txt не найден!\n");
        exit(1);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        sscanf(line, "%d | %[^\n]", &questions[total_questions].id, questions[total_questions].question);

        // Дополнительно читаем опции и другие поля (предполагается формат в файле)
        // Для простоты: предположим, что в строке содержится всё, и мы парсим её
        char* q = questions[total_questions].question;
        char* opt_start = strstr(q, "|");
        if (!opt_start) continue;
        *opt_start = 0;
        opt_start++;

        char* cat_start = strstr(opt_start, "|");
        if (!cat_start) continue;
        *cat_start = 0;
        strcpy(questions[total_questions].category, opt_start);
        opt_start = cat_start + 1;

        char* diff_start = strstr(opt_start, "|");
        if (!diff_start) continue;
        *diff_start = 0;
        strcpy(questions[total_questions].difficulty, opt_start);
        opt_start = diff_start + 1;

        char* corr_str = strstr(opt_start, "|");
        if (!corr_str) continue;
        *corr_str = 0;
        questions[total_questions].correct_answer = atoi(opt_start);
        opt_start = corr_str + 1;

        // Остальное — варианты ответов (предположим до 4 штук)
        char* token = strtok(opt_start, ";");
        int i = 0;
        while (token && i < MAX_ANSWERS) {
            strcpy(questions[total_questions].options[i], token);
            i++;
            token = strtok(NULL, ";");
        }

        total_questions++;
    }
    fclose(f);
}

void start_test(int unused) {
    char name[MAX_NAME];
    int n, mode;
    printf("Введите ваше имя: ");
    scanf(" %[^\n]", name);

    printf("Сколько вопросов в тесте? ");
    scanf("%d", &n);
    getchar(); // Очищаем буфер ввода

    printf("Выберите режим:\n");
    printf("1. Случайный выбор из всех вопросов\n");
    printf("2. По категории\n");
    printf("3. По уровню сложности\n");
    printf("Ваш выбор: ");
    scanf("%d", &mode);

    int selected_ids[n];
    int count = 0;

    if (mode == 1) { // случайные
        int* used = calloc(total_questions, sizeof(int));
        while (count < n) {
            int idx = rand() % total_questions;
            if (!used[idx]) {
                selected_ids[count++] = idx;
                used[idx] = 1;
            }
        }
        free(used);
    }
    else if (mode == 2) {
        char cat[50];
        printf("Введите категорию: ");
        scanf(" %[^\n]", cat);  // <-- Теперь будет работать корректно
        int* temp = malloc(total_questions * sizeof(int));
        int temp_count = 0;
        for (int i = 0; i < total_questions; i++) {
            if (strcmp(questions[i].category, cat) == 0)
                temp[temp_count++] = i;
        }
        if (temp_count == 0) {
            printf("Нет вопросов в этой категории.\n");
            free(temp);
            return;
        }
        int* used = calloc(temp_count, sizeof(int));
        while (count < n) {
            int idx = rand() % temp_count;
            if (!used[idx]) {
                selected_ids[count++] = temp[idx];
                used[idx] = 1;
            }
        }
        free(temp);
        free(used);
    }
    else if (mode == 3) {
        char diff[20];
        printf("Введите уровень сложности (easy/medium/hard): ");
        scanf(" %[^\n]", diff);  // <-- Теперь будет работать корректно
        int* temp = malloc(total_questions * sizeof(int));
        int temp_count = 0;
        for (int i = 0; i < total_questions; i++) {
            if (strcmp(questions[i].difficulty, diff) == 0)
                temp[temp_count++] = i;
        }
        if (temp_count == 0) {
            printf("Нет вопросов с таким уровнем сложности.\n");
            free(temp);
            return;
        }
        int* used = calloc(temp_count, sizeof(int));
        while (count < n) {
            int idx = rand() % temp_count;
            if (!used[idx]) {
                selected_ids[count++] = temp[idx];
                used[idx] = 1;
            }
        }
        free(temp);
        free(used);
    }
    else {
        printf("Неверный режим.\n");
        return;
    }

    int correct = 0;
    int answers[n];
    for (int i = 0; i < n; i++) {
        int q_idx = selected_ids[i];
        Question q = questions[q_idx];

        printf("\n%d. %s\n", i + 1, q.question);
        for (int j = 0; j < MAX_ANSWERS && strlen(q.options[j]) > 0; j++)
            printf("%d. %s\n", j + 1, q.options[j]);

        printf("Ваш ответ: ");
        scanf("%d", &answers[i]);

        if (answers[i] == q.correct_answer) {
            correct++;
            questions[q_idx].times_correct++;
        }
        questions[q_idx].times_asked++;
    }

    float percentage = ((float)correct / n) * 100;
    printf("\nВы набрали %d из %d баллов (%.2f%%)\n", correct, n, percentage);

    save_test_result(name, correct, n);

    printf("\n--- Статистика ---\n");
    for (int i = 0; i < n; i++) {
        int q_idx = selected_ids[i];
        // Исправляем порядок аргументов в printf
        printf("ID %d: %s -> %s\n", questions[q_idx].id,
               questions[q_idx].question,
               (answers[i] == questions[q_idx].correct_answer ? "Правильно" : "Неправильно"));
    }
}

void save_test_result(const char* name, int score, int total) {
    FILE *f = fopen("test_results.csv", "a");
    if (!f) {
        f = fopen("test_results.csv", "w");
        fprintf(f, "Name,Score,Total,Date\n");
    }

    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt)-1] = 0; // убираем символ новой строки

    fprintf(f, "%s,%d,%d,\"%s\"\n", name, score, total, dt);
    fclose(f);
}

void show_stats_per_question() {
    printf("\n--- Статистика по вопросам ---\n");
    for (int i = 0; i < total_questions; i++) {
        float rate = 0.0;
        if (questions[i].times_asked > 0)
            rate = ((float)questions[i].times_correct / questions[i].times_asked) * 100;

        printf("ID %d: %s | Сложность: %s | Категория: %s | Отвечено верно: %.2f%% (%d/%d)\n",
               questions[i].id, questions[i].question, questions[i].difficulty,
               questions[i].category, rate, questions[i].times_correct, questions[i].times_asked);
    }
}
