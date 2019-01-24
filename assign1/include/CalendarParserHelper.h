#ifndef CALENDARPARSERHELPER_H
#define CALENDARPARSERHELPER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

List * icsParser(char * fileName);

char* printContentLine(void *toBePrinted);

int compareContentLine(const void *first, const void *second);

void deleteContentLine(void *toBeDeleted);

#endif
