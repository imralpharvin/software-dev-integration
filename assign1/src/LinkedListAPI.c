#include <stdio.h>
#include "LinkedListAPI.h"


List* initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second))
{
  List *newList = malloc(sizeof(List));
  newList->head = NULL;
  newList->tail = NULL;
  newList->deleteData = deleteFunction;
  newList->printData = printFunction;
  newList->compare = compareFunction;
  newList->length = 0;
  return newList;
}

Node* initializeNode(void* data)
{
  Node *newNode = malloc(sizeof(Node));
  newNode->data = data;
  newNode->previous = NULL;
  newNode->next = NULL;

  return newNode;
}

void insertFront(List* list, void* toBeAdded)
{
  if (list == NULL || toBeAdded == NULL){
		return;
	}

	(list->length)++;

	Node* newNode = initializeNode(toBeAdded);

    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
		newNode->next = list->head;
        list->head->previous = newNode;
    	list->head = newNode;
    }
}

void insertBack(List* list, void* toBeAdded)
{
  if(list == NULL)
  {
    return;
  }

  Node *backNode = initializeNode(toBeAdded);
  (list->length)++;
  if(list->head == NULL && list->tail == NULL)
  {
    list->head = backNode;
    list->tail = backNode;
    return;
  }

  list->tail->next = backNode;
  backNode->previous = list->tail;
  list->tail = backNode;
}

void freeList(List* list)
{
  clearList(list);
  free(list);
}

void clearList(List* list)
{
  if(list == NULL)
  {
    return;
  }

  if(list->head == NULL)
  {

    free(list);
    return;
  }

  while(list->head != NULL)
  {
    Node *tempNode = NULL;
    tempNode = list->head;
    list->head = list->head->next;
    tempNode->next = NULL;
    tempNode->previous = NULL;
    if(tempNode->data != NULL)
    {
      list->deleteData(tempNode->data);
    }
    free(tempNode);
  }
  list->head = NULL;
  list->tail = NULL;
}

void insertSorted(List* list, void* toBeAdded)
{
  if (list == NULL || toBeAdded == NULL)
   {
     return;
   }

   if ((list->head == NULL && list->tail == NULL) || (list->compare(list->head->data, toBeAdded) >= 0))
   {
     insertFront(list, toBeAdded);
   }

   else
   {
     Node *tempNode = list->head;
     Node *beforeNode = NULL;

     while (tempNode != NULL && (list->compare(toBeAdded, tempNode->data) > 0))
     {
       beforeNode = tempNode;
       tempNode = tempNode->next;
     }

     if (tempNode == NULL)
     {
       insertBack(list, toBeAdded);
     }

     else
     {
       Node *betweenNode = initializeNode(toBeAdded);

       betweenNode->next = tempNode;
       betweenNode->previous = beforeNode;
       tempNode->previous = betweenNode;
       beforeNode->next = betweenNode;
     }
 }
}

void* deleteDataFromList(List* list, void* toBeDeleted)
{
  return NULL;
}

void* getFromFront(List* list)
{
  if(list == NULL || list->head == NULL)
  {
    printf("List empty when getting from front\n");
    return NULL;
  }

  else
  {
    return list->head->data;
  }
}

void* getFromBack(List* list)
{
  if(list == NULL || list->tail == NULL)
  {
    return NULL;
  }
  else
  {
    return list->tail->data;
  }
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
  return list->length;
}

void* findElement(List * list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord)
{
  return NULL;
}
