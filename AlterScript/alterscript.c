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
*        LSXCommand - Extended LSCommand Module            *
* This module adds to the functionality of limpid's        *
* LSCommand module.  Among the extentions are a resizable  *
* history, AutoCompletion, Internet Searching, Aliases,    *
* calculator functionality, backgrounds, transparency, and *
* clock functionality.  Maybe this is starting to bloat..  *
* Heck, this thing can even use the WinAmp scrolling title *
* feature.                                                 *
*                         *  *  *  *                       *
* Last Update:  July 16, 1999  2:00 AM                     *
***********************************************************/

#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "exports.h"
#include "alterscript.h"

/******************************************************************************
*	MSVC++ 6.0                                                                *
*	Go to Tools -> Options -> Directories                                     *
*	Select "Include file" in the list box and add the path to the directory   *
*	where you store your litestep core source files.                          *
*                                                                             *
*	Don't forget to do the same for the library file.                         *
*                                                                             *
*	Please do this to help portability and to support some development        *
*   standards.                                                                *
******************************************************************************/
#include <lsapi/lsapi.h>

int count = 0, numfiles = 0;
struct List *list = NULL, *scriptfiles = NULL;

void BangScript(HWND caller, char *name, char *args)
{
  char *p, *p2, *cmd;

  if(ListMoveToEntry(&list, name, 1)) {
    cmd = (char *)malloc(strlen(list->command) + 1);
    strcpy(cmd, list->command);
    p = p2 = cmd;

    while(*p2 != '|' && *p2)
      ++p2;

    if(*p2) {
      *p2 = '\0';
      ++p2;
    }
    
    while(p && *p) {
      ParseBangCommand(NULL, "!COMMAND", p);
      p = p2;
      
      while(*p2 != '|' && *p2)
        ++p2;

      if(*p2) {
        *p2 = '\0';
        ++p2;
      } else {
        ParseBangCommand(NULL, "!COMMAND", p);
        break;
      }
    }

    free(cmd);
  }
}

void ScanCustomBangs(char *path, char *buffer, char **tokens, char *extra)
{
  FILE *f = fopen(path, "r");
  if(f) {
    while(fgets(buffer, 4095, f)) {
      if(!_strnicmp(buffer, "*CustomBang", strlen("*CustomBang"))) {
        LCTokenize(buffer, tokens, 2, extra);
        if(extra[strlen(extra) - 1] == '\n')
          extra[strlen(extra) - 1] = '\0';
        ListAdd(&list, tokens[1], extra, &count);
      }
    }
    fclose(f);

    ListMoveToHead(&list);
    while(list->next) {
      AddBangCommandEx(list->name, BangScript);
      ListMoveNext(&list, 1);
    }
    AddBangCommandEx(list->name, BangScript);
  }
}

int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd)
{
  return initModuleEx(parent, dll, wd->lsPath);
}

int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath)
{
  FILE *f = LCOpen(NULL);
  char buffer[4096], name[] = "*CustomBang", commandname[128], extra[4096], *tokens[2];

  if(f) {
    tokens[0] = name;
    tokens[1] = commandname;

    while(LCReadNextCommand(f, buffer, 4095)) {
      if(!_strnicmp(buffer, "LoadScript", strlen("LoadScript"))) {
        LCTokenize(buffer, tokens, 2, extra);
        ListAdd(&scriptfiles, NULL, commandname, &numfiles);
      }
    }

    LCClose(f);
    ListMoveToHead(&scriptfiles);

    if(numfiles) {
      while(scriptfiles->next) {
        ScanCustomBangs(scriptfiles->command, buffer, tokens, extra);
        ListMoveNext(&list, 1);
      }
      ScanCustomBangs(scriptfiles->command, buffer, tokens, extra);

      ListMoveToHead(&list);
      ListRemoveAll(&scriptfiles, &numfiles);
    }
  }

  return 0;
}

int quitModule(HINSTANCE dll)
{
  /*
  ListMoveToHead(&list);
  while(list->next) {
    RemoveBangCommand(list->name);
  }
  */
  ListRemoveAll(&list, &count);
  return 0;
}