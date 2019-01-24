#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  //initialize and allocate memory for calendar
  Calendar *theCalendar = malloc(sizeof(Calendar));
  //initialize properties list
  List * properties = initializeList(&printFunc, &deleteFunc, &compareFunc);

  //open file
  FILE * fp;
  fp = fopen(fileName, "r");

  //process file line by line
  char line[75];
  while (fgets(line, sizeof(line), fp)){

    //Initialize property for the line
    Property * newProperty = malloc(sizeof(Property));
    line[strlen(line)-1] = '\0';

    //Delimit by : and copy string to the prop
    char *token = strtok(line, ":");
    strcpy(newProperty->propName, token);
    //printf("%s\n", newProperty->propName);
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
    //printf("List: %s\n", ((Property*)properties->head->data)->propName);
    //printf("%d\n", getLength(properties));
  }
  //close files
  fclose(fp);
  //point to the address
  theCalendar->properties = properties;
  *obj = theCalendar;


  return OK;
}

void deleteCalendar(Calendar* obj)
{
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

  printf("%s", calendarInfo);
  printf("%s", toString(obj->properties));
  free(version);
  free(calendarInfo);

  return "FIX THIS\n";
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
