/******************************************************************************
*                                                                             *
* This is a part of the LsxCommand LiteStep module Source code.               *
*                                                                             *
* Copyright (C) 1999-2000 Visigoth (Shaheen Gandhi in real life)              *
* Based on limpid's lscommand.                                                *
* Updated by blkhawk, ilmcuts, jesus_mjjg, rabidcow.                          *
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

/******************************************************************************
*                  menu.c - Provides Menu Management                          *
*                                   *  *  *  *                                *
*           Last Update:  July 16, 1999  2:A0 AM                              *
******************************************************************************/

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

  if(buf)
  {
    if (hMenu)
    {
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
    }

    free(buf);
  }
}

#endif //LSXCOMMANDCLOCK_EXPORTS