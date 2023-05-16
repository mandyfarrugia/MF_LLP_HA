#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "questions.h"

void viewQuestion(questionSet question) {
    printf("Question number: %d\nQuestion: %s\nAnswer: %s\n\n", question.questionNumber, question.question, question.answer);
}

void viewQuestions(questionsCollection questions) {
    for(unsigned int index = 0; index < questions.max; index++) {
        if(questions.list[index] != NULL) {
            questionSet* question = questions.list[index];
            viewQuestion(*question);
        }
    }
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

int main(void) {
    questionsCollection questions = initQuestions(10);
    printf("Your collection currently has %d questions.\n\n", questions.max);

    questionSet* question1 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question1).questionNumber = 1;
    strcpy((*question1).question, "Why is the null character used in C?");
    strcpy((*question1).answer, "To render a valid C string.");

    questionSet* question2 = (questionSet*)malloc(1 * sizeof(questionSet));
    (*question2).questionNumber = 2;
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

    //questionSet randomQuestion = askQuestion(questions);
    //printf("Question: %s\nAnswer: %s\n", randomQuestion.question, randomQuestion.answer);
    askQuestion(questions);

    return 0;
}