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
#include <shlwapi.h>
#include <shlobj.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include "exports.h"
#include "lsxcommand.h"
#include "lsapi.h"

BOOL visible = FALSE;

#ifndef LSXCOMMANDCLOCK_EXPORTS

BOOL editfirst = FALSE, staticfirst = FALSE;
char *szApp = "LSXCommand", *defaultEngine = NULL;
int nHistoryEntries = 0, nSearchEngines = 0, nAliases = 0, nFiles = 0;
HMENU hSearchEngineMenu = NULL, hAliasMenu = NULL, hHistoryMenu = NULL, hContextMenu = NULL;
MENUITEMINFO item;
struct History *current = NULL, *searchEngines = NULL, *aliases = NULL, *files = NULL;

#else

char *szApp = "LSXCommandClock";

#endif //LSXCOMMANDCLOCK_EXPORTS

char *szModuleIniPath = NULL;
int nTimers = 0;
HINSTANCE hInst = NULL;
HWND hWnd = NULL, hText = NULL, hBG = NULL;
HFONT hFont = NULL;
HBRUSH hBGBrush = NULL, hHollowBrush = NULL;
HBITMAP hBGBitmap = NULL;
WNDPROC wpOld, wpBG;
struct CommandSettings *cs = NULL;


/***********************************************************
* struct CommandSettings *ReadSettings()                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - wharfDataType *wd                                   *
*      Pointer to Litestep's structure for general info.   *
*                         *  *  *  *                       *
* Retrieves settings in the RC file and stores it all in a *
* local structure. Returns a pointer to a new structure.   *
***********************************************************/

struct CommandSettings *ReadSettings(wharfDataType *wd)
{
  int offsetx, offsety;
  RECT screen;
	struct CommandSettings *settings = (struct CommandSettings *)malloc(sizeof(struct CommandSettings));
  szModuleIniPath = (char *)malloc(strlen(wd->lsPath) + strlen("\\MODULES.INI") + 1);
  strcpy(szModuleIniPath, wd->lsPath);
  strcat(szModuleIniPath, "\\MODULES.INI");

#ifndef LSXCOMMANDCLOCK_EXPORTS
	GetRCString("CommandTextFontFace",settings->TextFontFace,"Arial",256);
	settings->BGColor = GetRCColor("CommandBGColor",RGB(255,255,255));
	settings->TextColor = GetRCColor("CommandTextColor",RGB(0,0,0));
	settings->TextSize = GetRCInt("CommandTextSize",14);
	settings->x = GetRCInt("CommandX",0);
	settings->y = GetRCInt("CommandY",0);
	settings->width = GetRCInt("CommandWidth",160);
	settings->height = GetRCInt("CommandHeight",20);
  settings->MaxHistoryEntries = GetRCInt("CommandHistoryEntries", 10) + 1;
  settings->MaxHistoryMenuEntries = GetRCInt("CommandHistoryMenuEntries", settings->MaxHistoryEntries - 1 );
	settings->BorderSize = GetRCInt("CommandBorderSize",2);
  settings->ContextMenuStandardItems = GetRCInt("CommandContextMenuStandardItems", 0);
  offsetx = GetRCInt("CommandOffsetX", 0);
  offsety = GetRCInt("CommandOffsetY", 0);
	settings->BorderColor = GetRCColor("CommandBorderColor",RGB(102,102,102));
	settings->BevelBorder = GetRCBool("CommandBevelBorder",TRUE);
	settings->NoAlwaysOnTop = GetRCBool("CommandNotAlwaysOnTop",TRUE);
	settings->notmoveable = GetRCBool("CommandNotMoveable",TRUE);
	settings->NoClearOnCommand = GetRCBool("CommandNoClearOnCommand",TRUE);
	settings->HideOnCommand = GetRCBool("CommandHideOnCommand",TRUE);
	settings->HiddenOnStart = GetRCBool("CommandHiddenOnStart",TRUE);
	settings->ClearOnHide = GetRCBool("CommandClearOnHide",TRUE);
	settings->NoCursorChange = GetRCBool("CommandNoCursorChange",TRUE);
  settings->SelectAllOnFocus = GetRCBool("CommandSelectAllOnFocus", TRUE);
  settings->AssumeNetAddress = GetRCBool("CommandAssumeNetAddress", TRUE);
  settings->NoWarnOnError = GetRCBool("CommandNoWarnOnError", TRUE);
  settings->NoTabMicroComplete = GetRCBool("CommandNoTabMicroComplete", TRUE);
  settings->TabFileComplete = GetRCBool("CommandTabFileComplete", TRUE);
  settings->ClearHistoryOnStartup = GetRCBool("CommandClearHistoryOnStartup", TRUE);
  settings->NewestHistoryItemsOnTop = GetRCBool("CommandNewestHistoryItemsOnTop", TRUE);
  settings->ContextMenuAutoPopup = GetRCBool("CommandContextMenuAutoPopup", TRUE);
  settings->ContextMenuAboveBox = GetRCBool("CommandContextMenuAboveBox", TRUE);
  settings->ContextMenuExecute = GetRCBool("CommandContextMenuExecute", TRUE);
  settings->UnixHistory = GetRCBool("CommandUnixHistory", TRUE);
  settings->ExplorePaths = GetRCBool("CommandExplorePaths", TRUE);
  settings->SelectAllOnMouseFocus = GetRCBool("CommandSelectAllOnMouseFocus", TRUE);
  settings->AutoComplete = GetRCBool("CommandNoAutoComplete", FALSE);
  settings->CommaDelimiter = GetRCBool("CommandCommaDelimiter", TRUE);
  settings->RPNCalculator = GetRCBool("CommandRPNCalculator", TRUE);
  settings->Transparent = GetRCBool("CommandTransparentEditBox", TRUE);
  settings->ClockDisappears = GetRCBool("CommandClockDisappearsOnFocus", TRUE);
  settings->ScrollWinAmp = GetRCBool("CommandScrollWinAmpTitle", TRUE);
  settings->WinAmpDisappears = GetRCBool("CommandWinAmpDisappearsOnFocus", TRUE);
  settings->HideOnUnfocus = GetRCBool("CommandHideOnUnfocus", TRUE);
  GetRCString("CommandSearchEngineList", settings->SearchEngineList, "engines.list", sizeof(settings->SearchEngineList));
  GetRCString("CommandContextMenuOrder", settings->ContextMenuOrder, "012", sizeof(settings->ContextMenuOrder));
  GetRCString("CommandBackground", settings->Background, "", sizeof(settings->Background));
  GetRCString("CommandClock", settings->Clock, "", sizeof(settings->Clock));
  GetRCString("CommandSearchEngineBrowser", settings->BrowserPath, "", sizeof(settings->BrowserPath));
  //GetRCString("CommandTextAlign", settings->TextAlign, "Left Top", sizeof(settings->TextAlign));
#else //LSXCOMMANDCLOCK_EXPORTS
	GetRCString("CommandClockTextFontFace",settings->TextFontFace,"Arial",256);
	settings->BGColor = GetRCColor("CommandClockBGColor",RGB(255,255,255));
	settings->TextColor = GetRCColor("CommandClockTextColor",RGB(0,0,0));
	settings->TextSize = GetRCInt("CommandClockTextSize",14);
	settings->x = GetRCInt("CommandClockX",0);
	settings->y = GetRCInt("CommandClockY",0);
	settings->width = GetRCInt("CommandClockWidth",160);
	settings->height = GetRCInt("CommandClockHeight",20);
	settings->BorderSize = GetRCInt("CommandClockBorderSize",2);
  offsetx = GetRCInt("CommandClockOffsetX", 0);
  offsety = GetRCInt("CommandClockOffsetY", 0);
	settings->BorderColor = GetRCColor("CommandClockBorderColor",RGB(102,102,102));
	settings->BevelBorder = GetRCBool("CommandClockBevelBorder",TRUE);
	settings->NoAlwaysOnTop = GetRCBool("CommandClockNotAlwaysOnTop",TRUE);
	settings->notmoveable = GetRCBool("CommandClockNotMoveable",TRUE);
	settings->HiddenOnStart = GetRCBool("CommandClockHiddenOnStart",TRUE);
	settings->NoCursorChange = GetRCBool("CommandClockNoCursorChange",TRUE);
  settings->SelectAllOnFocus = GetRCBool("CommandClockSelectAllOnFocus", TRUE);
  settings->SelectAllOnMouseFocus = GetRCBool("CommandClockSelectAllOnMouseFocus", TRUE);
  settings->Transparent = GetRCBool("CommandClockTransparentEditBox", TRUE);
  settings->ScrollWinAmp = GetRCBool("CommandClockScrollWinAmpTitle", TRUE);
  settings->HideOnUnfocus = GetRCBool("CommandClockHideOnUnfocus", TRUE);
  GetRCString("CommandClockBackground", settings->Background, "", sizeof(settings->Background));
  GetRCString("CommandClockString", settings->Clock, "", sizeof(settings->Clock));
#endif //LSXCOMMANDCLOCK_EXPORTS
  
