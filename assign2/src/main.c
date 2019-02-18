#include <stdio.h>
#include "CalendarParser_A2temp2.h"

int main()
{

  Calendar * pCalendar;

  ICalErrorCode error = createCalendar("testCalEvtPropAlm.ics", &pCalendar);

  if (error == OK)
  {
    char * calendarInfo = printCalendar(pCalendar);
    printf("%s" ,calendarInfo);
    free(calendarInfo);

  }
  else
  {
    char * printErr = printError(error);
    printf("%s", printErr);

  }

  char * printErr;
  error = writeCalendar("calendar.ics", pCalendar);
  printErr = printError(error);
  printf("WRITE CALENDAR: %s\n", printErr);
  free(printErr);

  error = validateCalendar(pCalendar);
  printErr = printError(error);
  printf("VALIDATE CALENDAR: %s\n", printErr);
  free(printErr);

  deleteCalendar(pCalendar);
  return 0;
}
