#include <stdio.h>
#include "CalendarParser.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{

}

void deleteCalendar(Calendar* obj)
{

}

char* printCalendar(const Calendar* obj)
{

}

char* printError(ICalErrorCode err)
{

}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{

}

ICalErrorCode validateCalendar(const Calendar* obj)
{

}


// ************* List helper functions - MUST be implemented ***************
void deleteEvent(void* toBeDeleted)
{

}
int compareEvents(const void* first, const void* second)
{

}
char* printEvent(void* toBePrinted)
{

}

void deleteAlarm(void* toBeDeleted)
{

}
int compareAlarms(const void* first, const void* second)
{

}
char* printAlarm(void* toBePrinted)
{

}

void deleteProperty(void* toBeDeleted)
{

}
int compareProperties(const void* first, const void* second)
{

}
char* printProperty(void* toBePrinted)
{

}

void deleteDate(void* toBeDeleted)
{

}
int compareDates(const void* first, const void* second)
{

}
char* printDate(void* toBePrinted)
{

}
