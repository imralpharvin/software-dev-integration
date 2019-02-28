#include <stdio.h>
#include "CalendarParser.h"

void validatePrint(Calendar * pCalendar);

int main()
{

  FILE * fp = fopen("files.txt", "r");

  char line[200];
  while (fgets(line, sizeof(line), fp)){

    line[strlen(line)-1] = '\0';
    printf("FILE: %s | ", line);
    Calendar * pCalendar;

    ICalErrorCode error = createCalendar(line, &pCalendar);

    if (error == OK)
    {
      deleteCalendar(pCalendar);

    }

      char * printErr = printError(error);
      printf("%s", printErr);
      free(printErr);

  }

  fclose(fp);

//ASSIGNMENT 2


FILE * fp1 = fopen("files1.txt", "r");

char line1[200];
while (fgets(line1, sizeof(line1), fp1)){

  line1[strlen(line1)-1] = '\0';
  printf("FILE: %s | ", line1);
  Calendar * pCalendar;

  ICalErrorCode error = createCalendar(line, &pCalendar);

  if (error == OK)
  {
    ICalErrorCode err = writeCalendar("calendar.ics", pCalendar);
    char * printErr = printError(err);
    printf("WRITE CALENDAR: %s", printErr);
    free(printErr);
    deleteCalendar(pCalendar);

  }

    char * printErr = printError(error);
    printf("%s", printErr);
    free(printErr);

}

fclose(fp1);
  char * printErr;

 Calendar * pCalendar;
  ICalErrorCode err = createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  printErr = printError(err);
  printf("CREATE CALENDAR: %s", printErr);
  free(printErr);
  char * printCal = printCalendar(pCalendar);
  printf("\n%s", printCal);
  free(printCal);
/*
  err = writeCalendar("calendar.ics", pCalendar);
  printErr = printError(err);
  printf("WRITE CALENDAR: %s", printErr);
  free(printErr);
  deleteCalendar(pCalendar);

  err = createCalendar("calendar.ics", &pCalendar);
  printErr = printError(err);
  printf("CREATE CALENDAR FROM WRITE: %s", printErr);
  free(printErr);
  printCal = printCalendar(pCalendar);
  printf("\n%s", printCal);
  free(printCal);*/

  err = writeCalendar("calendar2.ics", pCalendar);
  printErr = printError(err);
  printf("WRITE CALENDAR: %s", printErr);
  free(printErr);

  err = writeCalendar("calendar2.ics", NULL);
  printErr = printError(err);
  printf("WRITE CALENDAR: %s", printErr);
  free(printErr);

  err = writeCalendar(NULL, pCalendar);
  printErr = printError(err);
  printf("WRITE CALENDAR: %s", printErr);
  free(printErr);

  err = writeCalendar("", pCalendar);
  printErr = printError(err);
  printf("WRITE CALENDAR: %s", printErr);
  free(printErr);

  //GOOD CALENDAR
  printf("Valid Calendar: OK | ");
  validatePrint(pCalendar);
  //Missing Version
  pCalendar->version = 0;
  printf("\nMissing version: INV_CAL | ");
  validatePrint(pCalendar);
  pCalendar->version = 2.0;
  printf("OK version: OK | ");
  validatePrint(pCalendar);

  //Missing PRODID
  strcpy(pCalendar->prodID, "");
  printf("\nMissing prodID: INV_CAL | ");
  validatePrint(pCalendar);
  //Extra PRODID
  printf("Exceed prodID: INV_CAL | ");
  strcpy(pCalendar->prodID, "oC7gAiCqirtPEUq3Z8FD00laVPYnvO6CXHIBNIJlJL7zBsbjmTwWmU9qFWcGoDIupRwTHNruO5mDVu7b7OZXJwRaWhDvizkapTiU6FXrdHOM3S6SqZYFKE2uw9Ispuo2T0xzwKx1oCkzhdNQxWNvTmBN5TAmFOkepjnWNosY3naxj7sRLY1ZmKEjGsUgevtwEPlanKhknQbki37BtJ6Yjlc6iROfSMlNtjLc1P9xvQ3pznNAJkRMKgHF8Hq4ETG6eDN2perjpADVOnYcdjAsYEZ4HH1nFiSlxgOiJz8c6oPSMMF8wz7vyxCVlT91MzTdnApo7pWrjcsrOrw2I3CLdmmA8dUmnGkOIJ6k1sKjCtHHJEfYEdzyjrQ9W0tJB54xKRrDjOxSOz4B0c86fo65CqW4hCA8yukBn2HjaFXQLQGHfe2V2sZt5lWBNg3WUSPCmZrbipFeguKRx7f3nwNc9UgiRBaog1fx0frhTNlK9OYzNsfIYKQwLzX7YVsNUI8uQf51ptia2qSUvSyDZvnbghBzUp9u58rQkmSDGhjcGAbp7JnaJvrlq19jso0ebcJCbE4qw4LtZ2iaS8DsG2t49tsqgQhenF5UC4T7BPxPacVLzUYJe4e9PMWHjUwaY8ahZuLc0jpyKUsigEBRGVw6sLYnLxVLqysr2SEMl2hIGFpxL8elkoE3AIRXIQ3WL0ZtQEXAf7ZsvNotgRKPM8FKzGH8pS3HAD1GTUT6QbvSE3Hk1wMpYbDrL7FLz0D10cuzxDxNLYyD9xc2gUSjQltmugjTdjdEQFEoHeJRCk1ewm2PCyunSoAywrMp3B3xc6I5BY53qMslVJ7q5kdQG9EiLLg2MjRK3Ij2UOXBWh1vunAkHVnwVB4FQSzjJfpYXTG17UfENrvZWRXqQl4SjY4tzhwfNf2D3qEzP8EH5OH00ZLLkNQrtoJRyobRC3XzbXBtW2hvmOEx0cIPcDyGPDciZQhUdadaAETg4iotT4aOE");
  validatePrint(pCalendar);
  printf("OK prodID: OK | ");
  strcpy(pCalendar->prodID, "7gAiCqirtPEUq3Z8FD00laVPYnvO6CXHIBNIJlJL7zBsbjmTwWmU9qFWcGoDIupRwTHNruO5mDVu7b7OZXJwRaWhDvizkapTiU6FXrdHOM3S6SqZYFKE2uw9Ispuo2T0xzwKx1oCkzhdNQxWNvTmBN5TAmFOkepjnWNosY3naxj7sRLY1ZmKEjGsUgevtwEPlanKhknQbki37BtJ6Yjlc6iROfSMlNtjLc1P9xvQ3pznNAJkRMKgHF8Hq4ETG6eDN2perjpADVOnYcdjAsYEZ4HH1nFiSlxgOiJz8c6oPSMMF8wz7vyxCVlT91MzTdnApo7pWrjcsrOrw2I3CLdmmA8dUmnGkOIJ6k1sKjCtHHJEfYEdzyjrQ9W0tJB54xKRrDjOxSOz4B0c86fo65CqW4hCA8yukBn2HjaFXQLQGHfe2V2sZt5lWBNg3WUSPCmZrbipFeguKRx7f3nwNc9UgiRBaog1fx0frhTNlK9OYzNsfIYKQwLzX7YVsNUI8uQf51ptia2qSUvSyDZvnbghBzUp9u58rQkmSDGhjcGAbp7JnaJvrlq19jso0ebcJCbE4qw4LtZ2iaS8DsG2t49tsqgQhenF5UC4T7BPxPacVLzUYJe4e9PMWHjUwaY8ahZuLc0jpyKUsigEBRGVw6sLYnLxVLqysr2SEMl2hIGFpxL8elkoE3AIRXIQ3WL0ZtQEXAf7ZsvNotgRKPM8FKzGH8pS3HAD1GTUT6QbvSE3Hk1wMpYbDrL7FLz0D10cuzxDxNLYyD9xc2gUSjQltmugjTdjdEQFEoHeJRCk1ewm2PCyunSoAywrMp3B3xc6I5BY53qMslVJ7q5kdQG9EiLLg2MjRK3Ij2UOXBWh1vunAkHVnwVB4FQSzjJfpYXTG17UfENrvZWRXqQl4SjY4tzhwfNf2D3qEzP8EH5OH00ZLLkNQrtoJRyobRC3XzbXBtW2hvmOEx0cIPcDyGPDciZQhUdadaAETg4iotT4aOE");
  validatePrint(pCalendar);
  //Empty event list

  printf("\nLIST: %d\n", getLength(pCalendar->events));
  clearList(pCalendar->events);
  printf("LIST: %d\n", getLength(pCalendar->events));
  printf("Empty List: INV_CAL | ");
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);
  printf("List Not empty: OK | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Calscale > 1: INV_CAL | ");
  createCalendar("testFiles/invCalendar/dupCALSCALE.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Method > 1: INV_CAL | ");
  createCalendar("testFiles/invCalendar/dupMETHOD.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Invalid Property: INV_CAL | ");
  createCalendar("testFiles/invCalendar/invProp.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Invalid Property: INV_CAL | ");
  createCalendar("testFiles/invCalendar/invProp.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Empty Property: INV_CAL | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  Property * emptyProperty = malloc(sizeof(Property) + 3);
  strcpy(emptyProperty->propName, "X-PROP");
  strcpy(emptyProperty->propDescr, "");
  insertBack(pCalendar->properties, emptyProperty);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("NULL Property List: INV_CAL | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  freeList(pCalendar->properties);
  pCalendar->properties = NULL;
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("NULL Event List: INV_CAL | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  freeList(pCalendar->events);
  pCalendar->events = NULL;
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("NULL Obj: INV_CAL | ");
  pCalendar = NULL;
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  Event * theEvent = pCalendar->events->head->data;
  printf("\nUID: %s\n",theEvent->UID);
  strcpy(theEvent->UID, "");
  printf("UID: %s\n",theEvent->UID);
  printf("Empty UID: INV_EVENT | ");
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Invalid Property: INV_EVENT | ");
  err = createCalendar("testFiles/invEvent/invProperty.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Duplicate Property: INV_EVENT | ");
  err = createCalendar("testFiles/invEvent/dupProperty.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("DtEnd DUration: INV_EVENT | ");
  err = createCalendar("testFiles/invEvent/dtendDuration.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  Property * theProperty = theEvent->properties->head->data;
  printf("PROPERTY NAME: %s\n",theProperty->propName);
  printf("PROPERTY DESC: %s\n",theProperty->propDescr);
  strcpy(theProperty->propDescr, "");
  printf("PROPERTY DESC: %s\n",theProperty->propDescr);
  printf("Empty Property: INV_EVENT | ");
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);


  printf("Properties Null: INV_EVENT | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  freeList(theEvent->properties);
  theEvent->properties = NULL;
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("alarms Null: INV_EVENT | ");
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  freeList(theEvent->alarms);
  theEvent->alarms = NULL;
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);


  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  Alarm *theAlarm = theEvent->alarms->head->data;
  printf("\nACTION: %s\n",theAlarm->action);
  strcpy(theAlarm->action, "");
  printf("ACTION: %s\n",theAlarm->action);
  printf("Empty Action: INV_ALARM | ");
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);


  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  theAlarm = theEvent->alarms->head->data;
  printf("TRIGGER: %s\n",theAlarm->action);
  strcpy(theAlarm->action, "");
  printf("TRIGGER: %s\n",theAlarm->action);
  printf("Empty Trigger: INV_ALARM | ");
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Invalid Property: INV_ALARM | ");
  err = createCalendar("testFiles/invAlarm/invProperty.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Duplicate Property: INV_ALARM | ");
  err = createCalendar("testFiles/invAlarm/dupProperty.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  printf("Repeat Duration: INV_ALARM | ");
  err = createCalendar("testFiles/invAlarm/repeatDuration.ics", &pCalendar);
  validatePrint(pCalendar);
  deleteCalendar(pCalendar);

  //Module 3
  createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  theEvent = pCalendar->events->head->data;
  DateTime dt = theEvent->creationDateTime;
  printf("\ndtToJSON\n");
  char * jString = dtToJSON(dt);
  printf("%s\n\n", jString);
  free(jString);

  printf("\ndtToJSON2\n");
  dt.UTC = false;
  jString = dtToJSON(dt);
  printf("%s\n\n", jString);
  free(jString);

  printf("eventToJSON\n");
  jString = eventToJSON(theEvent);
  printf("%s\n\n", jString);
  free(jString);

  List * eventList = pCalendar->events;
  printf("eventListToJSON\n");
  jString = eventListToJSON(eventList);
  printf("%s\n\n", jString);
  free(jString);


  pCalendar->version = 4.6;
  printf("calendarToJSON\n");
  jString = calendarToJSON(pCalendar);
  printf("%s\n\n", jString);
  free(jString);

    deleteCalendar(pCalendar);

  printf("JSONtoCalendar\n");
  Calendar * newCalendar;
  newCalendar = JSONtoCalendar("{\"version\":2,\"prodID\":\"-//hacksw/handcal//NONSGML v1.0//EN\"}");
  printf("VERSION: %d\n", (int)newCalendar->version);
  printf("PRODID: %s\n\n", newCalendar->prodID);


  printf("JSONtoEvent\n");
  Event * newEvent;
  newEvent = JSONtoEvent("{\"UID\":\"1234\"}");
  printf("UID: %s\n", newEvent->UID);
  deleteEvent(newEvent);

  printf("JSONtoEvent2\n");
  newEvent = JSONtoEvent("{\"UID\":\"\"}");
  printf("UID: %s\n\n", newEvent->UID);


  addEvent(newCalendar, newEvent);
  newEvent = newCalendar->events->tail->data;
  printf("UID: %s\n", newEvent->UID);

  deleteCalendar(newCalendar);

  return 0;
}



void validatePrint(Calendar * pCalendar)
{
  char * printErr;
  ICalErrorCode err = validateCalendar(pCalendar);
  printErr = printError(err);
  printf("%s", printErr);
  free(printErr);

}
