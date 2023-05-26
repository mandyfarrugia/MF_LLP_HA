#define _CRT_SECURE_NO_WARNINGS

#include "interview.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_OPTIONS 6
#define MAX_OPTIONS_CHARS 30

void getNumericInput(char* prompt, unsigned int* numericInput, int check) {
    do {
        printf("\n%s: ", prompt);
        check = scanf("%u", numericInput);

        if (check < 1) {
            printf("Invalid input detected!\n");
            flushBuffer();
        }
    } while (check < 1);
}

void prepareAddQuestion(questionsCollection* questions, int check) {
    questionSet questionToBeAdded;

    do {
        flushBuffer();

        printf("\nEnter the number to be associated with the question: ");
        check = scanf("%u", &questionToBeAdded.questionNumber);

        if (check < 1) {
            printf("Invalid input detected!\n");
        } else if(checkIfQuestionNumberExists(*questions, questionToBeAdded.questionNumber)) {
            printf("There is already a question associated with this number!\n");
        }
    } while (check < 1 || checkIfQuestionNumberExists(*questions, questionToBeAdded.questionNumber));

    do {
        flushBuffer();
        printf("\nEnter a string: ");
        scanf("%399[^\n]", questionToBeAdded.question);
        questionToBeAdded.question[strlen(questionToBeAdded.question)] = '\0';

        if (!strlen(questionToBeAdded.question)) {
            printf("Please enter a valid question!\n");
        } else if(checkIfQuestionExists(*questions, questionToBeAdded.question)) {
            printf("Question already exists!\n");
        }
    } while (!strlen(questionToBeAdded.question) || checkIfQuestionExists(*questions, questionToBeAdded.question));

    _Bool questionAddedSuccessfully = addQuestion(questions, questionToBeAdded);
    printf("\n%s\n", questionAddedSuccessfully ? "Question added successfully!" : "Unable to add question!");
}

void prepareDeleteQuestion(questionsCollection* questions, int check) {
    unsigned int questionNumberToDelete = 0;
    getNumericInput("Enter the number associated with the question you want to delete", &questionNumberToDelete, check);
    _Bool questionDeletedSuccessfully = deleteQuestion(questions, questionNumberToDelete);
    printf("%s\n", questionDeletedSuccessfully ? "Question deleted successfully!" : "No question associated with this number can be found!");
}

void confirmBeforeExit(questionsCollection questions, FILE* file, _Bool* isProgramRunningFlag) {
    char exitConfirmation;

    do {
        flushBuffer();

        printf("\nAre you sure you want to exit? (Y/N): ");
        scanf("%c", &exitConfirmation);
        exitConfirmation = tolower(exitConfirmation);

        if(exitConfirmation != 'y' && exitConfirmation != 'n') {
            printf("Invalid input detected!\n");
        }
    } while(exitConfirmation != 'y' && exitConfirmation != 'n');

    if(exitConfirmation == 'y') {
        file = fopen("questions.dat", "wb");
        writeQuestionsToBinaryFile(questions, file);
        destroyCollection(&questions);
        *isProgramRunningFlag = 0;
    }
}

char** getMenuOptions() {
    char staticOptions[MENU_OPTIONS][MAX_OPTIONS_CHARS] = {"View all questions", "Ask a question", "Add a question", "Delete a question", "Shuffle questions", "Save to file and exit"};
    char** menuOptions = (char**)malloc(MENU_OPTIONS * sizeof(char*));

    if(menuOptions) {
        for(unsigned int index = 0; index < MENU_OPTIONS; index++) {
            size_t lengthOfOption = strlen(staticOptions[index]);
            menuOptions[index] = (char*)malloc((lengthOfOption + 1) * sizeof(char));
            strcpy(menuOptions[index], staticOptions[index]);
        }
    }

    return menuOptions;
}

void displayMenu(char** menuOptions) {
    if(!menuOptions)
        return;

    unsigned int index = 0;

    char title[20] = "Interview questions";
    printf("%s\n", title);
    for(index = 0; index < strlen(title); index++) {
        printf("=");
        if(index == strlen(title) - 1)
            printf("\n");
    }

    for(index = 0; index < MENU_OPTIONS; index++)
        printf("%u. %s\n", (index + 1), *(menuOptions + index));
}

void addDummyStruct(questionsCollection* questions, unsigned int questionNumber, char* question) {
    questionSet dummyQuestion;
    dummyQuestion.questionNumber = questionNumber;
    strcpy(dummyQuestion.question, question);
    addQuestion(questions, dummyQuestion);
}

void initialiseCollectionFromDummyStructs(questionsCollection* questions) {
    char interviewQuestions[10][90] = {
        "Why is C called a mid-level programming language?",
        "What are the features of the C language?",
        "What is the use of printf() and scanf() functions?",
        "What is a built-in function in C?",
        "What is a preprocessor?",
        "How can a string be converted to a number?",
        "How can a number be converted to a string?",
        "What is recursion in C?",
        "Why doesn't C support function overloading?",
        "What is the difference between global int and static int declaration?"
    };

    size_t amountOfInterviewQuestions = sizeof(interviewQuestions) / sizeof(interviewQuestions[0]);
    for (unsigned int index = 0; index < amountOfInterviewQuestions; index++) {
        addDummyStruct(questions, (index + 1), interviewQuestions[index]);
    }
}

int main() {
    questionsCollection questions = initialiseCollection();

    FILE* file = fopen("questions.dat", "rb");
    if(!file) {
        initialiseCollectionFromDummyStructs(&questions);
    } else {
        unsigned int questionsReadFromFile = readQuestionsFromBinaryFile(&questions, file);

        if(!questionsReadFromFile) {
            initialiseCollectionFromDummyStructs(&questions);
        }
    }

    _Bool isProgramRunning = 1;
    unsigned int choice = 0;

    do {
        int check = 0;

        char** menuOptions = getMenuOptions();
        displayMenu(menuOptions);

        getNumericInput("Enter choice", &choice, check);

        switch(choice) {
            case 1:
                viewQuestions(questions);
                break;
            case 2:
                askQuestion(questions);
                break;
            case 3:
                prepareAddQuestion(&questions, check);
                break;
            case 4:
                prepareDeleteQuestion(&questions, check);
                break;
            case 5:
                shuffleQuestions(&questions);
                break;
            case 6:
                confirmBeforeExit(questions, file, &isProgramRunning);
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }

        printf("\n");
    } while(choice != 6 || isProgramRunning);

    return 0;
}