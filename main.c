#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "questions.h"

unsigned int getFirstNullLocation(questionsCollection questions) {
    _Bool isFirstNullLocationLocated = 0;
    unsigned int index = 0;
    unsigned int firstNullLocationIndex = 0;

    while(!isFirstNullLocationLocated && index < questions.max) {
        if(questions.list[index] == NULL) {
            firstNullLocationIndex = index;
            isFirstNullLocationLocated = 1;
        }

        index++;
    }

    return firstNullLocationIndex;
}

void checkIfElementIsNull(questionsCollection questions) {
    for(unsigned int index = 0; index < questions.max; index++) {
        printf("%d - %d\n", index, questions.list[index] == NULL);
    }
}

void askQuestion(questionsCollection questions) {
    time_t t;
    srand(time(&t));

    unsigned int randomIndex = rand() % questions.max;
    printf("%d\n", randomIndex);

    if(questions.list[randomIndex] != NULL) {
        questionSet questionAtRandomPosition = *(questions.list[randomIndex]);
        printf("Index: %d\nQuestion: %s\nAnswer: %s\n", randomIndex, questionAtRandomPosition.question, questionAtRandomPosition.answer);
    } else {
        printf("No question at index %d.\n", randomIndex);
    }
}

_Bool addQuestion(questionsCollection* questions, questionSet* question) {
    if((*questions).size == (*questions).max) {
        return 0;
    }

    unsigned int nextAvailableSpace = (*questions).size;
    unsigned int firstNullLocation = getFirstNullLocation(questions);

    unsigned int location = firstNullLocation ? firstNullLocation : nextAvailableSpace;
    printf("%d\n", location);

    return 1;
}

int main(void) {
    questionsCollection questions = initQuestions(5);
    printf("Your collection currently has %d questions.\n\n", questions.max);

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

    addQuestion(&questions, question1);
    addQuestion(&questions, question2);
    addQuestion(&questions, question3);
    addQuestion(&questions, question4);
    addQuestion(&questions, question5);
    
    viewQuestions(questions);

    return 0;
}