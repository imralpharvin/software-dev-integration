#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

bool checkFile (char * filename)
{
  if(filename == NULL || strlen(filename) == 0)
  {
    return false;
  }

  FILE * fp;
  fp = fopen(filename, "r");
  if (fp)
  {
    fclose(fp);
  }
  else
  {
    printf("FILE DOES NOT EXIST");
    return false;
  }

  char * string = malloc(strlen(filename) + 1);
  strcpy(string, filename);

  char * token = strtok(string, ".");

  token = strtok(NULL, ".");
  printf("TOKEN: %s\n", token );

  if(strcmp(token, "ics") != 0)
  {
    printf("INVALID EXTENSION\n");
    free(string);
    return false;

  }
  free(string);

  FILE * file = fopen(filename, "r");
  char line[100];
  fgets(line, sizeof(line), file);
  if( line[strlen(line)-2] != '\r' || line[strlen(line)-1] != '\n')
  {

    printf("INVALID LINE ENDINGS\n");
    fclose(file);
    return false;
  }

  fclose(file);

  return true;
}

bool checkCalendar(List * contentLines)
{
  char * firstLine = contentLines->printData(contentLines->head->data);
  char * lastLine = contentLines->printData(contentLines->tail->data);

  if(strcmp(firstLine, "BEGIN:VCALENDAR") != 0 || strcmp(lastLine, "END:VCALENDAR") != 0)
  {
    printf("WRONG TAGS\n");
    free(firstLine);
    free(lastLine);
    return false;
  }

  free(firstLine);
  free(lastLine);

  return true;
}

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
  bool folded = false;
  while (fgets(line, sizeof(line), fp)){

    if(line[strlen(line)-2] == '\r')
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

    //If theres a space or tab
    if(line[0] == ' ' || line[0] == '\t')
    {
      char *ps = line;
      ps++;

      //printf("%s\n", ps);
      previousLine = realloc(previousLine, strlen(ps) + strlen(previousLine) + 2);
      strcat(previousLine, ps);
      free(contentLine);
    }
    else //No space or tab
    {
      //if everything is folded already
      if(folded == true)
      {
        insertBack(contentLines, previousLine);
        folded = false;
      }
      previousLine = malloc(strlen(contentLine) + 1);
      strcpy(previousLine, contentLine);
      free(contentLine);
      folded = true;
      //Insert content line
    }
  }
  insertBack(contentLines, previousLine);

  //printf("CONTENT LINES\n\n%s\n*********\n", toString(contentLines));

  fclose(fp);
  return contentLines;
}

DateTime * createDateTime (char * dtLine)
{
  //allocate memory for date time struct
  DateTime * newDateTime = malloc(sizeof(DateTime)) ;

  //Split string
  char * string = malloc(strlen(dtLine) + 1);
  strcpy(string, dtLine);

  char * token = strtok(string, "T");
  strcpy(newDateTime->date, token);

  token = strtok(NULL, "\n");

  strcpy(newDateTime->time, token);

  if(newDateTime->time[6] == 'Z' )
  {

    char * copy = malloc(strlen(newDateTime->time) + 1);
    strcpy(copy, newDateTime->time);
    newDateTime->UTC = true;

    strcpy(newDateTime->time, copy);
    free(copy);
  }
  free(string);

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
    else if((strcmp(token, "UID") == 0))
    {
      token = strtok(NULL, "\n");
      strcpy(newEvent->UID, token);
      //printf("UID: %s\n", newEvent->UID);
    }
    else if(strcmp(token, "DTSTAMP") == 0)
    {
      token = strtok(NULL, "\n");
      DateTime * dtStamp = createDateTime(token);
      newEvent->creationDateTime = *dtStamp;

      deleteDate(dtStamp);
    }

    else if(strcmp(token, "DTSTART") == 0)
    {
      token = strtok(NULL, "\n");
      DateTime * dtStart = createDateTime(token);
      newEvent->startDateTime = *dtStart;

      deleteDate(dtStart);
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
              break;
            }

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
/*

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

    }

    if(strcmp(token, "DTSTART") == 0)
    {
      token = strtok(NULL, "\n");
      DateTime * dtStart = createDateTime(token);
      newEvent->startDateTime = *dtStart;



      deleteDate(dtStart);
    }

    free(currDescr);
  }*/


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
    char * propertyLine = alarmLines->printData(elem);

    if(strcmp(currDescr, "END:VALARM") == 0 )
    {
      free(currDescr);
      free(propertyLine);
      break;
    }

    char *token = strtok(currDescr, ":");
    if(strcmp(token, "TRIGGER") == 0)
    {
      token = strtok(NULL, "\n");
      char * trigger = malloc(sizeof(char)*strlen(token) + 1);
      strcpy(trigger, token);
      strcpy(newAlarm->trigger, trigger);
      //printf("UID: %s\n", newEvent->UID);
    }
    else if(strcmp(token, "ACTION") == 0)
    {
      token = strtok(NULL, "\n");
      //DateTime * dtStamp = createDateTime(token);
      strcpy(newAlarm->action,token);
      //printf("ACTIOOON: %s\n", newAlarm->action );
    }
    else
    {
      Property * alarmProperty = createProperty(propertyLine);
      insertBack(alarmProperties, alarmProperty);


    }

    free(propertyLine);
    free(currDescr);

  }

  newAlarm->properties = alarmProperties;

/*
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
  }*/
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
