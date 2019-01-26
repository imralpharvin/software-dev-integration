#include <stdio.h>
#include <string.h>

#include "CalendarParser.h"
#include "CalendarParserHelper.h"


List * icsParser(char * fileName)
{
  FILE * fp;
  fp = fopen(fileName, "r");

  //process file line by line
  char line[75];

  //Fold ContentLines
  List * contentLines = initializeList(&printContentLine, &deleteContentLine, compareContentLine);
  char * previousLine;
  char * contentLine;
  int count = 0;
  while (fgets(line, sizeof(line), fp)){

    line[strlen(line)-1] = '\0';
    contentLine = malloc(strlen(line) + 1);
    strcpy(contentLine, line);

    if(line[0] == ' '){
      previousLine = realloc(previousLine, strlen(contentLine) + strlen(previousLine) + 2);
      strcat(previousLine, contentLine);
      free(contentLine);
    }
    else{
      if(count > 0)
      {
        insertBack(contentLines, previousLine);
      //Point previous to current if no new line
      }
      previousLine = malloc(strlen(contentLine) + 1);
      strcpy(previousLine, contentLine);
      free(contentLine);
      //Insert content line
      count ++;
    }
  }
  insertBack(contentLines, previousLine);

  fclose(fp);
  return contentLines;
}

Event * createEvent(List * eventLines)
{

}

char* printContentLine(void *toBePrinted){
  char * contentLine = malloc(sizeof(char)* strlen(toBePrinted) + 1);
  strcpy(contentLine, (char*)toBePrinted);
  return contentLine;
}

int compareContentLine(const void *first, const void *second){

  return 1;
}

void deleteContentLine(void *toBeDeleted){
  free(toBeDeleted);
}
