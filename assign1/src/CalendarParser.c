/* Name: Ralph Arvin De Castro
  Student ID: 0923223

*/

#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
  if(validateFile(fileName) != OK){ //Check file
    return INV_FILE;
  }

  Calendar *theCalendar = malloc(sizeof(Calendar)); //Initialize calendar members
  *obj = theCalendar;
  theCalendar->version = 0;
  strcpy(theCalendar->prodID, "");
  List * properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * events = initializeList(&printEvent, &deleteEvent, &compareEvents);
  theCalendar->events = events;
  theCalendar->properties = properties;

  List * contentLines = icsParser(fileName); //Parse content lines to a list
  bool versionParsed = false; //Checks if required members are parsed
  bool prodIdParsed = false;

  ListIterator iter = createIterator(contentLines);   //Iterates through content lines
	void* elem = nextElement(&iter);
  char * firstLine = contentLines->printData(elem);
  if (strcmp(firstLine, "BEGIN:VCALENDAR") != 0) {
      free(firstLine);
      deleteCalendar(theCalendar);
      freeList(contentLines);
      return INV_CAL;
  }
  free(firstLine);
  char * lastLine = contentLines->printData(contentLines->tail->data);
  if (strcmp(lastLine, "END:VCALENDAR") != 0) {
      free(lastLine);
      deleteCalendar(theCalendar);
      freeList(contentLines);
      return INV_CAL;
  }
  free(lastLine);


	while((elem = nextElement(&iter)) != NULL){
    char * currDescr = contentLines->printData(elem);

    if(strcmp(currDescr, "END:VCALENDAR") == 0 ) {   //Break Loop when calendar is done
      free(currDescr);
      break;
    }
    else if(strcmp(currDescr, "END:VEVENT") == 0  || strcmp(currDescr, "END:VALARM") == 0 ){
      free(currDescr);
      deleteCalendar(theCalendar);
      freeList(contentLines);
      return INV_CAL;
    }
    char * token = strtok(currDescr, determineDelimiter(currDescr));
    if(strcmp(token, "VERSION") == 0){//Find version
      token = strtok(NULL, "\n");
      if(versionParsed == true ){ //Duplicated version
        free(currDescr);
        freeList(contentLines);
        deleteCalendar(theCalendar);
        return DUP_VER;
      } else if(token != NULL && strcmp(token, "0.0") == 0) { //If version is 0.0
        theCalendar->version = 0.0;
        versionParsed = true;
      } else if(token == NULL || atof(token) == 0){ //Missing value or string not a float
        free(currDescr);
        freeList(contentLines);
        deleteCalendar(theCalendar);
        return INV_VER;
      } else {
        theCalendar->version = atof(token); //Assign the float to the version pointer
        versionParsed = true; //Version is parsed
      }
    } else if(strcmp(token, "PRODID") == 0){ //Production ID
      token = strtok(NULL, "\n");
      if(prodIdParsed == true ){ //Duplicate prod id
        deleteCalendar(theCalendar);
        free(currDescr);
        freeList(contentLines);
        return DUP_PRODID;
      } else if(token == NULL){ //missing
        deleteCalendar(theCalendar);
        free(currDescr);
        freeList(contentLines);
        return INV_PRODID;
      }
      strcpy(theCalendar->prodID, token);
      prodIdParsed = true;
    } else if(strcmp(token, "BEGIN") == 0){ //If begin
      token = strtok(NULL, "\n");
      if(strcmp(token, "VEVENT") == 0) {//If vevent
        List * eventLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine); //Find all content lines enclosing the event
        while((elem = nextElement(&iter)) != NULL){
          char * eventLine = contentLines->printData(elem);
          if(strcmp(eventLine, "END:VEVENT") ==0 ){
            free(eventLine);
            break;
          } else if(strcmp(eventLine, "END:VCALENDAR") ==0 || strcmp(eventLine, "BEGIN:VEVENT") ==0 ){
              free(eventLine);
              deleteCalendar(theCalendar);
              free(currDescr);
              freeList(eventLines);
              freeList(contentLines);
              return INV_EVENT;
          }
          insertBack(eventLines, eventLine);
        }
        Event * newEvent;
        ICalErrorCode err = createEvent(eventLines, &newEvent); //Create event
        if(err != OK){
          deleteCalendar(theCalendar);
          free(currDescr);
          freeList(contentLines);
          freeList(eventLines);
          return err;
        }
        insertBack(events, newEvent);
        freeList(eventLines);
      }
    } else {
      char * propertyLine = contentLines->printData(elem);
      Property * newProperty = createProperty(propertyLine);
      insertBack(properties, newProperty);
      free(propertyLine);
    }
    free(currDescr);
  }
  if(versionParsed == false|| prodIdParsed == false || getLength(theCalendar->events) == 0) {
    deleteCalendar(theCalendar);
    freeList(contentLines);
    return INV_CAL;
  }
  freeList(contentLines);

  return OK;
}

