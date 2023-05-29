#pragma once //This preprocessor directive forces this header file to be declared once and only once. 

#include <stdio.h> //Include stdio.h to use fopen(), scanf(), and printf() for file handling, input, and output operations.
#include <stdlib.h> //Include stdlib.h to use malloc(), realloc(), and free() for memory allocation/re-allocation/deallocation.
#include <string.h> //Include string.h to use strcpy() and strlen() to handle strings and assign data to them.
#include <time.h> //Include time.h to randomise the seed when shuffling the list and fetching a random question.

/* The struct should be made up of the question number which uniquely identifies each question, as well as the question itself. 
   The question will accept up to 150 characters. When using from scanf(), limit up to 149 characters to allow space for the null character. */
typedef struct questionSet {
    unsigned int questionNumber; //unsigned int was used since storing negative numbers is unnecessary for this scenario. 
    char question[150];
} questionSet; //typedef was used to avoid having to use the fully qualified name struct questionSet everytime.

typedef questionSet** questionPtr; //Using typedef, a pointer to pointer to questionSet can be referred to by questionPtr.

//This struct represents the collection and any information related to it.
typedef struct questionsCollection {
    /* A double pointer to questionSet representing the list.
       It was decided to use a double pointer-based implementation in lieu of a single pointer counterpart to allow deletions of a specific element by the question number.
       Although a single pointer-based implementation would have sufficed, the elements would be deleted virtually.*/
    questionPtr list;
    unsigned int size; //Represents the amount of questions currently stored in the list.
    unsigned int max; //Represents the maximum allowed questions in the list.
} questionsCollection; //typedef was used to avoid having to use the fully qualified name struct questionCollection everytime.

//The following are function declarations of the functions related to the manipulation of the list of questions.

questionsCollection initialiseCollection(); //Start by initialising an empty collection allowing up to 10 questions.
_Bool isEmpty(questionsCollection questions); //Check whether the list is empty by comparing the size with 0. 
void viewQuestion(questionSet question); //View a single question.
void viewQuestions(questionsCollection questions); //Go through the list and view all questions.
void askQuestion(questionsCollection questions); //Present the user with a random question.
unsigned int getFirstNullLocation(questionsCollection questions); //Get the first null location in the list.
_Bool resizeQuestionsCollection(questionsCollection* questions); //Resize the list using realloc() if the need arises.
_Bool addQuestion(questionsCollection* questions, questionSet question); //Add a new question to the list.
_Bool deleteQuestion(questionsCollection* questions, unsigned int questionNumber); //Delete a question by a question number typically supplied by input.
void writeQuestionsToBinaryFile(questionsCollection questions, FILE* file); //Copy the contents of the list to a file.
unsigned int readQuestionsFromBinaryFile(questionsCollection* questions, FILE* file); //Load the contents of a file to the list.
void shuffleQuestions(questionsCollection* questions); //Shuffle the order of the questions in the list.
_Bool checkIfQuestionNumberExists(questionsCollection questions, unsigned int questionNumber); //Check whether there is a question associated with a number typically supplied as input.
_Bool checkIfQuestionExists(questionsCollection questions, char* question); //Check whether a specific question already exists in the list.
void destroyCollection(questionsCollection* questions); //Free memory allocated to the list.
void flushBuffer(); //Flush the buffer so as not to affect the next scanf() especially in the case of previous erroneous input.

questionsCollection initialiseCollection() {
    questionsCollection questions; //Prepare the collection.
    questions.max = 10; //Since 10 dummy questions will be added, set max to 10.
    questions.list = (questionPtr)malloc((questions.max) * sizeof(questionSet)); //Allocate memory for 10 questionSet structs.

    //Set each child pointer to NULL, no need to allocate memory at this point.
    unsigned int index;
    for(index = 0; index < questions.max; index++) {
        questions.list[index] = NULL;
    }

    questions.size = 0; //Set the size to 0 since an empty collection is being initialised.
    return questions; //Return the initialised collection.
}

/* Check whether the list is empty by comparing question.size with 0 (!(questions.size)).
   Returns 0 if the list is not empty, otherwise 1 if the list is empty.
   questions is not declared as a pointer since the collection will not be modified. */
_Bool isEmpty(questionsCollection questions) {
    return !(questions.size);
}

//Print the contents of a question.
void viewQuestion(questionSet question) {
    printf("Question number: %u\nQuestion: %s\n\n", question.questionNumber, question.question);
}

