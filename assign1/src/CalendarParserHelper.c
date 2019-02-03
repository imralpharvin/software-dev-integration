#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode checkFile (char * filename)
{
  if(filename == NULL || strlen(filename) == 0)
  {
    return INV_FILE;
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
    return INV_FILE;
  }

  char * string = malloc(strlen(filename) + 1);
  strcpy(string, filename);

  char * token = strtok(string, ".");

  token = strtok(NULL, ".");

  if(strcmp(token, "ics") != 0)
  {
    printf("INVALID EXTENSION\n");
    free(string);
    return INV_FILE;

  }
  free(string);

  FILE * file = fopen(filename, "r");
  char line[100];
  fgets(line, sizeof(line), file);
  if( line[strlen(line)-2] != '\r' || line[strlen(line)-1] != '\n')
  {

    printf("INVALID LINE ENDINGS\n");
    fclose(file);
    return INV_FILE;
  }

  fclose(file);

  return OK;
}

ICalErrorCode checkCalendar(List * contentLines)
{
  char * firstLine = contentLines->printData(contentLines->head->data);
  char * lastLine = contentLines->printData(contentLines->tail->data);

  if(strcmp(firstLine, "BEGIN:VCALENDAR") != 0 || strcmp(lastLine, "END:VCALENDAR") != 0)
  {
    printf("WRONG TAGS\n");
    free(firstLine);
    free(lastLine);
    return INV_CAL;
  }

  free(firstLine);
  free(lastLine);

  return OK;
}

ICalErrorCode checkEvent(Event * newEvent)
{

  ListIterator iter = createIterator(newEvent->alarms);
  void* elem;
  while((elem = nextElement(&iter)) != NULL)
  {
    Alarm * currAlarm = (Alarm *) elem;
    if(checkAlarm(currAlarm) != OK)
    {
      return INV_ALARM;
    }


  }
  if(strlen(newEvent->creationDateTime.date) == 0|| strlen(newEvent->creationDateTime.time) == 0 )
  {
    printf("INVALID DATE TIME\n");
    return INV_DT;
  }

  if(strlen(newEvent->startDateTime.date) == 0|| strlen(newEvent->startDateTime.time) == 0 )
  {
    printf("INVALID DATE TIME\n");
    return INV_DT;
  }

  if(strlen(newEvent->UID) == 0)
  {
    printf("NO UID\n");
    return INV_EVENT;
  }
  return OK;
}

