#include <stdio.h>
#include <CalendarParser.h>

int main(void)
{

  Calendar * theCalendar;

  ICalErrorCode err = createCalendar("../uploads/megaCal1.ics", &theCalendar);

  

      printf("%s\n\n", printError(err));
    printf("%s\n\n", eventListToJSON(theCalendar->events));
  printf("%s\n\n", eventListToJSONv2(theCalendar->events));

    printf("THIS>: %s\n", filenameToEventListJSON("../uploads/"));

    printf("%s\n\n",  propertyListToJSON(theCalendar->properties));




  Event * theEvent = theCalendar->events->head->data;

  //printf("%s\n", eventToJSON(theEvent));
  printf("%s\n", eventToJSONv2(theEvent));

  printf("%s\n\n",  propertyToJSON(theEvent->properties->head->data));
  printf("%s\n\n",  propertyListToJSON(theEvent->properties));


  Alarm * theAlarm = theEvent->alarms->head->data;
  printf("%s\n", alarmToJSON(theAlarm));

  printf("%s\n", alarmListToJSON(theEvent->alarms));


  printf("%s\n", filenameToJSON("../uploads/testCalSimpleUTC.ics"));
    printf("%s\n", filenameToJSON("../uploads/testCalSimpleUTC1.ics"));
  printf("WORKS\n");

  createAndWriteCalendar("../uploads/new.ics",  "{\"version\":2,\"prodID\":\"-//Mozilla.org/NONSGML Mozilla Calendar V1.1//EN\",\"numProps\":2,\"numEvents\":2}", "{\"UID\":\"1234\"}","{\"date\":\"20151002\",\"time\":\"100000\",\"isUTC\":false}", "{\"date\":\"20151002\",\"time\":\"100000\",\"isUTC\":true}");

  DateTime dt = JSONToDT("{\"date\":\"20151002\",\"time\":\"100000\",\"isUTC\":true}");
  printf("DATETIME: %s %s %d\n", dt.date, dt.time, dt.UTC);
  return 0;
}
