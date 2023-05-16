#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "questions.h"

int main(void) {
    interview i = initInterviewQuestions(10);
    printf("%d", i.max);
    return 0;
}