ICalErrorCode checkAlarm(Alarm * newAlarm)
{
    if(strlen(newAlarm->trigger) == 0 || strlen(newAlarm->action) == 0)
    {
      printf("INVALID ALARM\n");
      return INV_ALARM;
    }
    return OK;
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

ICalErrorCode createDateTime (char * dtLine, DateTime ** theDateTime)
{
  //allocate memory for date time struct
  DateTime * newDateTime = malloc(sizeof(DateTime)) ;
  strcpy(newDateTime->time, "");
  strcpy(newDateTime->date, "");
  newDateTime->UTC = true;

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
    //printf("UTC: %d\n", newDateTime->UTC);
    free(copy);
  }
  free(string);

  *theDateTime = newDateTime;

  return OK;
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

ICalErrorCode createEvent(List * eventLines, Event **theEvent)
{
  //Initialize event struct
  Event * newEvent = malloc(sizeof(Event));
  List * eventProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);
  strcpy(newEvent->UID, "");

  bool uidParsed = false;
  bool createParsed = false;
  bool startParsed = false;

  //Iterate through lines relating to event
  ListIterator iter = createIterator(eventLines);
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = eventLines->printData(elem);
    char * toSplit = eventLines->printData(elem);
    char * token = strtok(toSplit, ":");
    //Break if event ends
    if(strcmp(currDescr, "END:VEVENT") == 0 )
    {
      free(toSplit);
      free(currDescr);
      break;
    }
    //If property name is UID
    else if((strcmp(token, "UID") == 0))
    {
      token = strtok(NULL, ":");

      if(token == NULL)
      {
        free(toSplit);
        free(currDescr);
        freeList(eventProperties);
        freeList(alarms);
        free(newEvent);
        return INV_EVENT;
      }
      strcpy(newEvent->UID, token);
      uidParsed = true;

    }
    //If property name is dtStamp
    else if(strcmp(token, "CREATED") == 0)
    {
      token = strtok(NULL, ":");

      if(token == NULL)
      {
        free(toSplit);
        free(currDescr);
        freeList(eventProperties);
        freeList(alarms);
        free(newEvent);
        return INV_DT;
      }

      DateTime * dtStamp;
      ICalErrorCode err = createDateTime(token, &dtStamp);

      if(err != OK)
      {
        free(toSplit);
        free(currDescr);
        freeList(eventProperties);
        freeList(alarms);
        free(newEvent);
        return err;
      }
      newEvent->creationDateTime = *dtStamp;

      deleteDate(dtStamp);
      createParsed = true;
    }
    //If property name is dtstamp
    else if(strcmp(token, "DTSTART") == 0)
    {
      token = strtok(NULL, "\n");
      if(token == NULL)
      {
        free(toSplit);
        free(currDescr);
        freeList(eventProperties);
        freeList(alarms);
        free(newEvent);
        return INV_DT;
      }
      DateTime * dtStart;

      ICalErrorCode err = createDateTime(token, &dtStart);

      if(err != OK)
      {
        free(toSplit);
        free(currDescr);
        freeList(eventProperties);
        freeList(alarms);
        free(newEvent);
        return err;
      }
      newEvent->startDateTime = *dtStart;

      deleteDate(dtStart);
      startParsed = true;
    }
    //If property name begins with alarm
    else if(strcmp(token, "BEGIN") == 0)
    {
      token = strtok(NULL, "\n");
      if(strcmp(token, "VALARM") == 0)
      {
          List * alarmLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);

          while((elem = nextElement(&iter)) != NULL)
          {
            char * alarmLine = eventLines->printData(elem);
            insertBack(alarmLines, alarmLine);
            if(strcmp(alarmLine, "END:VALARM") ==0 )
            {
              break;
            }
            if(strcmp(alarmLine, "END:VEVENT") ==0 || strcmp(alarmLine, "END:VCALENDAR") ==0  ||  strcmp(alarmLine, "BEGIN:VALARM") ==0 || strcmp(alarmLine, "BEGIN:VEVENT") ==0  )
            {
              free(toSplit);
              free(currDescr);
              freeList(eventProperties);
              freeList(alarms);
              freeList(alarmLines);
              free(newEvent);
              return INV_ALARM;
            }
          }
          Alarm * newAlarm;
          ICalErrorCode err = createAlarm(alarmLines, &newAlarm);
          if(err != OK)
          {
            free(toSplit);
            free(currDescr);
            freeList(eventProperties);
            freeList(alarms);
            freeList(alarmLines);
            free(newEvent);
            return err;
          }
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

  if(uidParsed == false || startParsed == false || createParsed == false)
  {
    freeList(eventProperties);
    freeList(alarms);
    free(newEvent);
    return INV_EVENT;
  }

  newEvent->properties = eventProperties;
  newEvent->alarms = alarms;

  ICalErrorCode err = checkEvent(newEvent);
  if(err != OK)
  {
    freeList(eventProperties);
    freeList(alarms);
    free(newEvent);
    return err;
  }
  *theEvent = newEvent;

  return OK;
}

ICalErrorCode createAlarm(List * alarmLines, Alarm **theAlarm)
{
  Alarm * newAlarm = malloc(sizeof(Alarm));
  strcpy(newAlarm->action, "");
  newAlarm->trigger = malloc(sizeof(char));
  strcpy(newAlarm->trigger, "");


  bool actionParsed = false;
  bool triggerParsed = false;

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
      if(token == NULL)
      {
        free(currDescr);
        free(propertyLine);
        free(newAlarm->trigger);
        free(newAlarm);
        freeList(alarmProperties);
        return INV_ALARM;
      }
      newAlarm->trigger = realloc(newAlarm->trigger, sizeof(char)*strlen(token) + 1);
      strcat(newAlarm->trigger, token);

      triggerParsed = true;
    }
    else if(strcmp(token, "ACTION") == 0)
    {
      token = strtok(NULL, "\n");
      if(token == NULL)
      {
        free(currDescr);
        free(propertyLine);
        free(newAlarm->trigger);
        free(newAlarm);
        freeList(alarmProperties);
        return INV_ALARM;
      }
      strcpy(newAlarm->action,token);

      actionParsed = true;
    }
    else
    {
      Property * alarmProperty = createProperty(propertyLine);
      insertBack(alarmProperties, alarmProperty);
    }
    free(propertyLine);
    free(currDescr);

  }


  if(actionParsed == false || triggerParsed == false)
  {
    free(newAlarm->trigger);
    free(newAlarm);
    freeList(alarmProperties);
    return INV_ALARM;
  }
  newAlarm->properties = alarmProperties;
  *theAlarm = newAlarm;
  return OK;
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
