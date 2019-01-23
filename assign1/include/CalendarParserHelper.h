#ifndef CALENDARPARSERHELPER_H
#define CALENDARPARSERHELPER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* printFunc(void *toBePrinted);

int compareFunc(const void *first, const void *second);

void deleteFunc(void *toBeDeleted);

#endif