void deleteCalendar(Calendar* obj)
{
  if(obj == NULL) {
    return;
  }
  freeList(obj->events);
  freeList(obj->properties);
  free(obj);
}

char* printCalendar(const Calendar* obj)
{
  if(obj == NULL){
    return NULL;
  }

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
  free(version);
  free(eventPrint);
  free(toPrint);

  return calendarInfo;
}

char* printError(ICalErrorCode err)
{
  char * ok = "OK\n";
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

  if(err == OK){
    error = malloc(sizeof(char) * strlen(ok) + 1);
    strcpy(error, ok);
  } else if(err == INV_FILE) {
    error = malloc(sizeof(char) * strlen(invFile) + 1);
    strcpy(error, invFile);
  } else if(err == INV_CAL){
    error = malloc(sizeof(char) * strlen(invCal) + 1);
    strcpy(error, invCal);
  } else if(err == INV_VER){
    error = malloc(sizeof(char) * strlen(invVer) + 1);
    strcpy(error, invVer);
  } else if(err == DUP_VER){
    error = malloc(sizeof(char) * strlen(dupVer) + 1);
    strcpy(error, dupVer);
  } else if(err == INV_PRODID){
    error = malloc(sizeof(char) * strlen(invProdid) + 1);
    strcpy(error, invProdid);
  } else if(err == DUP_PRODID) {
    error = malloc(sizeof(char) * strlen(dupProdid) + 1);
    strcpy(error, dupProdid);
  } else if(err == INV_EVENT) {
    error = malloc(sizeof(char) * strlen(invEvent) + 1);
    strcpy(error, invEvent);
  } else if(err == INV_DT) {
    error = malloc(sizeof(char) * strlen(invDateTime) + 1);
    strcpy(error, invDateTime);
  } else if(err == INV_ALARM) {
    error = malloc(sizeof(char) * strlen(invAlarm) + 1);
    strcpy(error, invAlarm);
  } else if(err == OTHER_ERROR) {
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
  strcat(toPrint, "UID:");
  strcat(toPrint, theEvent->UID);
  strcat(toPrint, "\n");
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
  if(toPrint->UTC == true) {
    strcat(toString, "YES\n");
  } else {
    strcat(toString, "NO\n");
  }

  return toString;
}

char * determineDelimiter(char * contentLine)
{
  char * colon = malloc(strlen(contentLine) + 1);
  char * semicolon = malloc(strlen(contentLine) + 1);
  strcpy(colon, contentLine);
  strcpy(semicolon, contentLine);
  char * colonToken = strtok(colon, ":");
  char * semicolonToken = strtok(semicolon, ";");
  if(strlen(colonToken) < strlen(semicolonToken)){
    free(colon);
    free(semicolon);
    return ":";
  }
  free(colon);
  free(semicolon);

  return ";";
}

ICalErrorCode validateFile (char * filename)
{
  if(filename == NULL || strlen(filename) == 0) { //If filename is NULL or empty string
    return INV_FILE;
  }

  FILE * fp =  fopen(filename, "r"); //If file doesnt exist
  if (fp){
    fclose(fp);
  } else{
    return INV_FILE;
  }

  char * string = malloc(strlen(filename) + 1); //If file doesnt have the ics extension
  strcpy(string, filename);
  char * token = strtok(string, ".");
  token = strtok(NULL, ".");
  if(strcmp(token, "ics") != 0){
    free(string);
    return INV_FILE;
  }
  free(string);

  FILE * file = fopen(filename, "r"); //If file has invalid line endings
  char line[100];
  while(fgets(line, sizeof(line), file) != NULL){
    if(line[strlen(line)-2] != '\r' || line[strlen(line)-1] != '\n') {
      fclose(file);
      return INV_FILE;
    }
  }
  fclose(file);

  return OK; //If no errors
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
    if(line[strlen(line)-2] == '\r'){
      line[strlen(line)-2] = '\0';

    }else{
      line[strlen(line)] = '\0';
    }

    contentLine = malloc(strlen(line) + 1);

    strcpy(contentLine, line);

    if(line[0] == ' ' || line[0] == '\t'){
      char *ps = line;
      ps++;
      previousLine = realloc(previousLine, strlen(ps) + strlen(previousLine) + 2);
      strcat(previousLine, ps);
      free(contentLine);
    }else if(line[0] == '.'){
      previousLine = realloc(previousLine, strlen(line) + strlen(previousLine) + 2);
      strcat(previousLine, line);
      free(contentLine);
    } else if(line[0] == ';' || strcmp(line, "") == 0 ){
      free(contentLine);
    } else {//No space or tab
      //if everything is folded already
      if(folded == true){
        insertBack(contentLines, previousLine);
        folded = false;
      }
      previousLine = malloc(strlen(contentLine) + 1);
      strcpy(previousLine, contentLine);
      free(contentLine);
      folded = true;
    }
  }
  insertBack(contentLines, previousLine);

  fclose(fp);
  return contentLines;
}

