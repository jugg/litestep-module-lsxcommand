/******************************************************************************
*                                                                             *
* This is a part of the AlterScript LiteStep module Source code.              *
*                                                                             *
* Copyright (C) 1999 Visigoth (Shaheen Gandhi in real life)                   *
* Look at the documentation for more informations.                            *
*                                                                             *
* This program is free software; you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation; either version 2 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program; if not, write to the Free Software                 *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   *
*                                                                             *
******************************************************************************/

/***********************************************************
*        list.c - Provides List Management                 *
*                         *  *  *  *                       *
* Last Update:  July 16, 1999  2:00 A M                    *
*                         *  *  *  *                       *
***********************************************************/

#include <windows.h>
#include <stdio.h>
#include "alterscript.h"


/***********************************************************
* void ListMoveNext()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int i                                               *
*      The number of entries to move.                      *
*                         *  *  *  *                       *
* Moves i entries forward in the list.  Note: i may be     *
* negative.                                                *
***********************************************************/

void ListMoveNext(struct List **hist, int i)
{
  int j;

  if(*hist) {
    if(i >= 0)
      for(j = 1; j <= i; j++) {
        if((*hist)->next)
          *hist = (struct List *)(*hist)->next;
        else        
          return;
      }
    else
      for(j = -1; j >= i; j--) {
        if((*hist)->prev)
          *hist = (struct List *)(*hist)->prev;
        else
          return;
      }
  }
}


/***********************************************************
* void ListMovePrev()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int i                                               *
*      The number of entries to move.                      *
*                         *  *  *  *                       *
* Moves i entries backware in the list.  Note: i may be    *
* negative.                                                *
***********************************************************/

void ListMovePrev(struct List **hist, int i)
{
  ListMoveNext(hist, -1*i);
}


/***********************************************************
* void ListMovetoHead()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Moves the list pointer to the head of the list           *
***********************************************************/

void ListMoveToHead(struct List **hist)
{
  if(*hist)
    while((*hist)->prev)
      *hist = (struct List *)(*hist)->prev;
}


/***********************************************************
* void ListMovetoTail()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Moves the list pointer to the end of the list            *
***********************************************************/

void ListMoveToTail(struct List **hist)
{
  if(*hist)
    while((*hist)->next)
      *hist = (struct List *)(*hist)->next;
}


/***********************************************************
* int ListFindIndexOf()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - char *pszValue                                      *
*      Pointer to a path to search for                     *
*    - int val                                             *
*      Tells which value in the element to compare.        *
*                         *  *  *  *                       *
* Searches the list for a particular entry and stops when  *
* it gets to the first one with a match.  Returns the      *
* index of the found entry, 0 upon reaching the end of the *
* list, -1 upon error.                                     *
***********************************************************/

int ListFindIndexOf(struct List **hist, char *pszValue, int val)
{
  int index=0;
  char *compval = NULL;
  struct List *temp = *hist;

  if(*hist && pszValue) {
    ListMoveToHead(hist);
    compval = val ? (*hist)->name : (*hist)->command;

    while((*hist)->next) {
      ++index;
      if(!stricmp(compval, pszValue))
        return index;
      ListMoveNext(hist, 1);
      compval = val ? (*hist)->name : (*hist)->command;
    }

    ++index;
    if(!stricmp(compval, pszValue))
      return index;

    *hist = temp;
    return 0;
  }

  *hist = temp;
  return -1;
}


/***********************************************************
* BOOL ListMoveToEntry()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - char *pszValue                                      *
*      Pointer to a path to search for                     *
*    - int val                                             *
*      Tells which value in the element to compare         *
*                         *  *  *  *                       *
* Searches the list for a particular entry and stops when  *
* it gets to the first one with a match.  Returns TRUE     *
* upon finding a match, FALSE if it gets to the end of the *
* list or upon error.                                      *
***********************************************************/

BOOL ListMoveToEntry(struct List **hist, char *pszValue, int val)
{
  char *compval = NULL;

  if(*hist && pszValue) {
    ListMoveToHead(hist);
    compval = val ? (*hist)->name : (*hist)->command;

    while((*hist)->next) {
      if(!_stricmp(compval, pszValue))
        return TRUE;
      ListMoveNext(hist, 1);
      compval = val ? (*hist)->name : (*hist)->command;
    }

    if(!_stricmp(compval, pszValue))
      return TRUE;

    return FALSE;
  }

  return FALSE;
}


/***********************************************************
* struct List *ListAdd()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
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

struct List *ListAdd(struct List **hist, char *pszValue1, char *pszValue2, int *count)
{
  char *szData1 = pszValue1 ? (char *)malloc(strlen(pszValue1) + 1) : NULL;
  char *szData2 = pszValue2 ? (char *)malloc(strlen(pszValue2) + 1) : NULL;
  struct List *temp = NULL;

  if(pszValue1 || pszValue2) {
    if(szData1) strcpy(szData1, pszValue1);
    if(szData2) strcpy(szData2, pszValue2);
    if(*hist) {
      ListMoveToTail(hist);
      (*hist)->next = (struct List *)malloc(sizeof(struct List));
      (*hist)->next->next = NULL;
      (*hist)->next->prev = (struct List *)*hist;
      (*hist)->next->name = szData1;
      (*hist)->next->command = szData2;
    } else {
      *hist = (struct List *)malloc(sizeof(struct List));
      (*hist)->next = NULL;
      (*hist)->prev = NULL;
      (*hist)->name = szData1;
      (*hist)->command = szData2;
    }

    *hist = ((*hist)->next) ? (struct List *)(*hist)->next : (struct List *)(*hist);
    if(count)
      ++(*count);
  }

	return *hist;
}


/***********************************************************
* void ListFree()                                          *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Frees the memory used by one entry in the list.          *
***********************************************************/

void ListFree(struct List **hist)
{
  if(*hist) {
    if((*hist)->name)
      free((*hist)->name);

    if((*hist)->command)
      free((*hist)->command);

    free(*hist);
  }
}


/***********************************************************
* void ListRemoveEntry()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int *count                                          *
*      Pointer to the integer with which to keep track of  *
*      the number of elements in this list                 *
*                         *  *  *  *                       *
* Removes the current entry from the list and manipulates  *
* the list appropriately to keep the list in tact.         *
***********************************************************/

void ListRemoveEntry(struct List **hist, int *count)
{
  struct List *temp, *temp2;

  if(*hist) {
    if(!(*hist)->prev) {
      temp = (*hist)->next;
      ListFree(hist);
      *hist = temp;
      if(*hist) (*hist)->prev = NULL;
    } else {
      if(!(*hist)->next) {
        temp = (struct List *)(*hist)->prev;
        ListFree(hist);
        *hist = (struct List *)temp;
        if(*hist) (*hist)->next = NULL;
      } else {
        temp = (struct List *)(*hist)->prev;
        temp2 = (struct List *)(*hist)->next;
        ListFree(hist);
        temp->next = (struct List *)temp2;
        temp2->prev = (struct List *)temp;
        *hist = temp;
      }
    }
    if(count)
      --(*count);
  }
}


/***********************************************************
* struct List *ListRemoveAll()                             *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct List **hist                                  *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*    - int *count                                          *
*      Pointer to the integer with which to keep track of  *
*      the number of elements in this list                 *
*                         *  *  *  *                       *
* Removes all entries from the list - should return NULL   *
***********************************************************/

struct List *ListRemoveAll(struct List **hist, int *count)
{
  if(*hist) {
    ListMoveToHead(hist);
    while(*hist) {
      ListRemoveEntry(hist, count);
    }
  }
  return *hist;
}
