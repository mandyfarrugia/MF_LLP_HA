#pragma once

typedef struct questionSet {
    char question[100];
    char answer[100];
} questionSet;

typedef questionSet** question;

typedef struct questions {
    question list;
    unsigned int size;
    unsigned int max;
} questions;

questions initQuestions(unsigned int max) {
    questions questionsCollection;
    questionsCollection.list = (question)malloc(max * sizeof(questionSet));
    questionsCollection.size = 0;
    questionsCollection.max = max;
    return questionsCollection;
}