#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  createCalendar("calendar.ics", &pCalendar);

  char * calendarInfo = printCalendar(pCalendar);
  printf("%s" ,calendarInfo);
  free(calendarInfo);

  deleteCalendar(pCalendar);

  return 0;
}
