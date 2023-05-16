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