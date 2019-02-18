#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  /*ICalErrorCode error = createCalendar("invEvtProp2.ics", &pCalendar);*/
  ICalErrorCode error = createCalendar("megaCal1.ics", &pCalendar);
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
