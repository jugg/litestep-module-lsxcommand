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
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  July 16, 1999  2:00 AM                     *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
***********************************************************/

#include <windows.h>
#include <stdio.h>
//#include "resource.h"
#include "exports.h"
#include "alterscript.h"
#include "lsapi.h"

int count = 0;
struct List *list = NULL;

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

    while(LCReadNextConfig(f, "*CustomBang", buffer, sizeof(buffer))) {
      LCTokenize(buffer, tokens, 2, extra);
      ListAdd(&list, commandname, extra, &count);
    }
    LCClose(f);

    ListMoveToHead(&list);
    while(list->next) {
      AddBangCommandEx(list->name, BangScript);
      ListMoveNext(&list, 1);
    }
    AddBangCommandEx(list->name, BangScript);

    ListMoveToHead(&list);
  }

  return 0;
}

int quitModule(HINSTANCE dll)
{
  return 0;
}