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

    line[strlen(line)-1] = '\0';
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

  token = strtok(NULL, "T");
  printf("%s\n", token);



  strcpy(newDateTime->time, token);
  if(token[6] == 'Z' )
  {
    printf("BEFORE: %s\n", newDateTime->time);
    newDateTime->UTC = true;
    printf("AFTER: %s\n", newDateTime->time);
  }






  return newDateTime;
}
Property * createProperty(char * contentLine)
{
  Property * newProperty = malloc(sizeof(Property));

  char *token = strtok(contentLine, ":");

  strcpy(newProperty->propName, token);
  token = strtok(NULL, ":");

  newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 2);
  strcpy(newProperty->propDescr, token);
  return newProperty;
}

Event * createEvent(List * eventLines)
{
  Event * newEvent = malloc(sizeof(Event));
  List * eventProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);

  ListIterator iter = createIterator(eventLines);
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = eventLines->printData(elem);

    if(strcmp(currDescr, "END:VEVENT") == 0 )
    {
      free(currDescr);
      break;
    }

    Property * eventProperty = createProperty(currDescr);
    insertBack(eventProperties, eventProperty);

    free(currDescr);
  }

  ListIterator iterProp = createIterator(eventProperties);
  void* prop;
  while((prop = nextElement(&iterProp)) != NULL){
    char * currDescr = eventProperties->printData(prop);
    char *token = strtok(currDescr, ":");

    if(strcmp(token, "UID") == 0)
    {
      token = strtok(NULL, ":");
      strcpy(newEvent->UID, token);
      printf("UID: %s\n", newEvent->UID);
    }

    if(strcmp(token, "DTSTAMP") == 0)
    {
      token = strtok(NULL, ":");
      DateTime * dtStamp = createDateTime(token);
      newEvent->creationDateTime = *dtStamp;

      printf("DTstampdate: %s\n", newEvent->creationDateTime.date);
      printf("DTstamptime: %s\n", newEvent->creationDateTime.time);
      printf("DTstamputc: %d\n", newEvent->creationDateTime.UTC);
      printf("DTstamputc: %s\n", newEvent->creationDateTime.UTC ? "true" : "false");
    }
  }


  newEvent->properties = eventProperties;

  return newEvent;
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
