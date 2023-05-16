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

void checkIfElementIsNull(questionsCollection questions) {
    for(unsigned int index = 0; index < questions.max; index++) {
        printf("%d - %d\n", index, questions.list[index] == NULL);
    }
}

void deleteQuestion(questionsCollection* questions, unsigned int index) {
    if(index < 0 || index > ((*questions).max) - 1) {
        printf("Index %d is outside the bounds of the array!\n", index);
        return;
    }

    if((*questions).list[index] != NULL) {
        free((*questions).list[index]);
        (*questions).list[index] = NULL;
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

    checkIfElementIsNull(questions);

    deleteQuestion(&questions, -1);
    deleteQuestion(&questions, 10);

    deleteQuestion(&questions, 1);
    checkIfElementIsNull(questions);

    return 0;
}