ICalErrorCode createDateTime (char * dtLine, DateTime ** theDateTime)
{
  DateTime * newDateTime = malloc(sizeof(DateTime)) ;   //allocate memory for date time struct
  *theDateTime = newDateTime;
  strcpy(newDateTime->time, "");
  strcpy(newDateTime->date, "");
  newDateTime->UTC = false;

  char * token = strtok(dtLine, "T");
  if(strlen(token) != 8){
    free(newDateTime);
    return INV_DT;
  }
  strcpy(newDateTime->date, token);

  token = strtok(NULL, "\n");
  if(strlen(token) > 7 || strlen(token) <6 ){
    free(newDateTime);
    return INV_DT;
  }
  strcpy(newDateTime->time, token);

  if(newDateTime->time[6] == 'Z' ){
    newDateTime->time[6] = '\0';
    newDateTime->UTC = true;
  }
  return OK;
}

Property * createProperty(char * contentLine)
{

  Property * newProperty = malloc(sizeof(Property));
  char *token = strtok(contentLine, determineDelimiter(contentLine));
  strcpy(newProperty->propName, token);
  token = strtok(NULL, "\n");
  newProperty = realloc(newProperty, sizeof(Property) + sizeof(char) *strlen(token) + 2);
  strcpy(newProperty->propDescr, token);
  return newProperty;
}

ICalErrorCode createEvent(List * eventLines, Event **theEvent)
{
  Event * newEvent = malloc(sizeof(Event));   //Initialize event struct
  *theEvent = newEvent;
  List * eventProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  List * alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);
  newEvent->properties = eventProperties;
  newEvent->alarms = alarms;
  strcpy(newEvent->UID, "");

  bool uidParsed = false; //flags
  bool createParsed = false;
  bool startParsed = false;

  ListIterator iter = createIterator(eventLines);   //Iterate through lines relating to event
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = eventLines->printData(elem);
    char *token = strtok(currDescr, determineDelimiter(currDescr));

    if(strcmp(currDescr, "END:VEVENT") == 0 ){
      free(currDescr);
      break;
    }

    if((strcmp(token, "UID") == 0)) {//If property name is UID
      token = strtok(NULL, "\n");
      if(token == NULL){
        free(currDescr);
        deleteEvent(newEvent);
        return INV_EVENT;
      }
      strcpy(newEvent->UID, token);
      uidParsed = true;
    } else if(strcmp(token, "DTSTAMP") == 0){//If property name is dtStamp
      token = strtok(NULL, "\n");
      if(token == NULL){
        free(currDescr);
        deleteEvent(newEvent);
        return INV_DT;
      }
      DateTime * dtStamp;
      ICalErrorCode err = createDateTime(token, &dtStamp);
      if(err != OK){
        free(currDescr);
        deleteEvent(newEvent);
        return err;
      }
      newEvent->creationDateTime = *dtStamp;
      deleteDate(dtStamp);
      createParsed = true;
    } else if(strcmp(token, "DTSTART") == 0){     //If property name is dtstamp
      token = strtok(NULL, "\n");
      if(token == NULL){
        free(currDescr);
        deleteEvent(newEvent);
        return INV_DT;
      }
      DateTime * dtStart;
      ICalErrorCode err = createDateTime(token, &dtStart);
      if(err != OK){
        free(currDescr);
        deleteEvent(newEvent);
        return err;
      }
      newEvent->startDateTime = *dtStart;
      deleteDate(dtStart);
      startParsed = true;
    } else if(strcmp(token, "BEGIN") == 0){ //If property name begins with alarm
      token = strtok(NULL, "\n");
      if(strcmp(token, "VALARM") == 0)  {
          List * alarmLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);
          while((elem = nextElement(&iter)) != NULL) {
            char * alarmLine = eventLines->printData(elem);
            insertBack(alarmLines, alarmLine);
            if(strcmp(alarmLine, "END:VALARM") ==0 ){
              break;
            }
            if(strcmp(alarmLine, "END:VEVENT") ==0 || strcmp(alarmLine, "END:VCALENDAR") ==0  ||  strcmp(alarmLine, "BEGIN:VALARM") ==0 || strcmp(alarmLine, "BEGIN:VEVENT") ==0  ){
              free(currDescr);
              deleteEvent(newEvent);
              freeList(alarmLines);
              return INV_ALARM;
            }
          }
          Alarm * newAlarm;
          ICalErrorCode err = createAlarm(alarmLines, &newAlarm);
          if(err != OK){
            free(currDescr);
            deleteEvent(newEvent);
            freeList(alarmLines);
            return err;
          }
          insertBack(alarms, newAlarm);
          freeList(alarmLines);
      }
    } else{
      token = strtok(NULL, "\n");
      if(token == NULL)
      {
        free(currDescr);
        deleteEvent(newEvent);
        return INV_EVENT;
      }
      char * propertyLine = eventLines->printData(elem);
      Property * eventProperty = createProperty(propertyLine);
      insertBack(eventProperties, eventProperty);
      free(propertyLine);
    }
    free(currDescr);
  }
  if(uidParsed == false || startParsed == false || createParsed == false){
    deleteEvent(newEvent);
    return INV_EVENT;
  }

  return OK;
}

