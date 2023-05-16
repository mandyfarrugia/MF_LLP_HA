#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "questions.h"

void checkIfElementIsNull(questionsCollection questions) {
    for(unsigned int index = 0; index < questions.max; index++) {
        printf("%d - %d\n", index, questions.list[index] == NULL);
    }
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

    printf("%d\n", getFirstNullLocation(&questions));

    viewQuestions(questions);

    deleteQuestions(&questions, 3);

    viewQuestions(questions);

    deleteQuestions(&questions, 4);

    checkIfElementIsNull(questions);

    addQuestion(&questions, question5);

    viewQuestions(questions);

    askQuestion(questions);

    return 0;
}