  /* Special Cases */

  /* If there's a bevel, then no border */
  if(settings->BevelBorder) settings->BorderSize = 0;
  
  /* No Background */
  if(!(*(settings->Background)))
    settings->Transparent = 0;

  GetClientRect(GetDesktopWindow(), &screen);

  /* Offsets */
  if(offsetx == 0) {
    if(settings->x < 0)
      settings->x = screen.right + settings->x;
  }
  else if(offsetx == 1)
    settings->x = screen.right / 2 - settings->width / 2 + settings->x;
  else
    settings->x = screen.right + settings->x;

  if(offsety == 0) {
    if(settings->y < 0)
      settings->y = screen.bottom + settings->y;
  }
  else if(offsety == 1)
    settings->y = screen.bottom / 2 - settings->height / 2 + settings->y;
  else
    settings->y = screen.bottom + settings->y;

	return settings;
}


/***********************************************************
* void ContextMenuInit()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Initializes the context menu.  This is called whenever   *
* one of the submenu handles is changed.                   *
***********************************************************/

#ifndef LSXCOMMANDCLOCK_EXPORTS
void ContextMenuInit()
{
  char buf[15], *p = cs->ContextMenuOrder, atom;
  int i, indices[7];

  for(i = 0; i < 3; ++i) {
    atom = *p;
    indices[i] = atoi(&atom);
    ++p;
  }

  if(cs->ContextMenuStandardItems == 1) {
    for(i = 0; i < 3; ++i)
      indices[i] += 4;
    indices[3] = 0;  //Cut
    indices[4] = 1;  //Copy
    indices[5] = 2;  //Paste
    indices[6] = 3;  //Separator
  } else if(cs->ContextMenuStandardItems == 2) {
    indices[6] = 3;
    indices[5] = 4;
    indices[4] = 5;
    indices[3] = 6;
  } else {
    indices[6] = 9;
    indices[5] = 9;
    indices[4] = 9;
    indices[3] = 9;
  }

  if(!hContextMenu) {
    item.cbSize = sizeof(MENUITEMINFO);
    item.dwTypeData = buf;
    item.cch = sizeof(buf);

    hContextMenu = CreatePopupMenu();
    /*strcpy(buf, "About LSXCommand");
    InsertMenuItem(hContextMenu, 0, TRUE, &item);
    item.fType = MFT_SEPARATOR;
    InsertMenuItem(hContextMenu, 1, TRUE, &item);*/

    for(i = 0; i < 9; ++i) {
      if(indices[0] == i) {
        item.fMask = MIIM_SUBMENU | MIIM_TYPE;
        item.hSubMenu = hHistoryMenu;
        item.fType = MFT_STRING;
        strcpy(buf, "&History");
        InsertMenuItem(hContextMenu, i, TRUE, &item);
      } else if(indices[1] == i) {
        item.fMask = MIIM_SUBMENU | MIIM_TYPE;
        item.hSubMenu = hAliasMenu;
        item.fType = MFT_STRING;
        strcpy(buf, "&Aliases");
        InsertMenuItem(hContextMenu, i, TRUE, &item);
      } else if(indices[2] == i) {
          item.fMask = MIIM_SUBMENU | MIIM_TYPE;
        item.hSubMenu = hSearchEngineMenu;
        item.fType = MFT_STRING;
        strcpy(buf, "&Search Engines");
        InsertMenuItem(hContextMenu, i, TRUE, &item);
      } else if(indices[3] == i) {
        strcpy(buf, "Cu&t");
        //InsertMenuItem(hContextMenu, i, TRUE, &item);
        AppendMenu(hContextMenu, MF_STRING, HMI_USER_CUT, buf);
      } else if(indices[4] == i) {
        strcpy(buf, "&Copy");
        //InsertMenuItem(hContextMenu, i, TRUE, &item);
        AppendMenu(hContextMenu, MF_STRING, HMI_USER_COPY, buf);
      } else if(indices[5] == i) {
        strcpy(buf, "&Paste");
        //InsertMenuItem(hContextMenu, i, TRUE, &item);
        AppendMenu(hContextMenu, MF_STRING, HMI_USER_PASTE, buf);
      } else if(indices[6] == i) {
        item.fMask = MIIM_TYPE;
        item.fType = MFT_SEPARATOR;
        item.hSubMenu = NULL;
        InsertMenuItem(hContextMenu, i, TRUE, &item);
      }
    }
  } else {
    item.cbSize = sizeof(MENUITEMINFO);
    item.fMask = MIIM_SUBMENU;

    item.hSubMenu = hHistoryMenu;
    SetMenuItemInfo(hContextMenu, indices[0], TRUE, &item);
    item.hSubMenu = hAliasMenu;
    SetMenuItemInfo(hContextMenu, indices[1], TRUE, &item);
    item.hSubMenu = hSearchEngineMenu;
    SetMenuItemInfo(hContextMenu, indices[2], TRUE, &item);
  }
}


/***********************************************************
* void CreateHistoryPopupMenu()                            *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Creates the history popup menu based on the current      *
* history.                                                 *
***********************************************************/

void CreateHistoryPopupMenu()
{
  int i = 0;
  struct History *temp = current;

  hHistoryMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE | MIIM_ID;
  item.fType = MFT_STRING;
  item.wID = HMI_USER_HISTORY;

  HistoryMoveToTail(&current);

  if(current) {
    while(i < cs->MaxHistoryMenuEntries && current->prev) {
      item.dwTypeData = current->prev->path;
      item.cch = sizeof(current->prev->path);
      if(cs->NewestHistoryItemsOnTop)
        InsertMenuItem(hHistoryMenu, i, TRUE, &item);
      else
        InsertMenuItem(hHistoryMenu, 0, TRUE, &item);
      ++item.wID;
      ++i;
      HistoryMovePrev(&current, 1);
    }
  }

  current = temp;
}


/***********************************************************
* void HistoryInit()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Loads past history entries from the modules.ini file.    *
***********************************************************/

void HistoryInit()
{
  BOOL sect_found=FALSE;
  char line[_MAX_PATH], *p;
  FILE *f;

  f = fopen(szModuleIniPath, "r");
  if(f) {
    while(fgets(line, _MAX_PATH - 1, f)) {
      line[strlen(line) - 1] = '\0';
      if(!stricmp(line, "[LSXCOMMAND]")) {
        sect_found = TRUE;
        break;
      }
    }

    if(sect_found) {
      while(fgets(line, _MAX_PATH - 1, f)) {
        if(!(*line) || *line == '[')
          break;

        line[strlen(line) - 1] = '\0';
        p = strtok(line, "=");
        p = strtok(NULL, "\0");
        HistoryAdd(&current, p, &nHistoryEntries);
      }
    }

    fclose(f);
  }

  HistoryAdd(&current, "", &nHistoryEntries);
  CreateHistoryPopupMenu();
}


/***********************************************************
* void ClearHistory()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Clears the virtual and 'physical' history                *
***********************************************************/

void ClearHistory(struct History **hist)
{
  int i = 0;
  char name[17];

  HistoryRemoveAll(hist, &nHistoryEntries);
  HistoryAdd(hist, "", &nHistoryEntries);
  DestroyMenu(hHistoryMenu);
  hHistoryMenu = CreatePopupMenu(); // Create blank menu, just like blank history

  for(i=0; i < cs->MaxHistoryEntries; i++) {
    _itoa(i, name, 10);
    WritePrivateProfileString("LSXCOMMAND", name, "", szModuleIniPath);
  }

  if(hContextMenu)
    ContextMenuInit();
}


/***********************************************************
* void WriteHistory()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Saves the current history list to the modules.ini file.  *
***********************************************************/

void WriteHistory()
{
  int i = 0;
  char name[17];
  struct History *temp = current;

  HistoryMoveToHead(&current);
  while(current->next) {   // We stop the last entry since it is NULL - this is intentional
    _itoa(i, name, 10);
    WritePrivateProfileString("LSXCOMMAND", name, current->path, szModuleIniPath);

    HistoryMoveNext(&current, 1);
    ++i;
  }

  current = temp;
}


