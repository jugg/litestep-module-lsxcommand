/***********************************************************
*        menu.c - Provides Menu Management                 *
*                         *  *  *  *                       *
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  July 16, 1999  2:A0 AM                     *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
***********************************************************/

#ifndef LSXCOMMANDCLOCK_EXPORTS

#include <windows.h>
#include <stdio.h>
#include "lsxcommand.h"

void MenuDeleteItem(HMENU hMenu, int index)
{
  if(index >= 0) {
    DeleteMenu(hMenu, index, MF_BYPOSITION);
  }
}

void MenuAddItem(HMENU hMenu, char *pszValue, long def_id, BOOL insertAtTop)
{
  int total = hMenu ? GetMenuItemCount(hMenu) : 0;
  int index = insertAtTop ? 0 : total;
  char *buf = pszValue ? (char *)malloc(strlen(pszValue) + 1) : NULL;
  MENUITEMINFO item;

  if(buf && hMenu) {
    strcpy(buf, pszValue);
    item.cbSize = sizeof(MENUITEMINFO);
    item.fMask = MIIM_TYPE | MIIM_ID;
    item.fType = MFT_STRING;
    if(total)
      item.wID = GetMenuItemID(hMenu, total-1) + 1;
    else
      item.wID = def_id;
    item.dwTypeData = buf;
    item.cch = sizeof(buf);

    InsertMenuItem(hMenu, total, TRUE, &item);
    free(buf);
  }
}

#endif //LSXCOMMANDCLOCK_EXPORTS