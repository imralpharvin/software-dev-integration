#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  //initialize and allocate memory for calendar
  Calendar *theCalendar = malloc(sizeof(Calendar));
  //initialize properties list
  List * properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * events = initializeList(&printEvent, &deleteEvent, &compareEvents);

  //Load contentlines to list
  List * contentLines = icsParser(fileName);
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
    return INV_FILE;
  }

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
      Property * newProperty = malloc(sizeof(Property));
      strcpy(newProperty->propName, token);
      token = strtok(NULL, ":");
      newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 1);
      strcpy(newProperty->propDescr, token);
      theCalendar->version = atof(token);
      insertBack(properties, newProperty);
    }

    else if(strcmp(token, "PRODID") == 0)
    {
      Property * newProperty = malloc(sizeof(Property));
      strcpy(newProperty->propName, token);
      token = strtok(NULL, ":");
      newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 1);
      strcpy(newProperty->propDescr, token);
      strcpy(theCalendar->prodID, token);
      insertBack(properties, newProperty);
    }

    else if(strcmp(token, "BEGIN") == 0)
    {
      token = strtok(NULL, ":");
      if(strcmp(token, "VEVENT") == 0)
      {
          List * eventLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);
          Event * newEvent = malloc(sizeof(Event));
          //List * eventProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);

          while((elem = nextElement(&iter)) != NULL)
          {
            char * eventLine = contentLines->printData(elem);
            insertBack(eventLines, eventLine);
            if(strcmp(eventLine, "END:VEVENT") ==0 )
            {
              free(eventLine);
              break;
            }
            free(eventLine);

          }

          insertBack(events, newEvent);




      }

    }

    /*
    Property * newProperty = malloc(sizeof(Property));
    strcpy(newProperty->propName, token);
    //printf("%s\n", newProperty->propName);

    //Initialize property for the line
    while (token != NULL){

      //take property name
      token = strtok(NULL, ":");

      if(token != NULL)  {
        //reallocate for property description
        newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 1);
        strcpy(newProperty->propDescr, token);
        //printf("%s\n", newProperty->propDescr);

        //take version
        if(strcmp(newProperty->propName, "VERSION") == 0)  {
          theCalendar->version = atof(token);
        }
        //take production id
        else if(strcmp(newProperty->propName, "PRODID") == 0){
          strcpy(theCalendar->prodID, token);
        }


      }
    }
    //insert property to list
    insertBack(properties, newProperty);

    free(currDescr);
    */
    free(currDescr);
  }

  freeList(contentLines);
  freeList(events);

  //point to the address
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

  free(version);

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

}
int compareDates(const void* first, const void* second)
{
  return 0;
}
char* printDate(void* toBePrinted)
{
  return NULL;
}
