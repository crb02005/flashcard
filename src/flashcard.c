#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 256
#define MAX_QUESTIONS 100

typedef struct {
    char question[MAX_LINE_LENGTH];
    char answer[MAX_LINE_LENGTH];
    char user_answer[MAX_LINE_LENGTH];
    int question_number;
} QA;

void shuffle(QA *array, int n) {
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            QA temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    QA questions[MAX_QUESTIONS];

    int question_count = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && question_count < MAX_QUESTIONS) {
        char *token = strtok(line, "|");
        if (token) {
            strncpy(questions[question_count].question, token, MAX_LINE_LENGTH);
            token = strtok(NULL, "\n");
            if (token) {
                strncpy(questions[question_count].answer, token, MAX_LINE_LENGTH);
                strncpy(questions[question_count].user_answer, "", MAX_LINE_LENGTH);
                questions[question_count].question_number = question_count;
                question_count++;
            }
        }
    }
    fclose(file);

    srand(time(NULL));

    int correct_count, incorrect_count;
    char user_answer[MAX_LINE_LENGTH];
    QA incorrect_questions[MAX_QUESTIONS];

    do {


        shuffle(questions, question_count);
        correct_count = 0;
        incorrect_count = 0;

        for (int i = 0; i < question_count; i++) {
            if(strcmp(questions[i].user_answer, questions[i].answer) != 0){
                printf("Question: %s\n", questions[i].question);
                printf("Your answer: ");
                fgets(user_answer, sizeof(user_answer), stdin);
                user_answer[strcspn(user_answer, "\n")] = 0; // Remove newline character

                strncpy(questions[i].user_answer, user_answer, MAX_LINE_LENGTH);

                if (strcmp(user_answer, questions[i].answer) == 0) {
                    correct_count++;
                } else {
                    incorrect_questions[incorrect_count++] = questions[i];
                }
            }
        }

        printf("\nResults:\n");
        for (int i = 0; i < question_count; i++) {
            printf("Q: %s\n", questions[i].question);
            printf("A: %s\n", questions[i].answer);
            printf("Your answer: %s\n", questions[i].user_answer);
            if (strcmp(questions[i].user_answer, questions[i].answer) == 0) {
                printf("Correct!\n");
            } else {
                printf("Incorrect!\n");
            }
            printf("\n");
        }

        printf("Total correct: %d\n", correct_count);
        printf("Total incorrect: %d\n", incorrect_count);

        if (incorrect_count > 0) {
            memcpy(questions, incorrect_questions, sizeof(QA) * incorrect_count);
            question_count = incorrect_count;
        }
    } while (incorrect_count > 0);

    printf("All answers correct! Exiting...\n");
    return 0;
}