#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  Calendar * pCalendar;

  createCalendar("testCalEvtProp.ics", &pCalendar);
  //Property * toPrint = malloc(sizeof((Property*)pCalendar->properties->head->data));
  //toPrint = ;
  //printf("%s", ((Property*)pCalendar->properties->head->data)->propName);
  //printf("%d\n", getLength((List*)pCalendar->properties));
  printf("%s" ,printCalendar(pCalendar));
  //free(printCalendar(pCalendar));
  //printf("%s",toString(pCalendar->properties));
  //free(toString(pCalendar->properties));
  deleteCalendar(pCalendar);

  return 0;
}