/***********************************************************
* void CreateSearchEnginePopupMenu()                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Creates the search engine popup menu based on the        *
* current list of search engines.                          *
***********************************************************/

void CreateSearchEnginePopupMenu()
{
  int i = 0;
  char *p;

  hSearchEngineMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE | MIIM_ID;
  item.fType = MFT_STRING;
  item.wID = HMI_USER_SE;

  HistoryMoveToHead(&searchEngines);

  if(searchEngines) {
    while(searchEngines->next) {
      p = (char *)malloc(strlen(searchEngines->path) + 1);
      strcpy(p, searchEngines->path);
      item.dwTypeData = strtok(p, "\t ");
      item.cch = sizeof(p);
      InsertMenuItem(hSearchEngineMenu, i, TRUE, &item);
      ++item.wID;
      ++i;
      HistoryMoveNext(&searchEngines, 1);
      free(p);
    }

    p = (char *)malloc(strlen(searchEngines->path) + 1);
    strcpy(p, searchEngines->path);
    item.dwTypeData = strtok(p, "\t ");
    item.cch = sizeof(p);
    InsertMenuItem(hSearchEngineMenu, i, TRUE, &item);
    free(p);
  }
}


/***********************************************************
* void SearchEngineInit()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct CommandSettings *cs                          *
*      Pointer to a structure containing the options       *
*      specified by the step.rc                            *
*                         *  *  *  *                       *
* Initializes the list of Search Engines.  Also, it        *
* creates a popup menu of search engines.  If no search    *
* engine list is available, the feature will simply not    *
* work.                                                    *
***********************************************************/

void SearchEngineInit(struct CommandSettings *cs)
{
  char listline[512], *p;
  FILE *fpEngineList;

  fpEngineList = fopen(cs->SearchEngineList, "r");
  if(!fpEngineList)
    return;

  hSearchEngineMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE | MIIM_ID;
  item.fType = MFT_STRING;
  item.wID = HMI_USER_SE;

  for(;;) {
    if(!fgets(listline, 511, fpEngineList))
      break;

    p = strstr(listline, ";");
    if(p == listline)
      continue;

    if(p)
      *p = '\0';

    // strtok for Default.  Save the other token to defaultEngine
    if(!(_strnicmp("default", listline, strlen("default")*sizeof(char)))) {
      p = strtok(listline, "\t ");
      p = strtok(NULL, "\n");
      defaultEngine = (char *)malloc(strlen(p) + 1);
      strcpy(defaultEngine, p);
      continue;
    }

    if(*listline) {
      HistoryAdd(&searchEngines, listline, &nSearchEngines);
      p = strtok(listline, "\t ");
      item.dwTypeData = p;
      item.cch = sizeof(p);
      InsertMenuItem(hSearchEngineMenu, item.wID - HMI_USER_SE, TRUE, &item);
      ++item.wID;
    }
  }

  fclose(fpEngineList);
}


/***********************************************************
* void CreateAliasPopupMenu()                              *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Creates the aliases popup menu based on the current      *
* list of aliases.                                         *
***********************************************************/

void CreateAliasPopupMenu()
{
  int i = 0;
  char *p = NULL;

  hAliasMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE | MIIM_ID;
  item.fType = MFT_STRING;
  item.wID = HMI_USER_ALIAS;

  HistoryMoveToHead(&aliases);

  if(aliases) {
    while(aliases->next) {
      p = (char *)malloc(strlen(aliases->path) + 1);
      strcpy(p, aliases->path);
      item.dwTypeData = strtok(p, "\t ");
      item.cch = sizeof(aliases->path);
      InsertMenuItem(hAliasMenu, i, TRUE, &item);
      ++item.wID;
      HistoryMoveNext(&aliases, 1);
      free(p);
    }

    p = (char *)malloc(strlen(aliases->path) + 1);
    strcpy(p, aliases->path);
    item.dwTypeData = strtok(p, "\t ");
    item.cch = sizeof(aliases->path);
    InsertMenuItem(hAliasMenu, i, TRUE, &item);
    free(p);
  }
}


/***********************************************************
* void AliasInit()                                         *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Initializes the list of Aliases.  If no aliases are      *
* found, the feature simply doesn't work.                  *
***********************************************************/

void AliasInit()
{
  char buffer[_MAX_PATH + 20], name[20], path[_MAX_PATH], prefix[15], extra[_MAX_PATH];
  char* tokens[3];
  FILE *f = LCOpen(NULL);

  tokens[0] = prefix;
  tokens[1] = name;
  tokens[2] = path;

  hAliasMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE | MIIM_ID;
  item.fType = MFT_STRING;
  item.wID = HMI_USER_ALIAS;

  if(f && !feof(f)) {
    while(LCReadNextConfig(f, "*CommandAlias", buffer, sizeof(buffer))) {
      if(LCTokenize(buffer, tokens, 3, extra) >= 3) {
        strcpy(buffer, name);
        strcat(buffer, " ");
        strcat(buffer, path);
        if(*extra) {
          strcat(buffer, " ");
          strcat(buffer, extra);
        }
        HistoryAdd(&aliases, buffer, &nAliases);
        item.dwTypeData = name;
        item.cch = sizeof(name);
        InsertMenuItem(hAliasMenu, item.wID - HMI_USER_ALIAS, TRUE, &item);
        ++item.wID;
      }
    }
  }

  if(f)
    LCClose(f);
}


/***********************************************************
* void ParseSearchCommand()                                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *command                                       *
*      Search Engine string.                               *
*    - char *args                                          *
*      The search phrase for this engine                   *
*                         *  *  *  *                       *
* This function is called after the user has entered a     *
* search command.  It finds the engine to use, sets up the *
* URL to open in full, then executes the URL.              *
***********************************************************/

