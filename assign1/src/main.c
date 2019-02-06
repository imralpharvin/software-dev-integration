#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  ICalErrorCode error = createCalendar("testCalSimpleUTCComments.ics", &pCalendar);
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
  return 0;
}
