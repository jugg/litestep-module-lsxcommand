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

#ifndef ALTERSCRIPT_H
#define ALTERSCRIPT_H

struct List {
  char *name;
  char *command;
  struct List *next, *prev;
};

/* List Services - List.c */
extern void ListMoveNext(struct List **hist, int i);
extern void ListMovePrev(struct List **hist, int i);
extern void ListMoveToHead(struct List **hist);
extern void ListMoveToTail(struct List **hist);
extern int ListFindIndexOf(struct List **hist, char *pszValue, int val);
extern BOOL ListMoveToEntry(struct List **hist, char *pszValue, int val);
extern struct List *ListAdd(struct List **hist, char *pszValue1, char *pszValue2, int *count);
extern void ListFree(struct List **hist);
extern void ListRemoveEntry(struct List **hist, int *count);
extern struct List *ListRemoveAll(struct List **hist, int *count);

#endif