ICalErrorCode createAlarm(List * alarmLines, Alarm **theAlarm)
{
  Alarm * newAlarm = malloc(sizeof(Alarm));
  strcpy(newAlarm->action, "");
  newAlarm->trigger = malloc(sizeof(char));
  strcpy(newAlarm->trigger, "");
  *theAlarm = newAlarm;
  List * alarmProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  newAlarm->properties = alarmProperties;

  bool actionParsed = false;
  bool triggerParsed = false;

  ListIterator iter = createIterator(alarmLines);
  void* elem;
  while((elem = nextElement(&iter)) != NULL){
    char * currDescr = alarmLines->printData(elem);

    if(strcmp(currDescr, "END:VALARM") == 0 ){
      free(currDescr);
      break;
    }
    char *token =  strtok(currDescr, determineDelimiter(currDescr));

    if(strcmp(token, "TRIGGER") == 0){
      token = strtok(NULL, "\n");
      if(token == NULL){
        free(currDescr);
        deleteAlarm(newAlarm);
        return INV_ALARM;
      }
      newAlarm->trigger = realloc(newAlarm->trigger, sizeof(char)*strlen(token) + 1);
      strcat(newAlarm->trigger, token);
      triggerParsed = true;
    } else if(strcmp(token, "ACTION") == 0){
      token = strtok(NULL, "\n");
      if(token == NULL){
        free(currDescr);
        deleteAlarm(newAlarm);
        return INV_ALARM;
      }
      strcpy(newAlarm->action,token);
      actionParsed = true;
    } else{
      token = strtok(NULL, "\n");
      if(token == NULL)
      {
        free(currDescr);
        deleteAlarm(newAlarm);
        return INV_ALARM;
      }
      char * propertyLine = alarmLines->printData(elem);
      Property * alarmProperty = createProperty(propertyLine);
      insertBack(alarmProperties, alarmProperty);
      free(propertyLine);
    }
    free(currDescr);
  }
  if(actionParsed == false || triggerParsed == false){
    deleteAlarm(newAlarm);
    return INV_ALARM;
  }
  return OK;
}

char* printContentLine(void *toBePrinted)
{
  char * contentLine = malloc(sizeof(char)* strlen(toBePrinted) + 1);
  strcpy(contentLine, (char*)toBePrinted);
  return contentLine;
}

int compareContentLine(const void *first, const void *second)
{
  return 0;
}

void deleteContentLine(void *toBeDeleted)
{
  free(toBeDeleted);
}
