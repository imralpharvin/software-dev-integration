#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  //initialize and allocate memory for calendar
  Calendar *theCalendar = malloc(sizeof(Calendar));

  //open file
  FILE * fp;
  fp = fopen(fileName, "r");


  //process file line by lune
  List * properties = initializeList(&printFunc, &deleteFunc, &compareFunc);

  char line[75];
  while (fgets(line, sizeof(line), fp)){
    line[strlen(line)-1] = '\0';

    char *token = strtok(line, ":");
    Property * newProperty = malloc(sizeof(Property));
    strcpy(newProperty->propName, token);
    printf("%s\n", newProperty->propName);
    while (token != NULL){

      //find version
      token = strtok(NULL, ":");

      if(token != NULL)  {

        newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 1);
        strcpy(newProperty->propDescr, token);
        printf("%s\n", newProperty->propDescr);
        if(strcmp(newProperty->propName, "VERSION") == 0)  {
          theCalendar->version = atof(token);

        }
        //find prodId
        else if(strcmp(newProperty->propName, "PRODID") == 0){
          strcpy(theCalendar->prodID, token);

        }
      }
    }

    //Node * propertyNode = initializeNode(newProperty);
    //printf("Node: %s\n", ((Property*)propertyNode->data)->propName);
    insertFront(properties, newProperty);
    printf("List: %s\n", ((Property*)properties->head->data)->propName);
    printf("%d\n", getLength(properties));
  }

  fclose(fp);
  //point to the address
  theCalendar->properties = properties;

  *obj = theCalendar;


  return OK;
}

void deleteCalendar(Calendar* obj)
{
  free(obj);
}

char* printCalendar(const Calendar* obj)
{
  char * calendarInfo = malloc(1000);
  char * version = malloc(1000);
  strcpy(calendarInfo , "Version: ");
  snprintf(version, 5, "%f", obj->version);
  strcat(calendarInfo, version);
  strcat(calendarInfo, "\n");

  strcat(calendarInfo , "Production ID: ");
  strcat(calendarInfo, obj->prodID);
  strcat(calendarInfo, "\n");
  free(version);
  //free(calendarInfo);
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