void ParseSearchCommand(char *command, char *args)
{
  char *p = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL, *p5 = args ? (char *)malloc(strlen(args)*3 + 1) : NULL;
  char hex[3];
  int n;

  if(searchEngines && args) {
    if(strlen(command) == 1) {
      if(!HistoryMoveToEntry(&searchEngines, defaultEngine, TRUE)) {
        MessageBox(NULL, "Could not find default search engine.  If you did\nnot specify one in your search engine list,\nplease do so.", "LSXCommand", MB_OK);
        return;
      }
    } else if(strlen(command) > 1) {
      ++command;
      if(!HistoryMoveToEntry(&searchEngines, command, TRUE)) {
        MessageBox(NULL, "Could not find specified search engine.", "LSXCommand", MB_OK);
        return;
      }
    }

    // This is hell, but I like to save memory...
    p = args;
    strcpy(p5, "");

    while(*p) {
      switch(*p) {
      case ' ':
        strcat(p5, "+");
        break;
  
      case '<':
      case '>':
      case '#':
      case '%':
      case '{':
      case '}':
      case '|':
      case '\\':
      case '^':
      case '~':
      case '[':
      case ']':
      case '`':
      case ';':
      case '/':
      case ':':
      case '@':
      case '(':
      case ')':
      case '\"':
      case '&':
      case '\'':
        _itoa(*p, hex, 16);
        strcat(p5, "%");
        strcat(p5, hex);
        break;

      default:
        n = strlen(p5);
        p5[n] = *p;
        p5[n + 1] = '\0';
      }
      p++;
    }

    p = (char *)malloc(strlen(searchEngines->path) + 1);
    strcpy(p, searchEngines->path);
    p2 = strtok(p, "\t ");
    p2 = strtok(NULL, "\t\n ");
    p3 = strstr(p2, "<args>");

    p4 = (char *)malloc(strlen(p2) + strlen(p5) + 1);
    strncpy(p4, p2, (p3 - p2)*sizeof(char));
    p4[p3 - p2] = '\0';
    strcat(p4, p5);
    p3 += 6*sizeof(char);
    strcat(p4, p3);

    if(*(cs->BrowserPath))
      ShellExecute(hWnd, "open", cs->BrowserPath, p4, NULL, SW_SHOWNORMAL);
    else
      ShellExecute(hWnd, "open", p4, "", 0, SW_SHOWNORMAL);
  } else {
    if(!cs->NoWarnOnError)
      MessageBox(hWnd, "No search argument specified or search engine list empty", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
  }

  if(p5)
    free(p5);
  if(p4)
    free(p4);
  if(p)
    free(p);
}


/***********************************************************
* void AliasExecute()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *command                                       *
*      Search Engine string.                               *
*    - char *args                                          *
*      The search phrase for this engine                   *
*                         *  *  *  *                       *
* This function is called after the user has entered a     *
* command.  It searches for an alias match and executes it *
* Returns TRUE if it finds a match, returns FALSE if it    *
* doesn't.                                                 *
***********************************************************/

BOOL AliasExecute(char *command, char *args)
{
  char *buf = NULL, *p = NULL, *p2 = NULL, *p3 = NULL;

  if(aliases) {
    if(HistoryMoveToEntry(&aliases, command, TRUE)) {
      buf = (char *)malloc(strlen(aliases->path) + 1);
      strcpy(buf, aliases->path);
      p = strtok(buf, "\t ");
      p = strtok(NULL, "\n\0");
      p3 = (char *)malloc(strlen(p) + (args ? strlen(args) : 0) + 2);
      strcpy(p3, p);
      if(args) {
        strcat(p3, " ");
        strcat(p3, args);
      }
      /*p = strtok(NULL, "\t\n ");
      p2 = strtok(NULL, "\n");
      if(p2) {
        p3 = (char *)malloc(strlen(p2) + (args ? strlen(args) : 0) + 2);
        strcpy(p3, p2);
        if(args && *args)
          strcat(p3, " ");
      } else {
        p3 = (char *)malloc((args ? strlen(args) : 0) + 1);
        strcpy(p3, "");
      }
      if(args) {
        strcat(p3, args);
      }

      ShellExecute(hWnd, "open", p, p3, 0, SW_SHOWNORMAL);*/
      ExecCommand(p3, TRUE);
    }
  }

  if(buf) {
    free(buf);
    if(p3)
      free(p3);
    return TRUE;
  } else
    return FALSE;
}


/***********************************************************
* void ParseCalculatorCommand()                            *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *command                                       *
*      Search Engine string.                               *
*                         *  *  *  *                       *
* This function is called when the user wants to calculate *
* a value using the calculator.                            *
***********************************************************/

void ParseCalculatorCommand(char *command)
{
  int error = 0;
  char *p = NULL, val[1024];

  ++command;
  if(cs->CommaDelimiter) {
  p = command;
  while(*p)
    if(*p == ',')
      *p = '.';
  }

  Evaluate(command, &error, val, sizeof(val));

  if(!error || error == ERROR_CONVERSIONS_DONE) {
    if(cs->CommaDelimiter) {
      p = val;
      while(*p)
        if(*p == '.')
          *p = ',';
    }
    SetWindowText(hText, val);
    HistoryRemoveAll(&files, &nFiles);
    SendMessage(hText, EM_SETSEL, strlen(val), strlen(val));
  } else {
    SetWindowText(hText, "error");
    HistoryRemoveAll(&files, &nFiles);
    SendMessage(hText, EM_SETSEL, 0, -1);
  }

  if(!visible) {
    ShowWindow(hWnd, SW_SHOWNORMAL);
    SetFocus(hText);
    visible = TRUE;
  }
}


/***********************************************************
* BOOL AutoComplete()                                      *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hText                                          *
*      Handle to the textbox that will get the resultant   *
*      AutoCompleted string.                               *
*    - char *szPath                                        *
*      Path to autocomplete.                               *
*                         *  *  *  *                       *
* AutoCompletes the path pointed to by szPath.  If the     *
* autocomplete succeeds, we add that string to the textbox *
* Returns TRUE upon success, FALSE upon never finding a    *
* match.                                                   *
***********************************************************/

BOOL AutoComplete(HWND hText, char *szPath)
{
  char *szText = NULL, *p;
  int len = strlen(szPath);

  if(len > 0) {
    if(len == 1)
      HistoryMoveToTail(&current);

    while(current->prev) {
      if(!_strnicmp(current->prev->path, szPath, len)) {
        szText = malloc(strlen(current->prev->path) + 1);
        p = current->prev->path + len*sizeof(char);
        strcpy(szText, szPath);
        strcat(szText, p);
        SetWindowText(hText, szText);
        HistoryRemoveAll(&files, &nFiles);
        SendMessage(hText, EM_SETSEL, len, -1);
        HistoryMovePrev(&current, 1); // This just keeps everything flowing smooth if the user
        free(szText);                 // hits up/down next - see WM_CHAR below
        return TRUE;
      }
      HistoryMovePrev(&current, 1);
    }
  }

  return FALSE;
}


/***********************************************************
* void HistoryUsePrev()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hText                                          *
*      Handle of the textbox that receives the previous    *
*      history entry.                                      *
*                         *  *  *  *                       *
* This function moves the list pointer back one and places *
* the text in the textbox.                                 *
***********************************************************/

void HistoryUsePrev(HWND hText)
{
	char *szBuf = "";
  HistoryMovePrev(&current, 1);
	SetWindowText(hText,current->path);
  HistoryRemoveAll(&files, &nFiles);
	return;
}


/***********************************************************
* void HistoryUseNext()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hText                                          *
*      Handle of the textbox that receives the next        *
*      history entry.                                      *
*                         *  *  *  *                       *
* This function moves the list pointer forward one and     *
* places the text in the textbox.                          *
***********************************************************/

void HistoryUseNext(HWND hText)
{
	char *szBuf = "";
  HistoryMoveNext(&current, 1);
	SetWindowText(hText,current->path);
  HistoryRemoveAll(&files, &nFiles);
	return;
}


/***********************************************************
* void ExecCommand()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *command                                       *
*      Pointer to the command to execute                   *
*    - BOOL alias                                          *
*      I use this to make sure that commands sent by an    *
*      alias isn't also added to the history.              *
*                         *  *  *  *                       *
* This function checks for a valid command and sends it to *
* the proper place - ParseBang, ParseSearch, or ShellEx    *
***********************************************************/

void ExecCommand(char *command, BOOL alias)
{
  int index;
	char *newcmd, *args, *p;
  HINSTANCE val=0;

	if(strlen(command)<1)
		return;

  if(!alias) {
    index = HistoryFindIndexOf(&current, command);

    if(index <= 0) {
      if(!(nHistoryEntries < cs->MaxHistoryEntries)) {
        HistoryMoveToHead(&current);
        HistoryRemoveEntry(&current, &nHistoryEntries);
      }
      HistoryAdd(&current, command, &nHistoryEntries);
      MenuAddItem(hHistoryMenu, command, HMI_USER_HISTORY, cs->NewestHistoryItemsOnTop);
    } else {
      // Bring this command to the front of the history.
      if(cs->NewestHistoryItemsOnTop)
        index = GetMenuItemCount(hHistoryMenu) - index;
      else
        --index;
      MenuDeleteItem(hHistoryMenu, index);
      HistoryRemoveEntry(&current, &nHistoryEntries);
      HistoryAdd(&current, command, &nHistoryEntries);
      MenuAddItem(hHistoryMenu, command, HMI_USER_HISTORY, cs->NewestHistoryItemsOnTop);
    }
  }

  if(*command == '\"') {
    newcmd = command + sizeof(char);
    newcmd = strtok(newcmd, "\"");
    args = strtok(NULL, "\0");
    
    if(!newcmd) {
      if(!cs->NoWarnOnError)
        MessageBox(hWnd, "Missing closing quote.", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
      return;
    }
  } else {
    newcmd = strtok(command, "\t ");
    if(!newcmd) {
      newcmd = command;
      args = NULL;
    } else {
      args = strtok(NULL, "\0");
    }
  }

  p = newcmd + (strlen(newcmd) - 1)*sizeof(char);
  while(*newcmd == ' ' || *newcmd == '\t')
    ++newcmd;
  while(*p == ' ' || *p == '\t') {
    *p = '\0';
    --p;
  }

  if(*newcmd != '?' && *newcmd != '!') {
    if(args) {
      p = args + (strlen(args) - 1)*sizeof(char);
      while(*args == ' ' || *args == '\t' || *args == '\"')
        ++args;
      while(*p == ' ' || *p == '\t' || *p == '\"') {
        *p = '\0';
        --p;
      }
    }
  } else
    if(args) {
      p = args + (strlen(args) - 1)*sizeof(char);
      while(*args == ' ' || *args == '\t')
        ++args;
      while(*p == ' ' || *p == '\t') {
        *p = '\0';
        --p;
      }
    }
	
  if(!AliasExecute(newcmd, args)) {
    if(*newcmd == '!') {
		  ParseBangCommand(hWnd, newcmd, args);
    } else if(*newcmd == '=') {
      if(args)
        newcmd[strlen(newcmd)] = ' ';
      ParseCalculatorCommand(newcmd);
    } else if(*newcmd == '?') {
      if(searchEngines) {
        ParseSearchCommand(newcmd, args);
      } else {
        MessageBeep(MB_ICONHAND);
        MessageBox(hWnd, "You have not specified any search engines, or specified\nan invalid search engine list.  You can not use the search feature at this time.", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
      }
    } else {
      if(cs->ExplorePaths && PathIsDirectory(newcmd))
        val = ShellExecute(hWnd, "explore", newcmd, args, NULL, SW_SHOWNORMAL);
      else
        val = ShellExecute(hWnd, "open", newcmd, args, NULL, SW_SHOWNORMAL);

      if(((long)val == ERROR_PATH_NOT_FOUND || (long)val == ERROR_FILE_NOT_FOUND) && !PathIsDirectory(newcmd)) {
        if(!strstr(command, "\\")) {
          if(cs->AssumeNetAddress) {
            if(args)
              newcmd[strlen(newcmd)] = ' ';
            newcmd = (char *)malloc(strlen(command) + 8);
            strcpy(newcmd, "http://");
            strcat(newcmd, command);
            ShellExecute(hWnd, "open", newcmd, "", "", SW_SHOWNORMAL);
            free(newcmd);
          } else {
            if(!cs->NoWarnOnError) {
              MessageBeep(MB_ICONHAND);
              MessageBox(NULL, "The file or path could not be found.", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            }
          }
        } else {
          if(args) {
            newcmd[strlen(newcmd)] = ' ';
            if(cs->ExplorePaths && PathIsDirectory(newcmd))
              val = ShellExecute(hWnd, "explore", newcmd, NULL, NULL, SW_SHOWNORMAL);
            else
              val = ShellExecute(hWnd, "open", newcmd, NULL, NULL, SW_SHOWNORMAL);
 
            if(((long)val == ERROR_PATH_NOT_FOUND || (long)val == ERROR_FILE_NOT_FOUND) && !PathIsDirectory(newcmd) && !cs->NoWarnOnError) {
              MessageBeep(MB_ICONHAND);
              MessageBox(NULL, "The file or path could not be found.", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            }
          } else {
            if(!cs->NoWarnOnError) {
              MessageBeep(MB_ICONHAND);
              MessageBox(NULL, "The file or path could not be found.", "LSXCommand", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            }
          }
        }
      }
    }
  }
}


/***********************************************************
* void ParseContextMenuCommand()                           *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - long retval                                         *
*      ID of the user's selection                          *
*    - char *args                                          *
*      The search phrase for this engine                   *
*                         *  *  *  *                       *
* This function is called after the user has entered a     *
* command.  It searches for an alias match and executes it *
* Returns TRUE if it finds a match, returns FALSE if it    *
* doesn't.                                                 *
***********************************************************/

void ParseContextMenuCommand(long retval, char *buf)
{
  char *buf2, temp[_MAX_PATH];
  HGLOBAL hglob;
  LPTSTR str;
  DWORD nStart, nEnd;

  item.cbSize = sizeof(MENUITEMINFO);
  item.fMask = MIIM_TYPE;
  item.dwTypeData = buf;
  item.cch = _MAX_PATH;

  if(retval == HMI_USER_PASTE) {
    if(!OpenClipboard(hWnd))
      return;
    hglob = (void *)GetClipboardData(CF_TEXT);
    if(hglob) {
      buf2 = GlobalLock(hglob);
      if(buf2) {
        SendMessage(hText, EM_REPLACESEL, TRUE, (LPARAM)buf2);
        GlobalUnlock(hglob);
      }
    }
    CloseClipboard();
  } else if(retval == HMI_USER_COPY || retval == HMI_USER_CUT) {
    if(!OpenClipboard(hWnd))
      return;
    GetWindowText(hText, temp, sizeof(temp));
    SendMessage(hText, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
    buf2 = temp + nStart*sizeof(char);
    temp[nEnd] = '\0';
    str = GlobalAlloc(GMEM_FIXED, (strlen(buf2) + 1)*sizeof(char));
    memcpy(str, buf2, (strlen(buf2) + 1)*sizeof(char));
    hglob = GlobalHandle(str);
    SetClipboardData(CF_TEXT, hglob);
    CloseClipboard();
    GlobalFree(hglob);
    if(retval == HMI_USER_CUT)
      SendMessage(hText, EM_REPLACESEL, TRUE, (LPARAM)"");
  } else if(retval >= HMI_USER_HISTORY) {
    GetMenuItemInfo(hHistoryMenu, retval, FALSE, &item);
    if(cs->ContextMenuExecute) {
      ExecCommand(buf, FALSE);
    } else {
      SetWindowText(hText, buf);
      SendMessage(hText, EM_SETSEL, 0, -1);
    }
  } else if(retval >= HMI_USER_ALIAS) {
    GetMenuItemInfo(hAliasMenu, retval, FALSE, &item);
    if(cs->ContextMenuExecute)
      ExecCommand(buf, FALSE);
    else {
      SetWindowText(hText, buf);
      SendMessage(hText, EM_SETSEL, 0, -1);
    }
  } else if(retval >= HMI_USER_SE) {
    GetMenuItemInfo(hSearchEngineMenu, retval, FALSE, &item);
    buf2 = (char *)malloc(strlen(buf) + 3);
    strcpy(buf2, "?");
    strcat(buf2, buf);
    strcat(buf2, " ");
    SetWindowText(hText, buf2);
    SendMessage(hText, EM_SETSEL, strlen(buf2), strlen(buf2));
    free(buf2);
  }

  if(cs->ContextMenuExecute && retval >= HMI_USER_ALIAS && retval < HMI_USER_CUT && buf[0] != '=') {
    if(!cs->NoClearOnCommand || (cs->HideOnCommand && cs->ClearOnHide))
      SetWindowText(hText, "");
    else
      SetWindowText(hText, buf);

    if(cs->HideOnCommand) {
      ShowWindow(hWnd, SW_HIDE);
      visible = FALSE;
    }
  } else {
	  if(!visible) {
		  ShowWindow(hWnd,SW_SHOWNORMAL);
		  visible = TRUE;
	  }
	  SetForegroundWindow(hText);
	  SetFocus(hText);
  }

  HistoryRemoveAll(&files, &nFiles);
}
#endif //LSXCOMMANDCLOCK_EXPORTS

/***********************************************************
* BOOL CALLBACK BGProc()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hText                                          *
*      A handle to a window which this function handles    *
*    - UINT msg                                            *
*      The message sent to the textbox                     *
*    - WPARAM wParam                                       *
*      The WPARAM of this message                          *
*    - LPARAM lParam                                       *
*      The LPARAM of this message                          *
*                         *  *  *  *                       *
* Handles the window procedure for the background window   * 
***********************************************************/

/*
BOOL CALLBACK BGProc(HWND hText,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg) {
	case WM_NCHITTEST:
		return HTNOWHERE;
	case WM_DROPFILES:
		{
		char szFileName[MAX_PATH];
		DragQueryFile((HDROP)wParam, 0, szFileName, sizeof(szFileName));
		SetWindowText(hText, szFileName);
		SetFocus(hText);
		DragFinish((HDROP)wParam);
		return 0;
		}
	}
	return CallWindowProc(wpOld,hText,msg,wParam,lParam);
}
*/


/***********************************************************
* BOOL CALLBACK EditProc()                                 *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hText                                          *
*      A handle to a window which this function handles    *
*    - UINT msg                                            *
*      The message sent to the textbox                     *
*    - WPARAM wParam                                       *
*      The WPARAM of this message                          *
*    - LPARAM lParam                                       *
*      The LPARAM of this message                          *
*                         *  *  *  *                       *
* Handles the window procedure for the command textbox.    * 
***********************************************************/

BOOL CALLBACK EditProc(HWND hText,UINT msg,WPARAM wParam,LPARAM lParam)
{
#ifndef LSXCOMMANDCLOCK_EXPORTS
	char buf[_MAX_PATH], *buf2, *buf3;
#endif //LSXCOMMANDCLOCK_EXPORTS
	switch(msg) {
#ifndef LSXCOMMANDCLOCK_EXPORTS
	case WM_KEYDOWN:
		if(wParam==VK_DOWN) {
      if(cs->UnixHistory)
        HistoryUseNext(hText);
      else
			  HistoryUsePrev(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
    } else if(wParam==VK_UP) {
      if(cs->UnixHistory)
        HistoryUsePrev(hText);
      else
			  HistoryUseNext(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
    } else if(wParam == VK_TAB) {
      // If we want file autocompletes, populate the files list and set text to the first one.
      // Consecutive tabs will select the next file in the list...
      // Move cursor to start of next word and select rest of the line.
      if(cs->TabFileComplete) {
        HANDLE handle;
        LPWIN32_FIND_DATA found = (LPWIN32_FIND_DATA)malloc(sizeof(found));
        BOOL bContinue;
			  char path_buffer[_MAX_PATH];
			  char drive[_MAX_DRIVE];
			  char dir[_MAX_DIR];

        if(nFiles == 0) {
		      GetWindowText(hText, buf, sizeof(buf));
		      strcat(buf, "*");
		      handle = FindFirstFile(buf, found);
          bContinue = (handle != INVALID_HANDLE_VALUE);

          while(bContinue) {
            if(*(found->cFileName) && strcmp(found->cFileName, ".") && strcmp(found->cFileName, "..")) {
              HistoryAdd(&files, found->cFileName, &nFiles);
            }
            bContinue = FindNextFile(handle, found);
          }

          if(nFiles != 0) {
            HistoryMoveToHead(&files);
            FindClose(handle);

			      _splitpath(buf, drive, dir, NULL, NULL);
			      _makepath(path_buffer, drive, dir, files->path, NULL);
			      SetWindowText(hText, path_buffer);
            SendMessage(hText, WM_KEYDOWN, VK_END, 0);
          }
        } else {
          GetWindowText(hText, buf, sizeof(buf));
          HistoryMoveNext(&files, 1);
			    _splitpath(buf, drive, dir, NULL, NULL);
			    _makepath(path_buffer, drive, dir, files->path, NULL);
			    SetWindowText(hText, path_buffer);
          SendMessage(hText, WM_KEYDOWN, VK_END, 0);
        }
      }
      if(!cs->NoTabMicroComplete) {
        DWORD nStart, nEnd;
        SendMessage(hText, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
        GetWindowText(hText, buf, sizeof(buf));
        buf2 = buf + nStart*sizeof(char);
        while(buf2 && *buf2 != ' ' && *buf2) {
          ++buf2;
          ++nStart;
        }
        if(*buf2) {
          ++nStart;
          SendMessage(hText, EM_SETSEL, (WPARAM)nStart, (LPARAM)strlen(buf));
        } else
          SendMessage(hText, EM_SETSEL, (WPARAM)nStart, (LPARAM)nStart);
      }
      return 0;
    } else if((wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_HOME || wParam == VK_END || VK_DELETE) && cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
        SetFocus(hText);
    }
		break;
  case WM_KEYUP:
    if((wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_HOME || wParam == VK_END || VK_DELETE) && cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
        SetFocus(hText);
    }
    break;
	case WM_CHAR:
    if(wParam != VK_TAB)
      HistoryRemoveAll(&files, &nFiles);  //If not TAB, we don't want any file autocompletes..
		if(wParam==VK_RETURN) {
      BOOL cleared = FALSE;
			GetWindowText(hText,buf,sizeof(buf));
      if(!cs->NoClearOnCommand) {
        SetWindowText(hText,"");
        cleared = TRUE;
      }
			if(cs->HideOnCommand && buf[0] != '=') {
				ShowWindow(hWnd,SW_HIDE);
				visible = FALSE;
        if(cs->ClearOnHide) {
          SetWindowText(hText,"");
          cleared = TRUE;
        }
			}
			ExecCommand(buf, FALSE);
      HistoryMoveToTail(&current);
      if(!cleared)
        HistoryMovePrev(&current, 1);
			return 0;
    } else if(wParam == VK_TAB)
      return 0;
    else {
      /*long retval;
      SIZE sz;
      RECT rct;*/
      if(cs->CommaDelimiter && (wParam == 0x2E)) { // Period
        GetWindowText(hText, buf, sizeof(buf));
        if(buf[0] == '=')
          wParam = 0x2C; // Comma
      }
      if(cs->AutoComplete) {
        DWORD nStart, nEnd;
        SendMessage(hText, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
        GetWindowText(hText, buf, sizeof(buf));
        buf2 = (char *)malloc(strlen(buf) + 2);
        buf3 = buf + nEnd*sizeof(char);
        strcpy(buf2, "");
        strncpy(buf2, buf, nStart);
        buf2[nStart] = (char)wParam;
        buf2[nStart + 1] = '\0';
        strcat(buf2, buf3);
        HistoryMoveNext(&current, 1);
        nStart = AutoComplete(hText, buf2);
        /*if((char)wParam == '?' && cs->ContextMenuAutoPopup) {
          GetTextExtentPoint32(GetDC(hText), "?", 1, &sz);
          GetWindowRect(hWnd, &rct);
          rct.left += (cs->BorderSize + sz.cx);
          retval = TrackPopupMenu(hSearchEngineMenu, TPM_RIGHTALIGN | (cs->ContextMenuAboveBox ? TPM_BOTTOMALIGN : TPM_TOPALIGN) | TPM_NONOTIFY | TPM_RIGHTBUTTON, rct.left, (cs->ContextMenuAboveBox ? rct.top : rct.bottom), 0, hWnd, NULL);
        }*/ // AUTOPOPUP STUFF - UNDOCUMENTED, CAN'T PULL OFF WITH REGULAR MENUS
        if(nStart) {
          free(buf2);
          return 0;
        } else HistoryMoveToTail(&current);  // We got to something that is not in the history.
      }
      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
        SetFocus(hText);
      }
    }
		break;
	case WM_DROPFILES:
		{
		  char szFileName[MAX_PATH];
		  DragQueryFile((HDROP)wParam, 0, szFileName, sizeof(szFileName));
		  SetWindowText(hText, szFileName);
		  SetFocus(hText);
		  DragFinish((HDROP)wParam);
      HistoryRemoveAll(&files, &nFiles);
      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
        SetFocus(hText);
      }
		  return 0;
		}
    break;
  case WM_LBUTTONDOWN:
    if(((*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears && FindWindow("WINAMP V1.X", NULL))) && GetFocus() != hText) {
      SetWindowText(hText, "");
      SetForegroundWindow(hWnd);
      SetFocus(hText);
      HistoryRemoveAll(&files, &nFiles);
    }
    if(cs->SelectAllOnMouseFocus && GetFocus() != hText) {
      SetForegroundWindow(hWnd);
      SetFocus(hText);
      SendMessage(hText, EM_SETSEL, 0, -1);
      return 0;
    }
    if(cs->Transparent) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOW);
      SetFocus(hText);
    }
    break;
#endif //LSXCOMMANDCLOCK_EXPORTS
  case WM_KILLFOCUS:
    if(visible && cs->HideOnUnfocus) {
	    ShowWindow(hWnd, SW_HIDE);
#ifndef LSXCOMMANDCLOCK_EXPORTS
      if(cs->ClearOnHide) {
        SetWindowText(hText,"");
        HistoryRemoveAll(&files, &nFiles);
      }
#endif //LSXCOMMANDCLOCK_EXPORTS
	    visible = FALSE;
    }
    break;
#ifndef LSXCOMMANDCLOCK_EXPORTS
  case WM_CONTEXTMENU:
    {
      long retval = TrackPopupMenu(hContextMenu, TPM_LEFTALIGN | (cs->ContextMenuAboveBox ? TPM_BOTTOMALIGN : TPM_TOPALIGN) | TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
      ParseContextMenuCommand(retval, buf);
      return 0;
    }
#endif //LSXCOMMANDCLOCK_EXPORTS
	}
	return CallWindowProc(wpOld,hText,msg,wParam,lParam);
}


/***********************************************************
* BOOL CALLBACK WndProc()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hWnd                                           *
*      A handle to a textbox which this function handles   *
*    - UINT msg                                            *
*      The message sent to the textbox                     *
*    - WPARAM wParam                                       *
*      The WPARAM of this message                          *
*    - LPARAM lParam                                       *
*      The LPARAM of this message                          *
*                         *  *  *  *                       *
* Handles the window procedure for the main window         * 
***********************************************************/

BOOL CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg) {
	case WM_CREATE:
		{
		hFont = CreateFont(cs->TextSize,0,0,0,0,0,0,0,0,0,0,0,0,cs->TextFontFace);
		hText = CreateWindowEx(0,"EDIT","",WS_CHILD|ES_AUTOHSCROLL,cs->BorderSize,cs->BorderSize,cs->width-(cs->BorderSize*2),cs->height-(cs->BorderSize*2),hWnd,0,hInst,0);
    hBGBrush = CreateSolidBrush(cs->BGColor);
    hHollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    hBGBitmap = *(cs->Background) ? LoadLSImage(cs->Background, NULL) : NULL;
		if(!hText) {
			MessageBox(hWnd,"Error creating window",szApp,MB_OK);
		}
		wpOld = (WNDPROC)SetWindowLong(hText,GWL_WNDPROC,(long)EditProc);
		//DragAcceptFiles(hBG, TRUE);
		SendMessage(hText,WM_SETFONT,(WPARAM)hFont,FALSE);
		//ShowWindow(hBG,SW_SHOW);
		ShowWindow(hText,SW_SHOW);
		return 0;
		}
	case WM_SETCURSOR:
		if(cs->NoCursorChange) return TRUE;
		else break;
	case WM_NCHITTEST:
		if(cs->notmoveable) return HTNOWHERE;
		else return HTCAPTION;
	////// THERES SOMETHING CAUSEING SOME KIND OF MEM LEAK HERE
	////// IF YOU KNOW WHAT IT IS, LET ME KNOW
	case WM_CTLCOLOREDIT:
    {
      /*DWORD align = 0;
      char *p, Align[15];
      strcpy(Align, cs->TextAlign);
      p = strtok(Align, "\t ");
      if(p) {
        if(!stricmp(p, "CENTER"))
          align |= TA_CENTER;
        else if(!stricmp(p, "RIGHT"))
          align |= TA_RIGHT;
        else
          align |= TA_LEFT;
        p = strtok(NULL, "\t ");
        if(p) {
          if(!stricmp(p, "BOTTOM"))
            align |= TA_BOTTOM;
          else
            align |= TA_TOP;
          if(align) {
            align |= TA_UPDATECP;
            SetTextAlign((HDC)wParam, (UINT)align);
          }
        }
      }*/
      if(cs->Transparent) {
        SetBkMode((HDC)wParam, TRANSPARENT);
        SetTextColor((HDC)wParam, cs->TextColor);
        return (int)hHollowBrush;
      } else {
        SetBkColor((HDC)wParam, cs->BGColor);
        SetTextColor((HDC)wParam, cs->TextColor);
        return (int)hBGBrush;
      }
    }
    break;
  case WM_PAINT:
    if(hBGBitmap) {
      PAINTSTRUCT ps;
      HDC src = CreateCompatibleDC(NULL);
      HDC hdc = BeginPaint(hWnd, &ps);
      SelectObject(src, hBGBitmap);
      if(!BitBlt(hdc, 0, 0, cs->width, cs->height, src, 0, 0, SRCCOPY))
        break;
      //TransparentBltLS((HDC)wParam, 0, 0, cs->width, cs->height, src, 0, 0, RGB(255,0,255));
      EndPaint(hWnd, &ps);
      DeleteDC(src);
      return 0;
    }
    break;
#ifndef LSXCOMMANDCLOCK_EXPORTS
	case WM_DROPFILES:
		{
		char szFileName[MAX_PATH];
		DragQueryFile((HDROP)wParam, 0, szFileName, sizeof(szFileName));
		SetWindowText(hText, szFileName);
		SetFocus(hText);
		DragFinish((HDROP)wParam);
		return 0;
		}
#endif //LSXCOMMANDCLOCK_EXPORTS
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}


/***********************************************************
* VOID CALLBACK TimerProc()                                *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND hWnd                                           *
*      The HWND of the window that owns this timer.        *
*    - UINT uMsg                                           *
*      The WM_TIMER message.                               *
*    - UINT idEvent                                        *
*      ID of the Timer event.                              *
*    - DWORD dwTime                                        *
*      Current system time.                                *
*                         *  *  *  *                       *
* If the user chooses to have a clock in LSXCommand, this  *
* Callback processes the request to update the time & date *
***********************************************************/

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
  char title[_MAX_PATH];

#ifndef LSXCOMMANDCLOCK_EXPORTS
  if(idEvent == ID_CLOCKTIMER && GetFocus() != hText && visible) {
    /* 7 lines of code stolen from LSTime */
    time_t tVal;
    struct tm *stVal;
    char tstring[512];

    time(&tVal);
    stVal = localtime(&tVal);
    strftime(tstring, sizeof(tstring), cs->Clock, stVal);

    SetWindowText(hText, tstring);
    HistoryRemoveAll(&files, &nFiles);
    if(cs->Transparent) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOWNOACTIVATE);
    }
  } else if(idEvent == ID_WINAMPTIMER && GetFocus() != hText && visible) {
    HWND wahWnd = FindWindow("WINAMP V1.X", NULL);
    if(wahWnd) {
      if(nTimers == 2) {
        KillTimer(hWnd, ID_CLOCKTIMER);
        --nTimers;
      }
      GetWindowText(wahWnd, title, sizeof(title));
      SetWindowText(hText, title);
      HistoryRemoveAll(&files, &nFiles);
      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOWNOACTIVATE);
      }
    } else if(*(cs->Clock) && nTimers < 2) {
      ++nTimers;
      SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
    }
  }
#else //LSXCOMMANDCLOCK_EXPORTS
  if(idEvent == ID_CLOCKTIMER && visible) {
    /* 7 lines of code stolen from LSTime */
    time_t tVal;
    struct tm *stVal;
    char tstring[512];

    time(&tVal);
    stVal = localtime(&tVal);
    strftime(tstring, sizeof(tstring), cs->Clock, stVal);

    SetWindowText(hText, tstring);
    if(cs->Transparent) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOWNOACTIVATE);
    }
  } else if(idEvent == ID_WINAMPTIMER && visible) {
    HWND wahWnd = FindWindow("WINAMP V1.X", NULL);
    if(wahWnd) {
      if(nTimers == 2) {
        KillTimer(hWnd, ID_CLOCKTIMER);
        --nTimers;
      }
      GetWindowText(wahWnd, title, sizeof(title));
      SetWindowText(hText, title);
      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOWNOACTIVATE);
      }
    } else if(*(cs->Clock) && nTimers < 2) {
      ++nTimers;
      SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
    }
  }
