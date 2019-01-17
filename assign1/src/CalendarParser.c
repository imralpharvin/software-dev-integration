#include <stdio.h>
#include "CalendarParser.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  return OK;
}

void deleteCalendar(Calendar* obj)
{

}

char* printCalendar(const Calendar* obj)
{
  return "ok:";
}

char* printError(ICalErrorCode err)
{
  return null;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{
  return null;
}

ICalErrorCode validateCalendar(const Calendar* obj)
{
  return null;
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