/* If the list has questions, iterate the list and print the contents of each question.
   questions is not declared as a pointer since the collection will not be modified. */
void viewQuestions(questionsCollection questions) {
    //If the list is empty, warn the user and exit this function.
    if(isEmpty(questions)) {
        printf("Sorry, there are no questions at the moment!\n");
        return;
    }

    //Iterate the list and print contents of questions within. Ignore any locations which are NULL.
    unsigned int index;
    for(index = 0; index < questions.max; index++) {
        if(questions.list[index] != NULL) {
            questionSet* question = questions.list[index];
            viewQuestion(*question);
        }
    }
}

//questions is not declared as a pointer since the collection will not be modified.
void askQuestion(questionsCollection questions) {
    //If the list is empty, warn the user and exit this function.
    if(isEmpty(questions)) {
        printf("Sorry, there are no questions at the moment!\n");
        return;
    }

    time_t t;
    srand(time(&t)); //Seed the random number generator to avoiding generating the same number every time.

    //If the list is not empty, fetch a random question and present it to the user.
    if(!isEmpty(questions)) {
        unsigned int randomIndex;

        do {
            randomIndex = rand() % questions.max;
        } while(questions.list[randomIndex] == NULL); //Keep generating a random number until a non-NULL location is found.

        questionSet questionAtRandomPosition = *(questions.list[randomIndex]);
        viewQuestion(questionAtRandomPosition);
    } else {
        printf("There are no questions!\n");
        return;
    }
}

//questions is not declared as a pointer since the collection will not be modified.
unsigned int getFirstNullLocation(questionsCollection questions) {
    _Bool isFirstNullLocationLocated = 0;
    unsigned int index = 0;
    unsigned int firstNullLocationIndex = 0;

    do {
        if(questions.list[index] == NULL) {
            firstNullLocationIndex = index;
            isFirstNullLocationLocated = 1; //Exit the loop if the first null location is found.
        }

        index++;
    } while(!isFirstNullLocationLocated && index < questions.max); //Keep until the first null location or until index reaches one less than the maximum allowed numbers.

    return firstNullLocationIndex;
}

//questions is declared as a pointer since the collection will be modified.
_Bool resizeQuestionsCollection(questionsCollection* questions) {
    unsigned int doubledMax = (*questions).max * 2;

    /* Use realloc() to resize the list to allow twice as much as the currently maximum allowed elements. 
       This is to avoid having to call realloc() frequently. */
    (*questions).list = (questionPtr)realloc((*questions).list, doubledMax * sizeof(questionSet*));

    /* If sufficient memory was allocated, then set the locations starting from the size up to the new size to NULL.
       There is no need to allocate memory to the new locations at this point. */
    if((*questions).list) {
        unsigned int index;
        for(index = (*questions).size; index < doubledMax; index++) {
            (*questions).list[index] = NULL;
        }

        (*questions).max = doubledMax;
    }
    else {
        return 0;
    }

    return (*questions).list != NULL;
}

//questions is declared as a pointer since the collection will be modified.
_Bool addQuestion(questionsCollection* questions, questionSet question) {
    //If there is no room for the new question, resize the list.
    if((*questions).size == (*questions).max) {
        _Bool collectionResizedSuccessfully = resizeQuestionsCollection(questions);
        if(!collectionResizedSuccessfully)
            return 0; //If reallocation fails, return 0 and stop at this point.
    }

    unsigned int firstNullLocation = getFirstNullLocation(*questions);
    unsigned int nextAvailableSpace = (*questions).size;

    //If there is a null location, insert the element at this location. Otherwise, append the question to the list at the next available space (at the end).
    unsigned int location = firstNullLocation ? firstNullLocation : nextAvailableSpace;
    (*questions).list[location] = (questionSet*)malloc(1 * sizeof(questionSet));
    *((*questions).list[location]) = question;
    (*questions).size++;

    return 1;
}

//questions is declared as a pointer since the collection will be modified.
_Bool deleteQuestion(questionsCollection* questions, unsigned int questionNumber) {
    _Bool isQuestionDeleted = 0;

    unsigned int index;
    for(index = 0; index < (*questions).max; index++) {
        //First start by checking whether the location is not null before deleting.
        if((*questions).list[index] != NULL) {
            questionSet* questionByQuestionNumber = (*questions).list[index];
            //If there is a question associated with the number supplied, go ahead and delete it.
            if((*questionByQuestionNumber).questionNumber == questionNumber) {
                free((*questions).list[index]);
                (*questions).list[index] = NULL;
                (*questions).size--;
                isQuestionDeleted = 1;
            }
        }
    }

    return isQuestionDeleted; //Return 0 if a question with the number supplied could not be found, otherwise 1 if the question was deleted successfully.
}