#endif //LSXCOMMANDCLOCK_EXPORTS
}


/***********************************************************
* void BangFocusCommand()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to focus the command box       *
***********************************************************/

void BangFocusCommand(HWND caller,char *args)
{
	if(!visible) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
	}
	SetForegroundWindow(hText);
	SetFocus(hText);
#ifndef LSXCOMMANDCLOCK_EXPORTS
  if((*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears)) {
    SetWindowText(hText, "");
    HistoryRemoveAll(&files, &nFiles);
  }
#endif //LSXCOMMANDCLOCK_EXPORTS
  if(cs->SelectAllOnFocus) SendMessage(hText, EM_SETSEL, 0, -1);
#ifndef LSXCOMMANDCLOCK_EXPORTS
  if(cs->Transparent && ((*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears))) {
    ShowWindow(hWnd, SW_HIDE);
    ShowWindow(hWnd, SW_SHOW);
    SetFocus(hText);
  }
#endif
}


/***********************************************************
* void BangToggleCommand()                                 *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to toggle the visibility of    *
* the command box.                                         *
***********************************************************/

void BangToggleCommand(HWND caller, char *args)
{
	if(visible) {
		ShowWindow(hWnd, SW_HIDE);
#ifndef LSXCOMMANDCLOCK_EXPORTS
    if(cs->ClearOnHide) {
      SetWindowText(hText,"");
      HistoryRemoveAll(&files, &nFiles);
    }
#endif //LSXCOMMANDCLOCK_EXPORTS
		visible = FALSE;
	}
	else {
		ShowWindow(hWnd, SW_SHOWNORMAL);
		SetForegroundWindow(hText);
		SetFocus(hText);
#ifndef LSXCOMMANDCLOCK_EXPORTS
    if((*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears)) {
      SetWindowText(hText, "");
      HistoryRemoveAll(&files, &nFiles);
    }
#endif //LSXCOMMANDCLOCK_EXPORTS
    if(cs->SelectAllOnFocus) SendMessage(hText, EM_SETSEL, 0, -1);
#ifndef LSXCOMMANDCLOCK_EXPORTS
    if((cs->Transparent && (*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears))) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOW);
      SetFocus(hText);
    }
