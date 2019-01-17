#include <stdio.h>
#include <string.h>
#include "CalendarParser.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  Calendar theCalendar;
  FILE * fp;
  fp = fopen(fileName, "r");
  char line[75];

  while (fgets(line, sizeof(line), fp))
  {
    line[strlen(line)-1] = '\0';

    char *token = strtok(line, ":");

    while (token != NULL)
    {
      if(strcmp(token, "VERSION") == 0)
      {
        token = strtok(NULL, ":");
        theCalendar.version = atof(token);
      }
      else if(strcmp(token, "PRODID") == 0)
      {
        token = strtok(NULL, ":");
        strcpy(theCalendar.prodID, token);

      }
      token = strtok(NULL, ":");
    }
  }

  fclose(fp);
  printf("version: %f\n", theCalendar.version);
  printf("prodID: %s\n", theCalendar.prodID);
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
  return NULL;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{
  return OK;
}

ICalErrorCode validateCalendar(const Calendar* obj)
{
  return OK;
}


// ************* List helper functions - MUST be implemented ***************
void deleteEvent(void* toBeDeleted)
{

}
int compareEvents(const void* first, const void* second)
{
  return 0;
}
char* printEvent(void* toBePrinted)
{
  return NULL;
}

void deleteAlarm(void* toBeDeleted)
{

}
int compareAlarms(const void* first, const void* second)
{
  return 0;
}
char* printAlarm(void* toBePrinted)
{
  return NULL;
}

void deleteProperty(void* toBeDeleted)
{

}
int compareProperties(const void* first, const void* second)
{
  return 0;
}
char* printProperty(void* toBePrinted)
{
  return NULL;
}

void deleteDate(void* toBeDeleted)
{

}
int compareDates(const void* first, const void* second)
{
  return 0;
}
char* printDate(void* toBePrinted)
{
  return NULL;
}
