/***********************************************************
*        history.c - Provides History Management           *
*                         *  *  *  *                       *
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  June 2, 1999  2:30 AM                      *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
***********************************************************/

#include <windows.h>
#include <stdio.h>
#include "lsxcommand.h"


/***********************************************************
* void HistoryMoveNext()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int i                                               *
*      The number of entries to move.                      *
*                         *  *  *  *                       *
* Moves i entries forward in the list.  Note: i may be     *
* negative.                                                *
***********************************************************/

void HistoryMoveNext(struct History **hist, int i)
{
  int j;

  if(*hist) {
    if(i >= 0)
      for(j = 1; j <= i; j++) {
        if((*hist)->next)
          *hist = (struct History *)(*hist)->next;
        else        
          return;
      }
    else
      for(j = -1; j >= i; j--) {
        if((*hist)->prev)
          *hist = (struct History *)(*hist)->prev;
        else
          return;
      }
  }
}


/***********************************************************
* void HistoryMovePrev()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int i                                               *
*      The number of entries to move.                      *
*                         *  *  *  *                       *
* Moves i entries backware in the list.  Note: i may be    *
* negative.                                                *
***********************************************************/

void HistoryMovePrev(struct History **hist, int i)
{
  HistoryMoveNext(hist, -1*i);
}


/***********************************************************
* void HistoryMovetoHead()                                 *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Moves the list pointer to the head of the list           *
***********************************************************/

void HistoryMoveToHead(struct History **hist)
{
  if(*hist)
    while((*hist)->prev)
      *hist = (struct History *)(*hist)->prev;
}


/***********************************************************
* void HistoryMovetoTail()                                 *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Moves the list pointer to the end of the list           *
***********************************************************/

void HistoryMoveToTail(struct History **hist)
{
  if(*hist)
    while((*hist)->next)
      *hist = (struct History *)(*hist)->next;
}


/***********************************************************
* int HistoryFindIndexOf()                                 *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - char *pszValue                                      *
*      Pointer to a path to search for                     *
*                         *  *  *  *                       *
* Searches the list for a particular entry and stops when  *
* it gets to the first one with a match.  Returns the      *
* index of the found entry, 0 upon reaching the end of the *
* list, -1 upon error.                                     *
***********************************************************/

int HistoryFindIndexOf(struct History **hist, char *pszValue)
{
  int index=0;
  struct History *temp = *hist;

  if(*hist && pszValue) {
    HistoryMoveToHead(hist);
    
    while((*hist)->next) {
      ++index;
      if(!stricmp((*hist)->path, pszValue))
        return index;
      HistoryMoveNext(hist, 1);
    }

    ++index;
    if(!stricmp((*hist)->path, pszValue))
      return index;

    *hist = temp;
    return 0;
  }

  *hist = temp;
  return -1;
}


/***********************************************************
* BOOL HistoryMoveToEntry()                                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - char *pszValue                                      *
*      Pointer to a path to search for                     *
*    - BOOL tokenize                                       *
*      Only compare the first token (delimited by spaces / *
*      tabs.                                               *
*                         *  *  *  *                       *
* Searches the list for a particular entry and stops when  *
* it gets to the first one with a match. The difference    *
* from this function and FindIndexOf() is it searches the  *
* first n characters of each History entry, where n is the *
* length of pszValue.  Returns TRUE upon finding a match,  *
* FALSE if it gets to the end of the list or upon error.   *
***********************************************************/

BOOL HistoryMoveToEntry(struct History **hist, char *pszValue, BOOL tokenize)
{
  char *buf, *p;

  if(*hist && pszValue) {
    HistoryMoveToHead(hist);

    while((*hist)->next) {
      if(tokenize) {
        buf = (char *)malloc(strlen((*hist)->path) + 1);
        strcpy(buf, (*hist)->path);
        p = strtok(buf, "\t ");
        if(p && !_stricmp(p, pszValue))
          return TRUE;
        HistoryMoveNext(hist, 1);
        free(buf);
      } else {
        if(!_stricmp((*hist)->path, pszValue))
          return TRUE;
        HistoryMoveNext(hist, 1);
      }
    }

    if(tokenize) {
      buf = (char *)malloc(strlen((*hist)->path) + 1);
      strcpy(buf, (*hist)->path);
      p = strtok(buf, "\t ");
      if(p && !_stricmp(p, pszValue)) {
        free(buf);
        return TRUE;
      }
      free(buf);
    } else
      if(!_stricmp((*hist)->path, pszValue))
        return TRUE;

    return FALSE;
  }

  return FALSE;
}