#endif
		visible = TRUE;
	}
	return;
}


/***********************************************************
* void BangRescanEngineList()                              *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

#ifndef LSXCOMMANDCLOCK_EXPORTS
void BangRescanEngineList(HWND caller, char *args)
{
  HistoryRemoveAll(&searchEngines, &nSearchEngines);
  DestroyMenu(hSearchEngineMenu);
  DestroyMenu(hHistoryMenu);
  DestroyMenu(hAliasMenu);
  SearchEngineInit(cs);
  CreateAliasPopupMenu();
  CreateHistoryPopupMenu();
  ContextMenuInit();
}


/***********************************************************
* void BangClearHistory()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

void BangClearHistory(HWND caller, char *args)
{
  ClearHistory(&current);
}


/***********************************************************
* void BangShowContextMenu()                               *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

void BangShowContextMenu(HWND caller, char *args)
{
  char buf[_MAX_PATH];
  long retval;
  POINT pt;

  GetCursorPos(&pt);
  retval = TrackPopupMenu(hContextMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
  ParseContextMenuCommand(retval, buf);
}


/***********************************************************
* void BangCommand()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Executes a command.  This enables any part of Litestep   *
* to use the power of LSXCommand's command engine.         *
***********************************************************/

void BangCommand(HWND caller, char *args)
{
  ExecCommand(args, FALSE);
}
#endif //LSXCOMMANDCLOCK_EXPORTS

