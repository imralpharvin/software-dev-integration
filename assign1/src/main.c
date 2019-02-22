#include <stdio.h>
#include "CalendarParser.h"

int main()
{

  FILE * fp = fopen("files.txt", "r");

    char line[200];
    while (fgets(line, sizeof(line), fp)){

      line[strlen(line)-1] = '\0';
      printf("FILE: %s | ", line);
      Calendar * pCalendar;

      ICalErrorCode error = createCalendar(line, &pCalendar);

      if(error == OK )
      {
        deleteCalendar(pCalendar);
      }

        char * printErr = printError(error);
        printf("%s", printErr);
        free(printErr);


    }

  fclose(fp);

  return 0;
}
