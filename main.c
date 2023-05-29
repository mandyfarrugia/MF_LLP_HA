/* By using the following preprocessor definition, the compiler error C4996 is disabled.
   Visual Studio will complain that scanf, strcpy, and fopen may be unsafe. 
   It will then suggest using scanf_s, strcpy_s, and fopen_s instead.
   This issue is as a result of the above functions being considered unsafe by most compilers.
   Particularly, scanf() is deemed unsafe due to buffer overflow and memory overwrite.
   An alternative to #define _CRT_SECURE_NO_WARNINGS would be #pragma warning(disable:4996).
   It is vital that this preprocessor definition goes before the rest of the preprocessor directive statements. */
#define _CRT_SECURE_NO_WARNINGS

#include "questions.h" //Include question.h to use functions manipulating the list of questions.

#include <ctype.h> //Include ctype.h to use tolower() to capture the character entered by the user when exiting the program.
#include <stdio.h> //Include stdio.h to use fopen(), scanf(), and printf() for file handling, input, and output operations.
#include <stdlib.h> //Include stdlib.h to use malloc(), realloc(), and free() for memory allocation/re-allocation/deallocation.
#include <string.h> //Include string.h to use strcpy() and strlen() to handle strings and assign data to them.

/* This constant represents the total number of menu options.
   It will be used to allocate the list of menu options, as well traversing the list to copy each option from the static array. 
   More information will be available in the function getMenuOptions. */
#define MENU_OPTIONS 6 
#define MAX_OPTIONS_CHARS 30 //This constant represents the maximum characters for each menu option.

#define MAX_DUMMY_QUESTIONS 10 //This constant represents the maximum dummy questions.
#define MAX_DUMMY_QUESTION_CHAR 90 //This constant represents the maximum characters for each dummy question.

/* The path to the binary file is stored in a constant since the same file will be used for both saving and loading.
   This constant replaces having to manually supplying the path to the file when fopen() is invoked, 
   whereby the path only once and then supplying the constant to fopen(). */
#define PATH_TO_BINARY_FILE "questions.dat"

//The following are function declarations of the functions to be used throughout the lifetime of the program.

/* This function will display a string passed as an argument and supplied to the title parameter. 
   The string would represent the title corresponding to the option chosen from the menu. 
   Furthermore, a line made up of the equals sign will appear underneath the title.
   The amount of equals signs printed depends on the length of the title parameter. */
void displayTitle(char* title);

/* Within this function, numeric input using scanf() takes place.
   Assuming that negative numeric input is unnecessary at this stage, the %u format specifier will be used.
   This is to indicate that scanf() should expect an unsigned/positive integer.
   With the use of a do-while, invalid input will be prevented and the buffer will also be flushed.
   The value returned by scanf() will be stored in a variable and the loop will keep iterating until the value is at least 1.
   The value will be 1 if the appropriate input is supplied, namely numeric input. */
void getNumericInput(char* prompt, unsigned int* numericInput, int check);

/* Ask the user to enter a question number, as well as a question. 
   Before adding the question to the list, it is important to check whether the question number already exists. 
   This also applies for the question itself. This is to avoid having duplicates, especially the question number
   since this should uniquely identify each question similar to a primary key for records in database tables. */
void prepareAddQuestion(questionsCollection* questions, int check);

/* This function starts by asking the user which question they wish to delete. The user should supply a question number.
   Validation will take place to ensure the input is in the correct format before proceeding with deletion. */
void prepareDeleteQuestion(questionsCollection* questions, int check);

/* This function starts by asking whether the user is sure as to whether they want to exit the program. 
   If yes, then attempt to save the questions (if any are in the list). Otherwise, return to the menu.
   Validation will take place to ensure only y/Y or n/N are accepted as input. */
void confirmBeforeExit(questionsCollection questions, FILE* file, _Bool* isProgramRunningFlag);

/* This function will copy the contents of a 2D array representing menu options to a pointer to a pointer to char representing a dynamic list.
   This was written to demonstrate an understanding of double pointers and dynamic collection of dynamic strings. */
char** getMenuOptions();

void displayMenu(char** menuOptions); //This function will traverse the dynamic list representing the menu options and print each option.

/* This function is a last resort in the case no questions are found in the binary file.
   At this point, there is no need to check whether the question number or question already exists. */