/***********************************************************
* void BangShowCommand()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Just shows the command box if it isn't visible.          *
***********************************************************/

void BangShowCommand(HWND caller, char *args)
{
  if(!visible) {
		ShowWindow(hWnd, SW_SHOWNORMAL);
		visible = TRUE;
  }
}


/***********************************************************
* void BangHideCommand()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Just hides the command box if it is visible.             *
***********************************************************/

void BangHideCommand(HWND caller, char *args)
{
  if(visible) {
		ShowWindow(hWnd, SW_HIDE);
#ifndef LSXCOMMANDCLOCK_EXPORTS
    if(cs->ClearOnHide) {
      SetWindowText(hText,"");
      HistoryRemoveAll(&files, &nFiles);
    }
#endif //LSXCOMMANDCLOCK_EXPORTS
		visible = FALSE;
  }
}


#ifndef LSXCOMMANDCLOCK_EXPORTS
/***********************************************************
* void BangBrowseFile()                                    *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Opens up an "Open File" dialog box so the user doesn't   *
* have to type...                                          *
***********************************************************/

void BangBrowseFile(HWND caller, char *args)
{
  char file[_MAX_PATH], file_total[_MAX_PATH], title[] = "LSXCommand Open File", *filter = NULL, *p, *p2;
  OPENFILENAME ofn;

  if(args && *args) {
    filter = (char *)malloc(strlen(args) + 2);
    strcpy(filter, args);
    filter[strlen(filter) + 1] = '\0';
    p2 = filter;
    p = strchr(p2, '|');
    while(p) {
      *p = '\0';
      p2 += strlen(p2) + 1;
      p = strchr(p2, '|');
    }
  }

  strcpy(file, "");
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = filter;
  ofn.lpstrCustomFilter = NULL;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = file;
  ofn.nMaxFile = _MAX_PATH - 1;
  ofn.lpstrTitle = title;
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

  if(GetOpenFileName(&ofn)) {
    p = ofn.lpstrFile;
    strcpy(file_total, p);
    p2 = file_total + strlen(file_total);

    p += strlen(p) + 1;
    while(*p) {
      strcpy(p2, p);
      ExecCommand(file_total, FALSE);
      p += strlen(p) + 1;
    }
  }

  if(filter)
    free(filter);
}


