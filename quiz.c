#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char question[100];
    char options[4][50];
    int correctOption;
} Question;

typedef struct {
    char category[50];
    Question questions[10];
    int numQuestions;
} Quiz;

// Function to create a new quiz
Quiz createQuiz(char category[]) {
    Quiz newQuiz;
    strcpy(newQuiz.category, category);
    newQuiz.numQuestions = 0;
    return newQuiz;
}

// Function to add a question to a quiz
void addQuestion(Quiz *quiz, char question[], char options[][50], int correctOption) {
    if (quiz->numQuestions < 10) {
        Question newQuestion;
        strcpy(newQuestion.question, question);
        for (int i = 0; i < 4; i++) {
            strcpy(newQuestion.options[i], options[i]);
        }
        newQuestion.correctOption = correctOption;
        quiz->questions[quiz->numQuestions] = newQuestion;
        quiz->numQuestions++;
    }
    else {
        printf("Quiz is full. Cannot add more questions.\n");
    }
}

// Function to take a quiz
void takeQuiz(Quiz quiz) {
    printf("Welcome to the %s Quiz!\n", quiz.category);
    printf("Answer the following questions:\n");

    int score = 0;

    for (int i = 0; i < quiz.numQuestions; i++) {
        printf("\nQuestion %d: %s\n", i + 1, quiz.questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, quiz.questions[i].options[j]);
        }

        int answer;
        printf("Your answer (1-4): ");
        scanf("%d", &answer);

        if (answer >= 1 && answer <= 4) {
            if (answer == quiz.questions[i].correctOption) {
                printf("Correct!\n");
                score++;
            }
            else {
                printf("Incorrect. The correct answer was option %d.\n", quiz.questions[i].correctOption);
            }
        }
        else {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            i--; // Repeat the current question
        }
    }

    printf("\nQuiz completed! Your score: %d / %d\n", score, quiz.numQuestions);
}

// Function to save a quiz to a file
void saveQuizToFile(Quiz quiz) {
    char fileName[100];
    printf("Enter a filename to save the quiz (include the .quiz extension): ");
    scanf("%s", fileName);

    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(&quiz, sizeof(Quiz), 1, file);
    fclose(file);

    printf("Quiz saved to %s.\n", fileName);
}

// Function to load a quiz from a file
Quiz loadQuizFromFile(char fileName[]) {
    FILE *file = fopen(fileName, "rb");
    Quiz loadedQuiz;
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        loadedQuiz.numQuestions = 0; // Indicate failure to load
        return loadedQuiz;
    }

    fread(&loadedQuiz, sizeof(Quiz), 1, file);
    fclose(file);

    printf("Quiz loaded from %s.\n", fileName);
    return loadedQuiz;
}

int main() {
    int choice;
    Quiz quizzes[10]; // Array to hold multiple quizzes
    int numQuizzes = 0;

    while (1) {
        printf("Welcome to the Quiz Application!\n");
        printf("1. Create a new quiz\n");
        printf("2. Take a quiz\n");
        printf("3. Save a quiz to a file\n");
        printf("4. Load a quiz from a file\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                 if (numQuizzes < 10) {
                    char category[50];
                    printf("Enter the category of the new quiz: ");
                    scanf("%s", category);
                    quizzes[numQuizzes] = createQuiz(category);

                    int numQuestions;
                    printf("Enter the number of questions for the quiz: ");
                    scanf("%d", &numQuestions);

                    for (int i = 0; i < numQuestions; i++) {
                        char question[100];
                        printf("Enter question %d: ", i + 1);
                        scanf(" %[^\n]", question);

                        char options[4][50];
                        for (int j = 0; j < 4; j++) {
                            printf("Enter option %d: ", j + 1);
                            scanf(" %[^\n]", options[j]);
                        }

                        int correctOption;
                        printf("Enter the correct option (1-4): ");
                        scanf("%d", &correctOption);

                        addQuestion(&quizzes[numQuizzes], question, options, correctOption);
                    }

                    printf("Quiz '%s' created.\n", category);
                    numQuizzes++;
                }
                else {
                    printf("Maximum number of quizzes reached.\n");
                }
                break;
            case 2:
                if (numQuizzes > 0) {
                    printf("Select a quiz to take:\n");
                    for (int i = 0; i < numQuizzes; i++) {
                        printf("%d. %s\n", i + 1, quizzes[i].category);
                    }
                    int quizChoice;
                    printf("Enter the number of the quiz: ");
                    scanf("%d", &quizChoice);
                    if (quizChoice >= 1 && quizChoice <= numQuizzes) {
                        takeQuiz(quizzes[quizChoice - 1]);
                    }
                    else {
                        printf("Invalid quiz number.\n");
                    }
                }
                else {
                    printf("No quizzes available.\n");
                }
                break;
            case 3:
                if (numQuizzes > 0) {
                    printf("Select a quiz to save:\n");
                    for (int i = 0; i < numQuizzes; i++) {
                        printf("%d. %s\n", i + 1, quizzes[i].category);
                    }
                    int saveChoice;
                    printf("Enter the number of the quiz: ");
                    scanf("%d", &saveChoice);
                    if (saveChoice >= 1 && saveChoice <= numQuizzes) {
                        saveQuizToFile(quizzes[saveChoice - 1]);
                    }
                    else {
                        printf("Invalid quiz number.\n");
                    }
                }
                else {
                    printf("No quizzes available.\n");
                }
                break;
            case 4:
                printf("Enter the filename to load the quiz from: ");
                char fileName[100];
                scanf("%s", fileName);
                Quiz loadedQuiz = loadQuizFromFile(fileName);
                if (loadedQuiz.numQuestions > 0) {
                    quizzes[numQuizzes] = loadedQuiz;
                    printf("Quiz loaded.\n");
                    numQuizzes++;
                }
                break;
            case 5:
                printf("Exiting the Quiz Application.\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                break;
        }
    }

    return 0;
}
