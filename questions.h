#pragma once

typedef struct questionSet {
    char question[100];
    char answer[100];
} questionSet;

typedef questionSet** question;

typedef struct questionsCollection {
    question list;
    unsigned int size;
    unsigned int max;
} questionsCollection;

questionsCollection initQuestions(unsigned int max);
_Bool addQuestion(questionsCollection* questions, questionSet question);
void deleteQuestion(questionsCollection* questions, unsigned int index);

questionsCollection initQuestions(unsigned int max) {
    questionsCollection questions;
    questions.list = (question)malloc(max * sizeof(questionSet));
    questions.size = 0;
    questions.max = max;
    return questions;
}

_Bool addQuestion(questionsCollection* questions, questionSet question) {
    return 1;
}

void deleteQuestion(questionsCollection* questions, unsigned int index) {
    if(index < 0 || index > ((*questions).max) - 1) {
        printf("Index %d is outside the bounds of the array!\n", index);
        return;
    }

    if((*questions).list[index] == NULL) {
        printf("Cannot delete at a position which is already null!");
        return;
    } else {
        free((*questions).list[index]);
        (*questions).list[index] = NULL;
    }
}