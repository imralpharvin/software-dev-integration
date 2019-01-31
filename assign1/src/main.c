#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  //createCalendar("testCalEvtProp.ics", &pCalendar);
  //createCalendar("testCalEvtPropAlm.ics", &pCalendar);
  if (createCalendar("test2.ics", &pCalendar) == OK)
  {


  //createCalendar("calendar.ics", &pCalendar);
    char * calendarInfo = printCalendar(pCalendar);
    printf("%s" ,calendarInfo);
    free(calendarInfo);

    deleteCalendar(pCalendar);
  }

  return 0;
}
