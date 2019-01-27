#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

List * icsParser(char * fileName)
{
  FILE * fp;
  fp = fopen(fileName, "r");

  //process file line by line
  char line[75];

  //Fold ContentLines
  List * contentLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);
  char * previousLine;
  char * contentLine;
  int count = 0;
  while (fgets(line, sizeof(line), fp)){

    if(  line[strlen(line)-2] == '\r')
    {
      line[strlen(line)-2] = '\0';
    }
    else
    {
      line[strlen(line)-1] = '\0';
    }
    //printf("%s = %lu\n", line, strlen(line));
    contentLine = malloc(strlen(line) + 1);
    strcpy(contentLine, line);

    if(line[0] == ' '){
      previousLine = realloc(previousLine, strlen(contentLine) + strlen(previousLine) + 2);
      strcat(previousLine, contentLine);
      free(contentLine);
    }
    else{
      if(count > 0)
      {
        insertBack(contentLines, previousLine);
      //Point previous to current if no new line
      }
      previousLine = malloc(strlen(contentLine) + 1);
      strcpy(previousLine, contentLine);
      free(contentLine);
      //Insert content line
      count ++;
    }
  }
  insertBack(contentLines, previousLine);

  fclose(fp);
  return contentLines;
}

DateTime * createDateTime (char * dtLine)
{
  DateTime * newDateTime = malloc(sizeof(DateTime)) ;

  char * token = strtok(dtLine, "T");
  strcpy(newDateTime->date, token);

  token = strtok(NULL, "\n");
  //printf("%s\n", token);

  strcpy(newDateTime->time, token);
  if(token[6] == 'Z' )
  {
    printf("BEFORE: %s\n", newDateTime->time);
    //newDateTime->UTC = true;
    printf("AFTER: %s\n", newDateTime->time);
  }

  return newDateTime;
}
Property * createProperty(char * contentLine)
{
  Property * newProperty = malloc(sizeof(Property));

  char *token = strtok(contentLine, ":");

  strcpy(newProperty->propName, token);
  token = strtok(NULL, "\n");

  newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 2);
  strcpy(newProperty->propDescr, token);
  return newProperty;
}

Event * createEvent(List * eventLines)
{
  Event * newEvent = malloc(sizeof(Event));
  List * eventProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);

  ListIterator iter = createIterator(eventLines);
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = eventLines->printData(elem);
    char * toSplit = eventLines->printData(elem);
    char * token = strtok(toSplit, ":");

    if(strcmp(currDescr, "END:VEVENT") == 0 )
    {
      free(toSplit);
      free(currDescr);
      break;
    }
    else if(strcmp(token, "BEGIN") == 0)
    {
      token = strtok(NULL, "\n");
      if(strcmp(token, "VALARM") == 0)
      {
          List * alarmLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);

          while((elem = nextElement(&iter)) != NULL)
          {
            char * alarmLine = eventLines->printData(elem);
            //printf("THIS %s\n", alarmLine);
            insertBack(alarmLines, alarmLine);
            if(strcmp(alarmLine, "END:VALARM") ==0 )
            {
              //free(eventLine);

              break;
            }
            //free(eventLine);

          }


          Alarm * newAlarm = createAlarm(alarmLines);
          insertBack(alarms, newAlarm);

          freeList(alarmLines);


      }

    }
    else
    {
      Property * eventProperty = createProperty(currDescr);
      insertBack(eventProperties, eventProperty);
    }
    free(currDescr);
    free(toSplit);
  }



//PART B VALUES

  ListIterator iterProp = createIterator(eventProperties);
  void* prop;
  while((prop = nextElement(&iterProp)) != NULL){
    char * currDescr = eventProperties->printData(prop);
    char *token = strtok(currDescr, ":");

    if(strcmp(token, "UID") == 0)
    {
      token = strtok(NULL, "\n");
      strcpy(newEvent->UID, token);
      //printf("UID: %s\n", newEvent->UID);
    }

    if(strcmp(token, "DTSTAMP") == 0)
    {
      token = strtok(NULL, "\n");
      DateTime * dtStamp = createDateTime(token);
      newEvent->creationDateTime = *dtStamp;

      deleteDate(dtStamp);
      /*
      printf("DTstampdate: %s\n", newEvent->creationDateTime.date);
      printf("DTstamptime: %s\n", newEvent->creationDateTime.time);
      printf("DTstamputc: %d\n", newEvent->creationDateTime.UTC);
      printf("DTstamputc: %s\n", newEvent->creationDateTime.UTC ? "true" : "false");*/
    }

    if(strcmp(token, "DTSTART") == 0)
    {
      token = strtok(NULL, "\n");
      DateTime * dtStart = createDateTime(token);
      newEvent->startDateTime = *dtStart;


      /*printf("DTstampdate: %s\n", newEvent->startDateTime.date);
      printf("DTstamptime: %s\n", newEvent->startDateTime.time);
      printf("DTstamputc: %d\n", newEvent->startDateTime.UTC);
      printf("DTstamputc: %s\n", newEvent->startDateTime.UTC ? "true" : "false");*/
      deleteDate(dtStart);
    }

    free(currDescr);
  }


  newEvent->properties = eventProperties;
  newEvent->alarms = alarms;

  return newEvent;
}

Alarm * createAlarm(List * alarmLines)
{
  Alarm * newAlarm = malloc(sizeof(Alarm));

  List * alarmProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);

  ListIterator iter = createIterator(alarmLines);
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = alarmLines->printData(elem);

    if(strcmp(currDescr, "END:VALARM") == 0 )
    {
      free(currDescr);
      break;
    }

    Property * alarmProperty = createProperty(currDescr);
    insertBack(alarmProperties, alarmProperty);




    free(currDescr);

  }



  newAlarm->properties = alarmProperties;

  ListIterator iterProp = createIterator(alarmProperties);
  void* prop;
  while((prop = nextElement(&iterProp)) != NULL){
    char * currDescr = alarmProperties->printData(prop);
    char *token = strtok(currDescr, ":");

    if(strcmp(token, "TRIGGER") == 0)
    {
      token = strtok(NULL, "\n");
      char * trigger = malloc(sizeof(char)*strlen(token) + 1);
      strcpy(trigger, token);
      strcpy(newAlarm->trigger, trigger);
      //printf("UID: %s\n", newEvent->UID);
    }

    if(strcmp(token, "ACTION") == 0)
    {
      token = strtok(NULL, "\n");
      //DateTime * dtStamp = createDateTime(token);
      strcpy(newAlarm->action,token);
      //printf("ACTIOOON: %s\n", newAlarm->action );

    }


    free(currDescr);
  }
  //printf("Length: %d\n", getLength(alarmProperties));
  return newAlarm;

}


char* printContentLine(void *toBePrinted){
  char * contentLine = malloc(sizeof(char)* strlen(toBePrinted) + 1);
  strcpy(contentLine, (char*)toBePrinted);
  return contentLine;
}

int compareContentLine(const void *first, const void *second){

  return 1;
}

void deleteContentLine(void *toBeDeleted){
  free(toBeDeleted);
}