//questions is not declared as a pointer since the collection will not be modified.
void writeQuestionsToBinaryFile(questionsCollection questions, FILE* file) {
    unsigned int index;
    for (index = 0; index < questions.size; index++) {
        //Ignore any null locations.
        if(questions.list[index] != NULL) {
            questionSet element = *(questions).list[index];
            fwrite(&element, sizeof(element), 1, file); //Write each question to the file.
        }
    }

    fclose(file); //Close the file.

    printf("Questions saved to binary file successfully!");
}

//questions is declared as a pointer since the collection will be modified.
unsigned int readQuestionsFromBinaryFile(questionsCollection* questions, FILE* file) {
    unsigned int questionsRead = 0;

    questionSet questionToRead;
    while(fread(&questionToRead, sizeof(questionToRead), 1, file)) {
        addQuestion(questions, questionToRead); //Load each question from the file to the collection.
        questionsRead++;
    }

    (*questions).max = questionsRead;
    (*questions).list = (questionPtr)realloc((*questions).list, ((*questions).max) * sizeof(questionSet)); //Reallocate the list according to the new maximum size.

    fclose(file); //Close the file.

    return (*questions).size; //Return how many questions are present in the list.
}

//questions is declared as a pointer since the collection will be modified.
void shuffleQuestions(questionsCollection* questions) {
    //No sense in shuffling an empty list or a list with only one question.
    if(isEmpty(*questions) || (*questions).size == 1) {
        //Warn the user and then exit the function.
        printf("Cannot shuffle an empty collection or if it only has one question!\n");
        return;
    }

    time_t t;
    srand(time(&t)); //Randomise the seed.

    questionSet* temporaryQuestion;
    unsigned int index;

    //Used the Yisher Yates algorithm from the highest index - 1 to lowest.
    for(index = (*questions).max - 1; index > 0; index--) {
        unsigned int j = rand() % (index + 1); //Generate an index between 0 and index + 1.

        if((*questions).list[index] == NULL)
            index--; //Decrement the index if the current location is NULL.

        //Check that the current locations for index and j are not null before shuffling.
        if((*questions).list[index] != NULL && (*questions).list[j] != NULL) {
            temporaryQuestion = (*questions).list[j];
            (*questions).list[j] = (*questions).list[index];
            (*questions).list[index] = temporaryQuestion;
        }
    }

    printf("Questions have been shuffled successfully.\n");
}

//questions is not declared as a pointer since the collection will not be modified.
_Bool checkIfQuestionNumberExists(questionsCollection questions, unsigned int questionNumber) {
    _Bool doesQuestionNumberExist = 0;
    unsigned int index;

    //Iterate the list and check whether there is a question associated with the question number supplied.
    for(index = 0; index < questions.max; index++) {
        //Ignore null locations.
        if(questions.list[index] != NULL) {
            questionSet currentQuestion = *(questions.list[index]);
            if(currentQuestion.questionNumber == questionNumber) {
                doesQuestionNumberExist = 1;
            }
        }
    }

    return doesQuestionNumberExist; //Return 1 if the question number is already used, otherwise 0.
}

//questions is not declared as a pointer since the collection will not be modified.
_Bool checkIfQuestionExists(questionsCollection questions, char* question) {
    _Bool doesQuestionExist = 0;

    unsigned int index;
    for(index = 0; index < questions.max; index++) {
        //Ignore null locations.
        if(questions.list[index] != NULL) {
            questionSet currentQuestion = *(questions.list[index]);
            if(!strcmp(currentQuestion.question, question)) {
                doesQuestionExist = 1;
            }
        }
    }

    return doesQuestionExist;
}

//questions is declared as a pointer since the collection will be modified.
void destroyCollection(questionsCollection* questions) {
    unsigned int index;
    for (index = 0; index < (*questions).max; index++) {
        free((*questions).list[index]); //Free each child pointer pointing to the elements.
    }

    free((*questions).list); //Free the mother pointer representing the list itself.
}

//This will be used to flush the buffer when handling user input using scanf().
void flushBuffer() {
    char character;
    while((character = getchar()) != '\n' && character != EOF);
}