#include <stdio.h>
#include <string.h>

#include "CalendarParserHelper.h"
#include "CalendarParser.h"

char* printFunc(void *toBePrinted){
  char * point = ((Property*)toBePrinted)->propName;
  strcat(point, ((Property*)toBePrinted)->propDescr);
	return point;
}

int compareFunc(const void *first, const void *second){

  return 1;
}

void deleteFunc(void *toBeDeleted){
  free(toBeDeleted);
}
