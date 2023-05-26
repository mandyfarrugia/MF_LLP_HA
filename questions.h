#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct questionSet {
    unsigned int questionNumber;
    char question[400];
} questionSet;

typedef questionSet** questionPtr;

typedef struct questionsCollection {
    questionPtr list;
    unsigned int size;
    unsigned int max;
} questionsCollection;

questionsCollection initialiseCollection();
_Bool isEmpty(questionsCollection questions);
void viewQuestion(questionSet question);
void viewQuestions(questionsCollection questions);
void askQuestion(questionsCollection questions);
unsigned int getFirstNullLocation(questionsCollection* questions);
_Bool addQuestion(questionsCollection* questions, questionSet question);
_Bool resizeQuestionsCollection(questionsCollection* questions);
_Bool deleteQuestion(questionsCollection* questions, unsigned int questionNumber);
_Bool writeQuestionsToBinaryFile(questionsCollection questions, FILE* file);
unsigned int readQuestionsFromBinaryFile(questionsCollection* questions, FILE* file);
void shuffleQuestions(questionsCollection* questions);
void destroyCollection(questionsCollection* questions);
void flushBuffer();

questionsCollection initialiseCollection() {
    questionsCollection questions;
    questions.max = 10;
    questions.list = (questionPtr)malloc((questions.max) * sizeof(questionSet));

    for(unsigned int index = 0; index < questions.max; index++) {
        questions.list[index] = NULL;
    }

    questions.size = 0;
    return questions;
}

_Bool isEmpty(questionsCollection questions) {
    return !questions.size;
}

void viewQuestion(questionSet question) {
    printf("\nQuestion number: %u\nQuestion: %s\n", question.questionNumber, question.question);
}

void viewQuestions(questionsCollection questions) {
    if(isEmpty(questions))
        return;

    for(unsigned int index = 0; index < questions.size; index++) {
        if(questions.list[index] != NULL) {
            questionSet* question = questions.list[index];
            viewQuestion(*question);
        }
    }
}

void askQuestion(questionsCollection questions) {
    time_t t;
    srand(time(&t));

    unsigned int randomIndex = rand() % questions.max;

    if(questions.list[randomIndex] != NULL) {
        questionSet questionAtRandomPosition = *(questions.list[randomIndex]);
        viewQuestion(questionAtRandomPosition);
    } else {
        printf("No question at index %d.\n", randomIndex);
    }
}

unsigned int getFirstNullLocation(questionsCollection* questions) {
    _Bool isFirstNullLocationLocated = 0;
    unsigned int index = 0;
    unsigned int firstNullLocationIndex = 0;

    do {
        if((*questions).list[index] == NULL) {
            firstNullLocationIndex = index;
            isFirstNullLocationLocated = 1;
        }

        index++;
    } while(!isFirstNullLocationLocated && index < (*questions).max);

    return firstNullLocationIndex;
}

_Bool addQuestion(questionsCollection* questions, questionSet question) {
    if((*questions).size == (*questions).max) {
        resizeQuestionsCollection(questions);
    }

    unsigned int firstNullLocation = getFirstNullLocation(questions);
    unsigned int nextAvailableSpace = (*questions).size;

    unsigned int location = firstNullLocation ? firstNullLocation : nextAvailableSpace;
    (*questions).list[location] = (questionSet*)malloc(1 * sizeof(questionSet));
    *((*questions).list[location]) = question;
    (*questions).size++;

    return 1;
}

_Bool resizeQuestionsCollection(questionsCollection* questions) {
    unsigned int doubledMax = (*questions).max * 2;
    (*questions).list = (questionPtr)realloc((*questions).list, doubledMax * sizeof(questionSet));

    if((*questions).list) {
        for(unsigned int index = (*questions).size; index < doubledMax; index++) {
            (*questions).list[index] = NULL;
        }

        (*questions).max = doubledMax;
    }
    else {
        return 0;
    }

    return (*questions).list != NULL;
}

_Bool deleteQuestion(questionsCollection* questions, unsigned int questionNumber) {
    _Bool isQuestionDeleted = 0;

    for(unsigned int index = 0; index < (*questions).size; index++) {
        if((*questions).list[index] != NULL) {
            questionSet* questionByQuestionNumber = (*questions).list[index];

            if((*questionByQuestionNumber).questionNumber == questionNumber) {
                free((*questions).list[index]);
                (*questions).list[index] = NULL;
                (*questions).size--;
                isQuestionDeleted = 1;
            }
        }
    }

    return isQuestionDeleted;
}

_Bool writeQuestionsToBinaryFile(questionsCollection questions, FILE* file) {
    for (unsigned int index = 0; index < questions.size; index++) {
        if(questions.list[index] != NULL) {
            questionSet element = *(questions).list[index];
            fwrite(&element, sizeof(element), 1, file);
        }
    }

    fclose(file);

    return file != NULL;
}

unsigned int readQuestionsFromBinaryFile(questionsCollection* questions, FILE* file) {
    unsigned int questionsRead = 0;

    questionSet questionToRead;
    while(fread(&questionToRead, sizeof(questionToRead), 1, file)) {
        addQuestion(questions, questionToRead);
        questionsRead++;
    }

    (*questions).max = questionsRead;
    (*questions).list = (questionPtr)realloc((*questions).list, ((*questions).max) * sizeof(questionSet));

    fclose(file);

    return (*questions).size;
}

void shuffleQuestions(questionsCollection* questions) {
    if(isEmpty(*(questions))) {
        printf("Cannot shuffle an empty collection!");
        return;
    }

    time_t t;
    srand(time(&t));

    questionSet* temporaryQuestion;
    for(unsigned int i = (*questions).size - 1; i > 0; i--) {
        unsigned int j = rand() % (i + 1);
        temporaryQuestion = (*questions).list[j];
        (*questions).list[j] = (*questions).list[i];
        (*questions).list[i] = temporaryQuestion;
    }
}

void destroyCollection(questionsCollection* questions) {
    for (unsigned int index = 0; index < (*questions).max; index++) {
        free((*questions).list[index]);
    }

    free((*questions).list);
}

void flushBuffer() {
    char character;
    while((character = getchar()) != '\n' && character != EOF);
}