/***********************************************************
* void BangBrowseFolder()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - char *args                                          *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* This lets the user select a folder to browse.            *
***********************************************************/

void BangBrowseFolder(HWND caller, char *args)
{
  BROWSEINFO bi;
  LPITEMIDLIST pidl;
  char title[] = "LSXCommand Browse", path[_MAX_PATH];

  bi.hwndOwner = hWnd;
  bi.pidlRoot = NULL;
  bi.lpszTitle = title;
  bi.ulFlags = BIF_VALIDATE;

  pidl = SHBrowseForFolder(&bi);

  if(pidl && SHGetPathFromIDList(pidl, path)) {
    ExecCommand(path, FALSE);
  }
}
#endif //LSXCOMMANDCLOCK_EXPORTS

/***********************************************************
* int initModule()                                         *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND parent                                         *
*      The HWND of the parent window to this module        *
*    - HINSTANCE dll                                       *
*      The HINSTANCE of this DLL.                          *
*    - WharfDataType *wd                                   *
*      A pointer to the structure containing data about LS *
*                         *  *  *  *                       *
* Handles all initialization of LSXCommand.  Returns 0     *
***********************************************************/

int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd)
{
	WNDCLASS wc;
	HBRUSH hBr;

	hInst = dll;
  cs = ReadSettings(wd);
	hBr = CreateSolidBrush(cs->BorderColor);
#ifndef LSXCOMMANDCLOCK_EXPORTS
  SearchEngineInit(cs);
  AliasInit();
  HistoryInit();
  ContextMenuInit();
  
  if(cs->ClearHistoryOnStartup)
    ClearHistory(&current);
#endif //LSXCOMMANDCLOCK_EXPORTS

  memset(&wc,0,sizeof(wc));
	wc.hInstance = hInst;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = szApp;
	wc.hbrBackground = hBr;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	RegisterClass(&wc);
  hWnd = CreateWindowEx(((cs->NoAlwaysOnTop)?0:WS_EX_TOPMOST)|WS_EX_TOOLWINDOW|((cs->Transparent==2) ? WS_EX_TRANSPARENT:0),szApp,"",WS_POPUP|((cs->BevelBorder)?WS_SIZEBOX:0),cs->x,cs->y,cs->width,cs->height,GetDesktopWindow(),0,hInst,0);
	
  //DragAcceptFiles(hWnd, TRUE);
	SetWindowLong(hWnd,GWL_USERDATA,magicDWord);
#ifndef LSXCOMMANDCLOCK_EXPORTS
	AddBangCommand("!TOGGLECOMMAND",BangToggleCommand);
	AddBangCommand("!FOCUSCOMMAND",BangFocusCommand);
  AddBangCommand("!COMMANDRESCANENGINES", BangRescanEngineList);
  AddBangCommand("!COMMANDCLEARHISTORY", BangClearHistory);
  AddBangCommand("!COMMANDSHOWCONTEXTMENU", BangShowContextMenu);
  AddBangCommand("!COMMAND", BangCommand);
  AddBangCommand("!COMMANDSHOW", BangShowCommand);
  AddBangCommand("!COMMANDHIDE", BangHideCommand);
  AddBangCommand("!COMMANDBROWSEFILE", BangBrowseFile);
  AddBangCommand("!COMMANDBROWSEFOLDER", BangBrowseFolder);
#else //LSXCOMMANDCLOCK_EXPORTS
	AddBangCommand("!TOGGLECOMMANDCLOCK",BangToggleCommand);
	AddBangCommand("!FOCUSCOMMANDCLOCK",BangFocusCommand);
  AddBangCommand("!COMMANDCLOCKSHOW", BangShowCommand);
  AddBangCommand("!COMMANDCLOCKHIDE", BangHideCommand);
#endif
	
  if(!cs->HiddenOnStart) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
  } else {
    ShowWindow(hWnd, SW_HIDE);
    visible = FALSE;
  }

  if(*(cs->Clock)) {
    SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
    ++nTimers;
  }

  if(cs->ScrollWinAmp) {
    SetTimer(hWnd, ID_WINAMPTIMER, 100, TimerProc);
    ++nTimers;
  }

	return 0;
}


/***********************************************************
* int quitModule()                                         *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HINSTANCE dll                                       *
*      The HINSTANCE of this DLL.                          *
*                         *  *  *  *                       *
* Handles all of the unloading of this DLL.                *
***********************************************************/

int quitModule(HINSTANCE dll)
{
#ifndef LSXCOMMANDCLOCK_EXPORTS
	WriteHistory();
  HistoryRemoveAll(&current, &nHistoryEntries);
  HistoryRemoveAll(&searchEngines, &nSearchEngines);
  HistoryRemoveAll(&aliases, &nAliases);
  free(defaultEngine);
#endif //LSXCOMMANDCLOCK_EXPORTS
  free(szModuleIniPath);
  free(cs);
#ifndef LSXCOMMANDCLOCK_EXPORTS
  DestroyMenu(hHistoryMenu);
  DestroyMenu(hSearchEngineMenu);
  DestroyMenu(hAliasMenu);
  DestroyMenu(hContextMenu);
#endif LSXCOMMANDCLOCK_EXPORTS
	DestroyWindow(hWnd);
	UnregisterClass(szApp,hInst);
	DeleteObject(hFont);
	return 0;
}