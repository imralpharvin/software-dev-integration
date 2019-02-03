#ifndef CALENDARPARSERHELPER_H
#define CALENDARPARSERHELPER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

ICalErrorCode checkFile (char * filename);

ICalErrorCode checkCalendar (List * contentLines);

ICalErrorCode checkEvent(Event * newEvent);

ICalErrorCode checkAlarm(Alarm * newAlarm);

List * icsParser(char * fileName);

ICalErrorCode createDateTime (char * dtLine, DateTime ** theDateTime);

Property * createProperty(char * contentLine);

ICalErrorCode createEvent(List * eventLines, Event **theEvent);

ICalErrorCode createAlarm(List * alarmLines, Alarm **theAlarm);

char* printContentLine(void *toBePrinted);

int compareContentLine(const void *first, const void *second);

void deleteContentLine(void *toBeDeleted);

#endif