void addDummyStruct(questionsCollection* questions, unsigned int questionNumber, char* question);

//This function will invoke addDummyStruct to add 10 dummy questions to the list to get started with the program.
void initialiseCollectionFromDummyStructs(questionsCollection* questions);

/* The string passed as an argument will be sent to a parameter of type char* named title.
   Underneath the title, a line made up of equals signs will be printed next to each other.
   How many will be printed depends on the length of the string. 
   Finally, skip a line when the index is one less than the length of the string. */
void displayTitle(char* title) {
    unsigned int index;
    printf("%s\n", title);
    for(index = 0; index < strlen(title); index++) {
        printf("=");
        if(index == strlen(title) - 1)
            printf("\n");
    }
}

/* numericInput is declared as a pointer since the argument must change outside this function (call by reference).
   This was done to demonstrate an understanding of call-by-reference functions. */
void getNumericInput(char* prompt, unsigned int* numericInput, int check) {
    do {
        printf("%s: ", prompt); //Prompt the user to enter a positive/unsigned integer.

        /* Force scanf() to expect input of type unsigned integer because of the format specifier %u. 
           Store the value returned by scanf() to check whether input is valid.
           The value returned by check represents how many successful inputs were captured. */
        check = scanf("%u", numericInput);

        //check < 1 indicates that no successful input was captured.
        if (check < 1) {
            printf("Invalid input detected!\n"); //Inform user that input was invalid.
            flushBuffer(); //Flush the buffer in case of invalid input so it does not affect the next scanf() invocation.
        }
    } while (check < 1); //Keep looping until input is of correct format.
}

//questions is declared as a pointer since the collection will be modified.
void prepareAddQuestion(questionsCollection* questions, int check) {
    questionSet questionToBeAdded; //Prepare a struct of type questionSet.

    /* getNumericInput was not used since the function must also check whether the number supplied is already associated with a question in the list.
       The question number should uniquely identify a question like a primary key uniquely identifies each record in a table. 
       Therefore, it is important that the question number is unique for each question to simplify references to specific questions. */
    do {
        flushBuffer();

        printf("Enter the number to be associated with the question: "); //Prompt the user to enter a number.
        check = scanf("%u", &questionToBeAdded.questionNumber); //Capture input of type unsigned integer and store it in questionToBeAdded.questionNumber.

        /* check < 1 denotes that no successful input was captured by scanf(), in this case indicating that numeric input was not provided.
           checkIfQuestionNumberExists checks whether there is a question associated with the question number provided by the user. 
           Obviously, if it is true, then the user should not be allowed to use this number again. */
        if (check < 1) {
            printf("Invalid input detected!\n\n");
        } else if(checkIfQuestionNumberExists(*questions, questionToBeAdded.questionNumber)) {
            printf("There is already a question associated with this number!\n\n");
        }
    } while (check < 1 || checkIfQuestionNumberExists(*questions, questionToBeAdded.questionNumber));

    do {
        flushBuffer();

        printf("Enter a question: ");

        /* Tell scanf() to read up to 149 characters to save at least one character for the null character which terminates the string.
           [^\n] denotes keep reading until the new line character '\n' is detected (in layman terms, when the Enter/Return key is pressed).*/
        scanf("%149[^\n]", questionToBeAdded.question);
        questionToBeAdded.question[strlen(questionToBeAdded.question)] = '\0';

        //If there is already a question associated with the number provided, warn the user and prompt them to enter another number.
        if(checkIfQuestionExists(*questions, questionToBeAdded.question)) {
            printf("Question already exists!\n\n");
        }
    } while (checkIfQuestionExists(*questions, questionToBeAdded.question));

    /* addQuestion returns a value of type _Bool indicating whether adding the question to the list was successful.
       0 - If an error occurred while resizing the list if there is no more space (size == max).
       1 - If the question was added to the list successfully. */
    _Bool questionAddedSuccessfully = addQuestion(questions, questionToBeAdded);
    printf("\n%s\n", questionAddedSuccessfully ? "Question added successfully!" : "Unable to add question!");
}

