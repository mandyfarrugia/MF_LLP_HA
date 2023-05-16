#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "questions.h"

void viewQuestions(questionsCollection questions) {
    for(unsigned int index = 0; index < questions.max; index++) {
        if(questions.list[index] != NULL) {
            questionSet* question = questions.list[index];
            questionSet current = *question;
            printf("Question: %s\nAnswer: %s\n\n", current.question, current.answer);
        }
    }
}

int main(void) {
    questionsCollection questions = initQuestions(10);
    printf("Your collection currently has %d questions.\n\n", questions.max);

    questionSet* question1 = (questionSet*)malloc(1 * sizeof(questionSet));
    strcpy((*question1).question, "Why is the null character used in C?");
    strcpy((*question1).answer, "To render a valid C string.");

    questionSet* question2 = (questionSet*)malloc(1 * sizeof(questionSet));
    strcpy((*question2).question, "What is the advantage of unsigned int in C?");
    strcpy((*question2).answer, "Offers a wider range of positive values.");
    
    /* 
        printf("Question: %s\nAnswer: %s\n", (*question).question, (*question).answer);

        questions.list[0] = question;
        printf("%s", questions.list[0]->question); */

    questions.list[0] = question1;
    questions.list[1] = question2;
    viewQuestions(questions);

    return 0;
}