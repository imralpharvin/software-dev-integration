#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  ICalErrorCode error = createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  if (error == OK)
  {
    char * calendarInfo = printCalendar(pCalendar);
    printf("%s" ,calendarInfo);
    free(calendarInfo);

    deleteCalendar(pCalendar);
  }
  else
  {
    char * printErr = printError(error);
    printf("%s", printErr);
    free(printErr);
  }

/*
  DateTime newDT;

  strcpy(newDT.date , "ASDDSD");
  strcpy(newDT.time, "000000Z");

    printf("UTC: %d\n", newDT.UTC);
      printf("TIME: %s\n", newDT.time);
    newDT.UTC = true;


    printf("UTC: %d\n", newDT.UTC);
    printf("TIME: %s\n", newDT.time);
*/

  return 0;
}
