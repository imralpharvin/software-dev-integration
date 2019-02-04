#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  //Check file
  if(checkFile(fileName) != OK)
  {
    return INV_FILE;
  }
  //initialize and allocate memory for calendar
  //Load contentlines to list
  List * contentLines = icsParser(fileName);

  if(checkCalendar(contentLines) != OK)
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
    freeList(contentLines);
    free(firstLine);
    return INV_CAL;
  }

  Calendar *theCalendar = malloc(sizeof(Calendar));
  theCalendar->version = 0;
  strcpy(theCalendar->prodID, "");

  //initialize properties list
  List * properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * events = initializeList(&printEvent, &deleteEvent, &compareEvents);

  bool versionParsed = false;
  bool prodIdParsed = false;

	while((elem = nextElement(&iter)) != NULL){
    char * currDescr = contentLines->printData(elem);
    char * colon = contentLines->printData(elem);
    char * semicolon = contentLines->printData(elem);
    if(strcmp(currDescr, "END:VCALENDAR") == 0 )
    {
      free(colon);
      free(semicolon);
      free(currDescr);
      break;
    }
    //Delimit by : and copy string to the prop

    char * colonToken = strtok(colon, ":");
    char * semicolonToken = strtok(semicolon, ";");
    char *token;

    if(strlen(colonToken) < strlen(semicolonToken))
    {
        token = strtok(currDescr, ":");
    }
    else if (strlen(colonToken) > strlen(semicolonToken))
    {
        token = strtok(currDescr, ";");
    }

    printf("TOKEN: %s\n", token);
    free(colon);
    free(semicolon);

    if(strcmp(token, "VERSION") == 0)
    {
      token = strtok(NULL, "\n");
      //printf("TOKEN: %s", token);
      if(versionParsed == true )
      {
        free(theCalendar);
        free(currDescr);
        freeList(properties);
        freeList(events);
        freeList(contentLines);
        return DUP_VER;
      }
      if(token == NULL ||  atof(token) == 0)
      {
        free(theCalendar);
        free(currDescr);
        freeList(properties);
        freeList(events);
        freeList(contentLines);
        return INV_VER;
      }

      theCalendar->version = atof(token);
      versionParsed = true;


    }

    else if(strcmp(token, "PRODID") == 0)
    {
      token = strtok(NULL, "\n");
      if(prodIdParsed == true )
      {
        free(theCalendar);
        free(currDescr);
        freeList(properties);
        freeList(events);
        freeList(contentLines);
        return DUP_PRODID;
      }
      if(token == NULL)
      {
        free(theCalendar);
        free(currDescr);
        freeList(properties);
        freeList(events);
        freeList(contentLines);
        return INV_PRODID;
      }

      strcpy(theCalendar->prodID, token);
      prodIdParsed = true;
    }

    else if(strcmp(token, "BEGIN") == 0)
    {
      token = strtok(NULL, "\n");
      printf("TOKEN: %s\n", token);
      if(strcmp(token, "VEVENT") == 0)
      {
          List * eventLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);

          while((elem = nextElement(&iter)) != NULL)
          {
            char * eventLine = contentLines->printData(elem);
            printf("eventLine: %s\n", eventLine);
            if(strcmp(eventLine, "END:VEVENT") ==0 )
            {
              printf("HERE AT END EVENT\n");
              free(eventLine);
              break;
            }
            else if(strcmp(eventLine, "END:VCALENDAR") ==0 || strcmp(eventLine, "BEGIN:VEVENT") ==0 )
            {
              free(eventLine);
              free(theCalendar);
              free(currDescr);
              freeList(properties);
              freeList(events);
              freeList(eventLines);
              freeList(contentLines);
              return INV_EVENT;
            }
            //free(eventLine);
            insertBack(eventLines, eventLine);
          }

          Event * newEvent;
          ICalErrorCode err = createEvent(eventLines, &newEvent) ;
          if(err != OK)
          {
            free(theCalendar);
            free(currDescr);
            freeList(properties);
            freeList(events);
            freeList(contentLines);
            freeList(eventLines);
            printf("HERE AT ERROR\n");
            return err;
          }


          insertBack(events, newEvent);

          freeList(eventLines);
      }
    }
    else
    {
      char * propertyLine = contentLines->printData(elem);
      Property * newProperty = createProperty(propertyLine);

      insertBack(properties, newProperty);
      free(propertyLine);
    }

    free(currDescr);
  }
  freeList(contentLines);


  //point to the address
  theCalendar->events = events;
  theCalendar->properties = properties;
  *obj = theCalendar;

  if(theCalendar->version == 0 || strlen(theCalendar->prodID) == 0 || getLength(theCalendar->events) == 0)
  {
    printf("NO VERSION OR prodID OR EVENTS\n");

    deleteCalendar(theCalendar);

    return INV_CAL;
  }

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

  char* calendarInfo;
  calendarInfo = (char*)malloc(sizeof(char) * 50);
  strcpy(calendarInfo, "************\nYOUR CALENDAR\n\n");
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen("Version: ")  + 1);
  strcat(calendarInfo , "Version: ");
  char * version;
  version = (char*)malloc(sizeof(char) +5);
  snprintf(version, 5, "%f", obj->version);
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen(version) + strlen("Production ID: ") + strlen(obj->prodID) + 100);
  strcat(calendarInfo, version);
  strcat(calendarInfo, "\n");
  strcat(calendarInfo , "Production ID: ");
  strcat(calendarInfo, obj->prodID);
  strcat(calendarInfo, "\nOther Calendar Properties:\n");

  char* toPrint = toString(obj->properties);
  calendarInfo = (char*)realloc(calendarInfo, strlen(calendarInfo) + strlen(toPrint) + 2);
  strcat(calendarInfo, toPrint);
  strcat(calendarInfo, "\n");

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
  char * invFile = "Error: INV_FILE (Invalid File)\n";
  char * invCal = "Error: INV_CAL (Invalid Calendar)\n";
  char * invEvent = "Error: INV_EVENT (Invalid Event)\n";
  char * invAlarm = "Error: INV_ALARM (Invalid Alarm)\n";
  char * invDateTime = "Error: INV_DT (Invalid Date Time)\n";
  char * invVer = "Error: INV_VER (Invalid Version)\n";
  char * dupVer = "Error: DUP_VER (Duplicate Version)\n";
  char * invProdid = "Error: INV_PRODID (Invalid Production ID)\n";
  char * dupProdid = "Error: DUP_PRODID (Duplicate Production ID)\n";
  char * otherError = "Error: OTHER_ERROR (Other Error)\n";

  char * error;
  if(err == INV_FILE)
  {
    error = malloc(sizeof(char) * strlen(invFile) + 1);
    strcpy(error, invFile);
  }
  else if(err == INV_CAL)
  {
    error = malloc(sizeof(char) * strlen(invCal) + 1);
    strcpy(error, invCal);
  }
  else if(err == INV_VER)
  {
    error = malloc(sizeof(char) * strlen(invVer) + 1);
    strcpy(error, invVer);
  }
  else if(err == DUP_VER)
  {
    error = malloc(sizeof(char) * strlen(dupVer) + 1);
    strcpy(error, dupVer);
  }
  else if(err == INV_PRODID)
  {
    error = malloc(sizeof(char) * strlen(invProdid) + 1);
    strcpy(error, invProdid);
  }
  else if(err == DUP_PRODID)
  {
    error = malloc(sizeof(char) * strlen(dupProdid) + 1);
    strcpy(error, dupProdid);
  }
  else if(err == INV_EVENT)
  {
    error = malloc(sizeof(char) * strlen(invEvent) + 1);
    strcpy(error, invEvent);
  }
  else if(err == INV_DT)
  {
    error = malloc(sizeof(char) * strlen(invDateTime) + 1);
    strcpy(error, invDateTime);
  }
  else if(err == INV_ALARM)
  {
    error = malloc(sizeof(char) * strlen(invAlarm) + 1);
    strcpy(error, invAlarm);
  }
  else if(err == OTHER_ERROR)
  {
    error = malloc(sizeof(char) * strlen(otherError) + 1);
    strcpy(error, otherError);
  }
  return error;
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

  char * toPrint  = (char*)malloc(sizeof(char)*1000);
  strcpy(toPrint, "> Event:\n");
  strcat(toPrint, "Creation Date and Time: \n");

  char * createDT = printDate(&(theEvent->creationDateTime));
  strcat(toPrint, createDT);
  strcat(toPrint, "Start Date and Time: \n");
  char * startDT = printDate(&(theEvent->startDateTime));
  strcat(toPrint, startDT);
  strcat(toPrint, "Other Properties: \n");
  char * properties = toString(theEvent->properties);
  toPrint = (char*)realloc(toPrint, strlen(toPrint) + strlen(properties)  + 1);
  strcat(toPrint, properties);

  char * alarms = toString(theEvent->alarms);
  toPrint = (char*)realloc(toPrint, strlen(toPrint) + strlen(alarms)  + 1);
  strcat(toPrint, alarms);

  free(createDT);
  free(startDT);
  free(properties);
  free(alarms);

  return toPrint;
}

void deleteAlarm(void* toBeDeleted)
{
  Alarm * toDelete = (Alarm *)toBeDeleted;
  free(toDelete->trigger);
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

  char * toPrint  = (char*)malloc(sizeof(char)*1000);
  strcpy(toPrint, ">>Alarm:\n");
  strcat(toPrint, "Action: ");
  strcat(toPrint, theAlarm->action);
  strcat(toPrint, "\n");
  strcat(toPrint, "Trigger: ");
  strcat(toPrint, theAlarm->trigger);
  strcat(toPrint, "\n");
  strcat(toPrint, "Other Properties:\n");

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
  strcat(toString, ": ");
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
  DateTime * toPrint = (DateTime*)  toBePrinted;
  char * toString = malloc(sizeof(char) *1000);

  strcpy(toString, "");
  strcat(toString, "Date: ");
  strcat(toString, toPrint->date );
  strcat(toString, "\n");
  strcat(toString, "Time: ");
  strcat(toString, toPrint->time);
  strcat(toString, "\n");
  strcat(toString, "UTC: ");
  if(toPrint->UTC == true)
  {
    strcat(toString, "YES\n");
  }

  else
  {
    strcat(toString, "NO\n");
  }

  return toString;
}