/***********************************************************
* struct History *HistoryAdd()                             *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - char *data                                          *
*      Pointer to the path to add to the history or search *
*      engine list.                                        *
*    - int *count                                          *
*      Integer with which to keep track of the number of   *
*      elements in the list                                *
*                         *  *  *  *                       *
* Adds a history, search engine, or alias record to the    *
* list.  It also keeps the null path at the tail of the    *
* history so that the history behaves correctly.  Returns  *
* a pointer to the current list element.                   *
***********************************************************/

struct History *HistoryAdd(struct History **hist, char *pszValue, int *count)
{
  char *szData = pszValue ? (char *)malloc(strlen(pszValue) + 1) : NULL;
  struct History *temp = NULL;

  if(pszValue) {
    strcpy(szData, pszValue);
    if(*hist) {
      HistoryMoveToTail(hist);
      if(!(*((*hist)->path))) {
        temp = *hist;
        HistoryMovePrev(hist, 1);
      } else temp = NULL;
      if(*hist == temp) {
        (*hist)->path = szData;
        HistoryAdd(hist, "", NULL);
      } else {
        (*hist)->next = (struct History *)malloc(sizeof(struct History));
        if(temp)
          temp->prev = (*hist)->next;
        (*hist)->next->next = temp;
        (*hist)->next->prev = (struct History *)*hist;
        (*hist)->next->path = szData;
      }
    } else {
      *hist = (struct History *)malloc(sizeof(struct History));
      (*hist)->next = NULL;
      (*hist)->prev = NULL;
      (*hist)->path = szData;
    }

    *hist = ((*hist)->next) ? (struct History *)(*hist)->next : (struct History *)(*hist);
    if(count)
      ++(*count);
  }

	return *hist;
}


/***********************************************************
* void HistoryFree()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Frees the memory used by one entry in the list.          *
***********************************************************/

void HistoryFree(struct History **hist)
{
  if(*hist) {
    if((*hist)->path)
      free((*hist)->path);

    free(*hist);
  }
}


/***********************************************************
* void HistoryRemoveEntry()                                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int *count                                          *
*      Pointer to the integer with which to keep track of  *
*      the number of elements in this list                 *
*                         *  *  *  *                       *
* Removes the current entry from the list and manipulates  *
* the list appropriately to keep the list in tact.         *
***********************************************************/

void HistoryRemoveEntry(struct History **hist, int *count)
{
  struct History *temp, *temp2;

  if(*hist) {
    if(!(*hist)->prev) {
      temp = (*hist)->next;
      HistoryFree(hist);
      *hist = temp;
      if(*hist) (*hist)->prev = NULL;
    } else {
      if(!(*hist)->next) {
        temp = (struct History *)(*hist)->prev;
        HistoryFree(hist);
        *hist = (struct History *)temp;
        if(*hist) (*hist)->next = NULL;
      } else {
        temp = (struct History *)(*hist)->prev;
        temp2 = (struct History *)(*hist)->next;
        HistoryFree(hist);
        temp->next = (struct History *)temp2;
        temp2->prev = (struct History *)temp;
        *hist = temp;
      }
    }
    if(count)
      --(*count);
  }
}


/***********************************************************
* struct History *HistoryRemoveAll()                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int *count                                          *
*      Pointer to the integer with which to keep track of  *
*      the number of elements in this list                 *
*                         *  *  *  *                       *
* Removes all entries from the list - should return NULL   *
***********************************************************/

struct History *HistoryRemoveAll(struct History **hist, int *count)
{
  HistoryMoveToHead(hist);
  while(*hist) {
    HistoryRemoveEntry(hist, count);
  }
  return *hist;
}