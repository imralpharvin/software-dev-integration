/* Name: Ralph Arvin De Castro
  Student ID: 0923223

*/

#ifndef CALENDARPARSERHELPER_H
#define CALENDARPARSERHELPER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

ICalErrorCode validateFile (char * filename);

char * determineDelimiter(char * contentLine);

List * icsParser(char * fileName);

ICalErrorCode createDateTime (char * dtLine, DateTime ** theDateTime);

Property * createProperty(char * contentLine);

ICalErrorCode createEvent(List * eventLines, Event **theEvent);

ICalErrorCode createAlarm(List * alarmLines, Alarm **theAlarm);

char* printContentLine(void *toBePrinted);

int compareContentLine(const void *first, const void *second);

void deleteContentLine(void *toBeDeleted);

#endif
