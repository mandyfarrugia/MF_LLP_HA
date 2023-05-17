#pragma once

typedef struct questionSet {
    unsigned int questionNumber;
    char question[400];
    char answer[400];
} questionSet;

typedef questionSet** questionPtr;

typedef struct questionsCollection {
    questionPtr list;
    unsigned int size;
    unsigned int max;
} questionsCollection;

questionsCollection initQuestions(unsigned int max);
void viewQuestion(questionSet question);
void viewQuestions(questionsCollection questions);
void askQuestion(questionsCollection questions);
unsigned int getFirstNullLocation(questionsCollection* questions);
_Bool addQuestion(questionsCollection* questions, questionSet* question);
void deleteQuestions(questionsCollection* questions, unsigned int questionNumber);

questionsCollection initQuestions(unsigned int max) {
    questionsCollection questions;
    questions.list = (questionPtr)malloc(max * sizeof(questionSet));
    questions.size = 0;
    questions.max = max;

    for(unsigned int index = 0; index < questions.max; index++) {
        questions.list[index] = NULL;
    }

    return questions;
}

_Bool isEmpty(questionsCollection questions) {
    return !questions.size;
}

void viewQuestion(questionSet question) {
    printf("Question number: %d\nQuestion: %s\nAnswer: %s\n\n", question.questionNumber, question.question, question.answer);
}

void viewQuestions(questionsCollection questions) {
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

_Bool addQuestion(questionsCollection* questions, questionSet* question) {
    if((*questions).size == (*questions).max) {
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
    }

    unsigned int firstNullLocation = getFirstNullLocation(questions);
    unsigned int nextAvailableSpace = (*questions).size;

    unsigned int location = firstNullLocation ? firstNullLocation : nextAvailableSpace;
    (*questions).list[location] = question;
    (*questions).size++;

    return 1;
}

void deleteQuestions(questionsCollection* questions, unsigned int questionNumber) {
    for(unsigned int index = 0; index < (*questions).size; index++) {
        if((*questions).list[index] != NULL) {
            questionSet* questionByQuestionNumber = (*questions).list[index]; 

            if((*questionByQuestionNumber).questionNumber == questionNumber) {
                free((*questions).list[index]);
                (*questions).list[index] = NULL;
                (*questions).size--;
            }
        }
    }
}