#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{

  if(checkFile(fileName) == false)
  {
    printf("ERROR\n");
    return INV_FILE;
  }

  //initialize and allocate memory for calendar

  //Load contentlines to list
  List * contentLines = icsParser(fileName);

  if(checkCalendar(contentLines) == false)
  {
    return INV_CAL;

  }
  ListIterator iter = createIterator(contentLines);

  //Get the first element and see if it begins with v calendar
	void* elem;
  elem = nextElement(&iter);
  char* firstLine = contentLines->printData(elem);
  if(strcmp(firstLine, "BEGIN:VCALENDAR") == 0)
  {
    free(firstLine);
  }
  else
  {
    printf("INVALID BEGINNING TAG\n");
    freeList(contentLines);
    free(firstLine);
    return INV_CAL;
  }

  Calendar *theCalendar = malloc(sizeof(Calendar));
  //initialize properties list
  List * properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * events = initializeList(&printEvent, &deleteEvent, &compareEvents);


	while((elem = nextElement(&iter)) != NULL){
    char * currDescr = contentLines->printData(elem);
    if(strcmp(currDescr, "END:VCALENDAR") == 0 )
    {
      free(currDescr);
      break;
    }
    //Delimit by : and copy string to the prop
    char *token = strtok(currDescr, ":");

    if(strcmp(token, "VERSION") == 0)
    {

      char * propertyLine = contentLines->printData(elem);
      Property * newProperty = createProperty(propertyLine);
      theCalendar->version = atof(newProperty->propDescr);
      insertBack(properties, newProperty);
      free(propertyLine);
    }

    else if(strcmp(token, "PRODID") == 0)
    {
      char * propertyLine = contentLines->printData(elem);
      Property * newProperty = createProperty(propertyLine);

      strcpy(theCalendar->prodID, newProperty->propDescr);
      insertBack(properties, newProperty);
      free(propertyLine);
    }

    else if(strcmp(token, "BEGIN") == 0)
    {
      token = strtok(NULL, ":");
      if(strcmp(token, "VEVENT") == 0)
      {
          List * eventLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);

          while((elem = nextElement(&iter)) != NULL)
          {
            char * eventLine = contentLines->printData(elem);

            if(strcmp(eventLine, "END:VEVENT") ==0 )
            {
              free(eventLine);
              break;
            }
            //free(eventLine);
            insertBack(eventLines, eventLine);

          }

          Event * newEvent = createEvent(eventLines);
          insertBack(events, newEvent);

          freeList(eventLines);
      }
    }
    free(currDescr);
  }
  freeList(contentLines);

  //point to the address
  theCalendar->events = events;
  theCalendar->properties = properties;
  *obj = theCalendar;

  return OK;
}

void deleteCalendar(Calendar* obj)
{
  freeList(obj->events);
  freeList(obj->properties);
  free(obj);
}

char* printCalendar(const Calendar* obj)
{
  char * version;
  version = (char*)malloc(sizeof(char) +5);
  strcpy(version, "");
  //version = (char*)realloc(version, strlen(version) + 5);

  char* calendarInfo;
  calendarInfo = (char*)malloc(sizeof(char));
  strcpy(calendarInfo, "");
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen("Version: ")  + 1);
  strcat(calendarInfo , "Version: ");
  snprintf(version, 5, "%f", obj->version);
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen(version) + strlen("Production ID: ") + strlen(obj->prodID) + 5);
  strcat(calendarInfo, version);
  strcat(calendarInfo, "\n");
  strcat(calendarInfo , "Production ID: ");
  strcat(calendarInfo, obj->prodID);
  strcat(calendarInfo, "\n");

  char* toPrint = toString(obj->properties);
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen(toPrint) + 1);
  strcat(calendarInfo, toPrint);

  char* eventPrint = toString(obj->events);
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen(eventPrint) + 1);
  strcat(calendarInfo, eventPrint);
  //char * eventProperties = toString(obj)
  free(version);
  free(eventPrint);
  free(toPrint);

  return calendarInfo;
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
  Event * delete = (Event*)toBeDeleted;
  freeList(delete->properties);
  freeList(delete->alarms);
  free(toBeDeleted);
}
int compareEvents(const void* first, const void* second)
{
  return 0;
}
char* printEvent(void* toBePrinted)
{
  Event * theEvent = (Event*)toBePrinted;

  char * toPrint  = (char*)malloc(sizeof(char)*8);
  strcpy(toPrint, "Event:\n");

  char * properties = toString(theEvent->properties);
  toPrint = (char*)realloc(toPrint, strlen(toPrint) + strlen(properties)  + 1);
  strcat(toPrint, properties);

  char * alarms = toString(theEvent->alarms);
  toPrint = (char*)realloc(toPrint, strlen(toPrint) + strlen(alarms)  + 1);
  strcat(toPrint, alarms);

  free(properties);
  free(alarms);

  return toPrint;
}

void deleteAlarm(void* toBeDeleted)
{
  Alarm * toDelete = (Alarm *)toBeDeleted;
  freeList(toDelete->properties);
  free(toBeDeleted);
}
int compareAlarms(const void* first, const void* second)
{
  return 0;
}
char* printAlarm(void* toBePrinted)
{
  Alarm * theAlarm = (Alarm*)toBePrinted;

  char * toPrint  = (char*)malloc(sizeof(char)*8);
  strcpy(toPrint, "Alarm:\n");

  char * properties = toString(theAlarm->properties);
  toPrint = (char*)realloc(toPrint, strlen(toPrint) + strlen(properties)  + 1);
  strcat(toPrint, properties);

  free(properties);

  return toPrint;
}

void deleteProperty(void* toBeDeleted)
{
  free((Property*)toBeDeleted);
}
int compareProperties(const void* first, const void* second)
{
  return 0;
}
char* printProperty(void* toBePrinted)
{
  char * propName = ((Property*)toBePrinted)->propName;
  char * propDescr = ((Property*)toBePrinted)->propDescr;
  char * toString = malloc(sizeof(char) *(strlen(propName) + strlen(propDescr) + 3));
  strcpy(toString, propName);
  strcat(toString, ":");
  strcat(toString, propDescr);
  return toString;

}

void deleteDate(void* toBeDeleted)
{
  free(toBeDeleted);
}
int compareDates(const void* first, const void* second)
{
  return 0;
}
char* printDate(void* toBePrinted)
{
  return NULL;
}
