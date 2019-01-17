#include <stdio.h>
#include "LinkedListAPI.h"


List* initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second))
{
  return NULL;
}

Node* initializeNode(void* data)
{
  return NULL;
}

void insertFront(List* list, void* toBeAdded)
{

}

void insertBack(List* list, void* toBeAdded)
{

}

void freeList(List* list)
{

}

void clearList(List* list)
{

}

void insertSorted(List* list, void* toBeAdded)
{

}

void* deleteDataFromList(List* list, void* toBeDeleted)
{
  return NULL;
}

void* getFromFront(List* list)
{
  return NULL;
}

void* getFromBack(List* list)
{
  return NULL;
}

char* toString(List* list)
{
  return NULL;
}

/*
ListIterator createIterator(List* list)
{
  return ;
}
*/

void* nextElement(ListIterator* iter)
{
  return NULL;
}


int getLength(List* list)
{
  return 0;
}

void* findElement(List * list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord)
{
  return NULL;
}