//questions is declared as a pointer since the collection will be modified.
void prepareDeleteQuestion(questionsCollection* questions, int check) {
    unsigned int questionNumberToDelete = 0;

    /* Prompt the user for input of type unsigned integer. 
       The address of questionNumberToDelete is passed to the function since the changes to the parameter within the function must affect the argument. */
    getNumericInput("Enter the number associated with the question you want to delete", &questionNumberToDelete, check);
    
    /* deleteQuestion returns a value of type _Bool indicating whether the question was deleted from the list successfully. 
       0 - If there is no question associated with the number entered by the user. 
       1 - If the question was deleted from the list successfully. */
    _Bool questionDeletedSuccessfully = deleteQuestion(questions, questionNumberToDelete);
    printf("%s\n", questionDeletedSuccessfully ? "Question deleted successfully!" : "No question associated with this number can be found!");
}

//In this case, we will be copying the contents of the list to a binary file. Therefore, questions is not declared as a pointer since the collection will not be modified.
void confirmBeforeExit(questionsCollection questions, FILE* file, _Bool* isProgramRunningFlag) {
    char exitConfirmation;

    do {
        flushBuffer();

        printf("Are you sure you want to exit? (Y/N): "); //Prompt the user to enter either Y to save and exit, or to go back to the menu.
        scanf("%c", &exitConfirmation);
        exitConfirmation = tolower(exitConfirmation); //Convert the input to lowercase.

        //If the input is not y or n, warn the user and prompt them to try again.
        if(exitConfirmation != 'y' && exitConfirmation != 'n') {
            printf("Invalid input detected!\n");
        }
    } while(exitConfirmation != 'y' && exitConfirmation != 'n'); //Restrict input to y/n.
    
    /* If the user enters y, then check whether the list is empty as it would not make sense to save no data at all. 
       If the list is not empty, then copy the contents of the list to a binary file. */
    if(exitConfirmation == 'y') {
        if (!isEmpty(questions)) {
            file = fopen(PATH_TO_BINARY_FILE, "wb"); //Set the access mode to wb to write to the binary file.

            //Check if file exists or if it was created successfully if it did not exist.
            if (file) {
                writeQuestionsToBinaryFile(questions, file); //Save the contents of the list to the binary file.
                destroyCollection(&questions); //Free the memory allocated to both children and mother pointers respectively.
            }
            else {
                printf("File could not be found or created!\n");
            }
        } else {
            printf("There is nothing to save!\n");
        }
        
        *isProgramRunningFlag = 0; //Set the flag to 0 to exit the loop.
    }
}

//getMenuOptions() will return a dynamic list of dynamic strings representing menu options.
char** getMenuOptions() {
    //Start by storing each option in a 2D array. This will be used to copy each element to the dynamic list.
    char staticOptions[MENU_OPTIONS][MAX_OPTIONS_CHARS] = {"View all questions", "Ask a question", "Add a question", "Delete a question", "Shuffle questions", "Save to file and exit"};
    
    /* Allocate memory to a dynamic list of dynamic strings in the form of pointer to pointer to char. 
       This is like saying declare a list which must store up to 6 strings. */
    char** menuOptions = (char**)malloc(MENU_OPTIONS * sizeof(char*)); 

    /* If memory was allocated to menuOptions successfully, then traverse each pointer within menuOptions.
       Allocate memory to each pointer using malloc(), then copy each element from the static array to the dynamic list.*/
    if(menuOptions) {
        unsigned int index;
        for(index = 0; index < MENU_OPTIONS; index++) {
            //So as not to allocate more memory than necessary, use strlen to get how many characters each string in the static array is made up of.
            size_t lengthOfOption = strlen(staticOptions[index]);
            /* Allocate memory for each pointer in the double pointer using malloc(). To allow space for the null character, add 1 to lengthOfOption.
               In layman terms, for each element in the static array, declare a string made up of the amount of characters which makes up the string. */
            menuOptions[index] = (char*)malloc((lengthOfOption + 1) * sizeof(char));
            strcpy(menuOptions[index], staticOptions[index]); //Copy the current element in the static array to the current position of the newly declared dynamic list.
        }
    }

    return menuOptions; //Return NULL if malloc() fails to allocate memory for the double pointer. Otherwise, return the dynamic list of dynamic strings.
}

