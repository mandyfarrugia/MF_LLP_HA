#pragma once

typedef struct questionSet {
    unsigned int questionNumber;
    char question[400];
    char answer[400];
} questionSet;

typedef questionSet** question;

typedef struct questionsCollection {
    question list;
    unsigned int size;
    unsigned int max;
} questionsCollection;

questionsCollection initQuestions(unsigned int max);
void viewQuestion(questionSet question);
void viewQuestions(questionsCollection questions);
void askQuestion(questionsCollection questions);
//_Bool addQuestion(questionsCollection* questions, questionSet question);
void deleteQuestions(questionsCollection* questions, unsigned int questionNumber);

questionsCollection initQuestions(unsigned int max) {
    questionsCollection questions;
    questions.list = (question)malloc(max * sizeof(questionSet));
    questions.size = 0;
    questions.max = max;
    return questions;
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
    printf("%d\n", randomIndex);

    if(questions.list[randomIndex] != NULL) {
        questionSet questionAtRandomPosition = *(questions.list[randomIndex]);
        printf("Index: %d\nQuestion: %s\nAnswer: %s\n", randomIndex, questionAtRandomPosition.question, questionAtRandomPosition.answer);
    } else {
        printf("No question at index %d.\n", randomIndex);
    }
}

// _Bool addQuestion(questionsCollection* questions, questionSet question) {
//     return 1;
// }

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