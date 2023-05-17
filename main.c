#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "questions.h"

void getMenuOptions() {
    char staticOptions[6][30] = {"View all questions", "Ask a question", "Add a question", "Delete a question", "Shuffle questions", "Save to file and exit"};
    size_t optionsCount =  sizeof(staticOptions) / sizeof(staticOptions[0]);
    char** menuOptions = (char**)malloc(optionsCount * sizeof(char*));

    for(unsigned int index = 0; index < optionsCount; index++) {
        size_t lengthOfOption = strlen(staticOptions[index]);
        menuOptions[index] = (char*)malloc(lengthOfOption * sizeof(char));
        strcpy(menuOptions[index], staticOptions[index]);
    }

    for(unsigned int index = 0; index < optionsCount; index++) {
        printf("%s\n", menuOptions[index]);
    }
}

int main(void) {
    questionsCollection questions = initQuestions(5);
    printf("Your collection currently supports up to %d questions.\n\n", questions.max);
    printf("You have %d questions.\n\n", questions.size);

    questionSet* question1 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question1).questionNumber = 1;
    strcpy((*question1).question, "Why is the null character used?");
    strcpy((*question1).answer, "To render a valid C string.");
    
    questionSet* question2 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question2).questionNumber = 2;
    strcpy((*question2).question, "What is the advantage of unsigned int in C?");
    strcpy((*question2).answer, "Offers a wider range of positive values.");

    questionSet* question3 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question3).questionNumber = 3;
    strcpy((*question3).question, "What is buffer overflow? How can it be avoided when working with C strings?");
    strcpy((*question3).answer, "When you try to store more characters than your string variable can hold. Mitigations include field width (one less than the maximum size to allow space for null character) before the format specifier, or flush the buffer.");

    questionSet* question4 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question4).questionNumber = 4;
    strcpy((*question4).question, "What are the four stages of compilation in C in the correct order?");
    strcpy((*question4).answer, "Preprocessing, compilation, assembly, and linking.");

    questionSet* question5 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question5).questionNumber = 5;
    strcpy((*question5).question, "What does the preprocessor do?");
    strcpy((*question5).answer, "Removes comments and whitespace, expands macros and included header files, and obeys preprocessor directives.");

    printf("Is collection empty: %d\n", isEmpty(questions));

    addQuestion(&questions, question1);
    addQuestion(&questions, question2);
    addQuestion(&questions, question3);
    addQuestion(&questions, question4);
    addQuestion(&questions, question5);

    printf("Is collection empty: %d\n", isEmpty(questions));

    questionSet* question6 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question6).questionNumber = 6;
    strcpy((*question6).question, "Does a static array allow growing and shrinking?");
    strcpy((*question6).answer, "No, a static array is of a fixed size.");

    // addQuestion(&questions, question6);

    // printf("Your collection currently supports up to %d questions.\n\n", questions.max);
    // printf("You have %d questions.\n\n", questions.size);

    // addQuestion(&questions, question6);
    // addQuestion(&questions, question6);
    // addQuestion(&questions, question6);
    // addQuestion(&questions, question6);

    // printf("Your collection currently supports up to %d questions.\n\n", questions.max);
    // printf("You have %d questions.\n\n", questions.size);

    // addQuestion(&questions, question6);

    // printf("Your collection currently supports up to %d questions.\n\n", questions.max);
    // printf("You have %d questions.\n\n", questions.size);

    // viewQuestions(questions);

    // deleteQuestions(&questions, 5);

    // viewQuestions(questions);

    // printf("Your collection currently supports up to %d questions.\n\n", questions.max);
    // printf("You have %d questions.\n\n", questions.size);

    getMenuOptions();

    return 0;
}