//Print the menu to the console window.
void displayMenu(char** menuOptions) {
    if(!menuOptions)
        return; //If menuOptions is NULL, then exit the function.

    displayTitle("Interview questions");
    //Traverse the list of menu options and print each element using pointer indexing.
    unsigned int index;
    for(index = 0; index < MENU_OPTIONS; index++)
        printf("%u. %s\n", (index + 1), *(menuOptions + index)); //(index + 1) to start from 1 and not 0.
}

//For each dummy question, prepare a struct with the parameters representing the question number and question, then invoke addQuestion to add the dummy struct.
void addDummyStruct(questionsCollection* questions, unsigned int questionNumber, char* question) {
    questionSet dummyQuestion;
    dummyQuestion.questionNumber = questionNumber;
    strcpy(dummyQuestion.question, question);
    addQuestion(questions, dummyQuestion);
}

void initialiseCollectionFromDummyStructs(questionsCollection* questions) {
    //Create a static 2D array with a maximum of 10 dummy questions, each with a maximum of 90 characters.
    char interviewQuestions[MAX_DUMMY_QUESTIONS][MAX_DUMMY_QUESTION_CHAR] = {
        "Why is C called a mid-level programming language?",
        "What are the features of the C language?",
        "What is the use of printf() and scanf() functions?",
        "What is a built-in function in C?",
        "What is a preprocessor?",
        "How can a string be converted to a number?",
        "How can a number be converted to a string?",
        "What is recursion in C?",
        "Why doesn't C support function overloading?",
        "What is the difference between global int and static int declaration?"
    };

    //Get the amount of elements in the static array by dividing the size of the array with the size of one element.
    size_t amountOfInterviewQuestions = sizeof(interviewQuestions) / sizeof(interviewQuestions[0]);
    unsigned int index;
    for (index = 0; index < amountOfInterviewQuestions; index++) {
        addDummyStruct(questions, (index + 1), interviewQuestions[index]); //Add each question with the question number starting from 1 up to 10.
    }
}

int main(void) {
    /* Start by initialising the collection which allows a maximum of 10 elements.
       Since the collection is newly initialised, the size is set to 0 as there would not be any elements at this stage. */
    questionsCollection questions = initialiseCollection();

    FILE* file = fopen(PATH_TO_BINARY_FILE, "rb"); //Set the file mode to rb to load the questions from the binary file.

    //If the file does not exist, then add dummy structs to the list to get started.
    if(!file) {
        initialiseCollectionFromDummyStructs(&questions);
    } else {
        unsigned int questionsReadFromFile = readQuestionsFromBinaryFile(&questions, file); //Get the amount of question structs read from the file.

        //If no question structs could be found, then add dummy structs to the list to get started.
        if(!questionsReadFromFile) {
            initialiseCollectionFromDummyStructs(&questions);
        }
    }

    _Bool isProgramRunning = 1;
    unsigned int choice = 0;
    char** menuOptions;

    do {
        int check = 0;

        menuOptions = getMenuOptions();
        displayMenu(menuOptions);

        if(menuOptions) {
            getNumericInput("Enter choice", &choice, check);

            switch(choice) {
                case 1:
                    displayTitle("\nView questions");
                    viewQuestions(questions);
                    break;
                case 2:
                    displayTitle("\nAsk question");
                    askQuestion(questions);
                    break;
                case 3:
                    displayTitle("\nAdd question");
                    prepareAddQuestion(&questions, check);
                    break;
                case 4:
                    displayTitle("\nDelete question");
                    prepareDeleteQuestion(&questions, check);
                    break;
                case 5:
                    displayTitle("\nShuffle question");
                    shuffleQuestions(&questions);
                    break;
                case 6:
                    displayTitle("\nSave and exit");
                    confirmBeforeExit(questions, file, &isProgramRunning);
                    break;
                default:
                    printf("Invalid choice!\n");
                    break;
            }

            printf("\n");
        }
        else {
            printf("Unable to load menu!\n");
            isProgramRunning = 0;
        }
    } while(choice != 6 || isProgramRunning); //Keep looping until 6 is chosen or isProgramRunning is set to 0.

    //Go through each single char pointer in the double pointer of char and free the memory allocated to it.
    unsigned int index;
    for(index = 0; index < MENU_OPTIONS; index++) {
        free(menuOptions[index]);
    }

    free(menuOptions); //Free the double pointer which stores the menu options.

    return 0; //Return 0 to indicate a successful execution.
}