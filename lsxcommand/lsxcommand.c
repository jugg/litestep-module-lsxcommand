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
*                lsxcommand.c - Main lsxcommand source file                   *
*                                   *  *  *  *                                *
*           Last Update:  October 31, 2003  10:20 A M                         *
******************************************************************************/

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "resource.h"
#include "exports.h"
#include "lsxcommand.h"

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

//20021115
int msgs[] = {LM_GETREVID, 0};
const char szAppName[] = "lsxcommand.dll"; // Our window class, etc
const char rcsRevision[] = "1.8.5"; // Our Version

//20021114
#define MAX_LINE_LENGTH 4096

#ifndef LSXCOMMANDCLOCK_EXPORTS

BOOL editfirst = FALSE, staticfirst = FALSE;
char *szApp = "LSXCommand", *defaultEngine = NULL;
int nHistoryEntries = 0, nSearchEngines = 0, nAliases = 0, nFiles = 0;
HMENU hSearchEngineMenu = NULL, hAliasMenu = NULL, hHistoryMenu = NULL, hContextMenu = NULL;
MENUITEMINFO item;
struct History *current = NULL, *searchEngines = NULL, *aliases = NULL, *files = NULL;

extern double Evaluate(char *expr, int *error, char *replacement, size_t replace_size, struct CommandSettings *cs);

#else

char *szApp = "LSXCommandClock";

#endif //LSXCOMMANDCLOCK_EXPORTS

BOOL visible = FALSE;
BOOL lsboxed = FALSE;
BOOL waitforlsBox = FALSE;
char szModuleIniPath[MAX_PATH];
enum Timers curTimer = NO_TIMER;
HINSTANCE hInst = NULL;
HWND hWnd = NULL, hText = NULL, hBG = NULL;
HFONT hFont = NULL;
HBRUSH hBGBrush = NULL, hHollowBrush = NULL;
HBITMAP hBGBitmap = NULL;
WNDPROC wpOld, wpBG;
struct CommandSettings *cs;


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

struct CommandSettings *ReadSettings(LPCSTR lsPath)
{
  int offsetx, offsety;
  RECT screen;
  // 20021114
  char szTemp[256];

  struct CommandSettings *settings = (struct CommandSettings *)malloc(sizeof(struct CommandSettings));
  
  //20021114
  settings->OnFocusCommand = NULL;
  settings->OnUnfocusCommand = NULL;

#ifndef LSXCOMMANDCLOCK_EXPORTS
  if (!GetRCString("CommandHistoryFile", szModuleIniPath, NULL, MAX_PATH))
  {
      strcpy(szModuleIniPath, lsPath);
      strcat(szModuleIniPath, "\\MODULES.INI");
  }
  
    GetRCString("CommandTextFontFace",settings->TextFontFace,"Arial",256);
	settings->BGColor = GetRCColor("CommandBGColor",RGB(255,255,255));
	settings->TextColor = GetRCColor("CommandTextColor",RGB(0,0,0));
	settings->TextSize = GetRCInt("CommandTextSize",14);
	settings->m_bTextBold = settings->m_bTextUnderline = settings->m_bTextItalic = FALSE;
	settings->m_bTextBold = GetRCBool("CommandTextBold",TRUE);
	settings->m_bTextUnderline = GetRCBool("CommandTextUnderline",TRUE);
	settings->m_bTextItalic = GetRCBool("CommandTextItalic",TRUE);

	settings->origx = settings->x = GetRCInt("CommandX",0);
	settings->origy = settings->y = GetRCInt("CommandY",0);
	settings->width = GetRCInt("CommandWidth",160);
	settings->height = GetRCInt("CommandHeight",20);
  settings->MaxHistoryEntries = GetRCInt("CommandHistoryEntries", 10) + 1;
  settings->MaxHistoryMenuEntries = GetRCInt("CommandHistoryMenuEntries", settings->MaxHistoryEntries - 1 );
	settings->BorderSize = GetRCInt("CommandBorderSize", -1);
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
	settings->WaitForBox = GetRCBool("CommandWaitForBox",TRUE);
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
  settings->AddExternalsToHistory = GetRCBool("CommandAddExternalsToHistory", TRUE);
  GetRCString("CommandSearchEngineList", settings->SearchEngineList, "engines.list", sizeof(settings->SearchEngineList));
  GetRCString("CommandContextMenuOrder", settings->ContextMenuOrder, "012", sizeof(settings->ContextMenuOrder));
  GetRCString("CommandBackground", settings->Background, "", sizeof(settings->Background));
  settings->m_bStretchImage = GetRCBoolDef("CommandStretchBackground", FALSE);
  GetRCString("CommandTime", settings->Clock, "", sizeof(settings->Clock));
  GetRCString("CommandSearchEngineBrowser", settings->BrowserPath, "", sizeof(settings->BrowserPath));
  GetRCString("CommandMusicPlayer", settings->MusicPlayer, "WINAMP V1.X", sizeof(settings->MusicPlayer));
  //GetRCString("CommandTextAlign", settings->TextAlign, "Left Top", sizeof(settings->TextAlign));
#else //LSXCOMMANDCLOCK_EXPORTS
  if (!GetRCString("CommandClockHistoryFile", szModuleIniPath, NULL, MAX_PATH))
  {
      strcpy(szModuleIniPath, lsPath);
      strcat(szModuleIniPath, "\\MODULES.INI");
  }
  GetRCString("CommandClockTextFontFace",settings->TextFontFace,"Arial",256);
	settings->BGColor = GetRCColor("CommandClockBGColor",RGB(255,255,255));
	settings->TextColor = GetRCColor("CommandClockTextColor",RGB(0,0,0));
	settings->TextSize = GetRCInt("CommandClockTextSize",14);
	settings->origx = settings->x = GetRCInt("CommandClockX",0);
	settings->origy = settings->y = GetRCInt("CommandClockY",0);
	settings->width = GetRCInt("CommandClockWidth",160);
	settings->height = GetRCInt("CommandClockHeight",20);
	settings->BorderSize = GetRCInt("CommandClockBorderSize", -1);
  offsetx = GetRCInt("CommandClockOffsetX", 0);
  offsety = GetRCInt("CommandClockOffsetY", 0);
	settings->BorderColor = GetRCColor("CommandClockBorderColor",RGB(102,102,102));
	settings->BevelBorder = GetRCBool("CommandClockBevelBorder",TRUE);
	settings->NoAlwaysOnTop = GetRCBool("CommandClockNotAlwaysOnTop",TRUE);
	settings->notmoveable = GetRCBool("CommandClockNotMoveable",TRUE);
	settings->HiddenOnStart = GetRCBool("CommandClockHiddenOnStart",TRUE);
	settings->WaitForBox = GetRCBool("CommandClockWaitForBox",TRUE);
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

  /* If the universal border is invalid, use separate borders */
	if(settings->BorderSize == -1) {
#ifndef LSXCOMMANDCLOCK_EXPORTS
		settings->BottomBorderSize = GetRCInt("CommandBottomBorderSize",2);
		settings->TopBorderSize = GetRCInt("CommandTopBorderSize",2);
		settings->LeftBorderSize = GetRCInt("CommandLeftBorderSize",2);
		settings->RightBorderSize = GetRCInt("CommandRightBorderSize",2);
#else // LSXCOMMANDCLOCK_EXPORTS
		settings->BottomBorderSize = GetRCInt("CommandClockBottomBorderSize",2);
		settings->TopBorderSize = GetRCInt("CommandClockTopBorderSize",2);
		settings->LeftBorderSize = GetRCInt("CommandClockLeftBorderSize",2);
		settings->RightBorderSize = GetRCInt("CommandClockRightBorderSize",2);
#endif // LSXCOMMANDCLOCK_EXPORTS
	} else {
		settings->BottomBorderSize = settings->BorderSize;
		settings->TopBorderSize = settings->BorderSize;
		settings->LeftBorderSize = settings->BorderSize;
		settings->RightBorderSize = settings->BorderSize;
	}

  /* No Background */
  if(!(*(settings->Background)))
    settings->Transparent = 0;

  screen.left = 0;
  screen.right = GetSystemMetrics(SM_CXSCREEN);
  screen.top = 0;
  screen.bottom = GetSystemMetrics(SM_CYSCREEN);

  /* Offsets */
  if(offsetx == 0) {
    if(settings->x < 0)
      settings->origx = settings->x = screen.right + settings->x;
  } else if(offsetx == 1)
    settings->origx = settings->x = screen.right / 2 - settings->width / 2 + settings->x;
  else
    settings->origx = settings->x = screen.right + settings->x;

  if(offsety == 0) {
    if(settings->y < 0)
      settings->origy = settings->y = screen.bottom + settings->y;
  } else if(offsety == 1)
    settings->origy = settings->y = screen.bottom / 2 - settings->height / 2 + settings->y;
  else
    settings->origy = settings->y = screen.bottom + settings->y;

	//20021114
	GetRCLine("CommandOnFocus", szTemp, MAX_LINE_LENGTH, "!NONE");
	settings->OnFocusCommand = (char*) malloc((strlen(szTemp)+1)*sizeof(char));
	strcpy(settings->OnFocusCommand, szTemp);
  
	GetRCLine("CommandOnUnfocus", szTemp, MAX_LINE_LENGTH, "!NONE");
	settings->OnUnfocusCommand = (char*) malloc((strlen(szTemp)+1)*sizeof(char));
	strcpy(settings->OnUnfocusCommand, szTemp);

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

  if( current ){
	  while(current->next) {   // We stop the last entry since it is NULL - this is intentional
		_itoa(i, name, 10);
		WritePrivateProfileString("LSXCOMMAND", name, current->path, szModuleIniPath);

		HistoryMoveNext(&current, 1);
		++i;
	  }
  }

  current = temp;
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
  char listline[1024], *p, *p2;
  FILE *fpEngineList;
  /*int nummenus = 0;*/
  HMENU tempmenu = NULL;
  struct  MenuHist {
    HMENU *hm;
    int curitem;
    struct MenuHist *prev;
  } *mhist = NULL, *tmphist = NULL;

  fpEngineList = fopen(cs->SearchEngineList, "r");
  if(!fpEngineList)
    return;

  hSearchEngineMenu = CreatePopupMenu();
  item.cbSize = sizeof(MENUITEMINFO);
  item.fType = MFT_STRING;
  item.wID = HMI_USER_SE;

  mhist = (struct MenuHist *)malloc(sizeof(struct MenuHist));
  mhist->hm = &hSearchEngineMenu;
  mhist->curitem = 0;
  mhist->prev = NULL;

  strcpy(listline, "");

  for(;;) {
    if(!fgets(listline, 511, fpEngineList))
      break;

    p = strchr(listline, '\n');
    if(p)
      *p = '\0';

    p = strchr(listline, ';');
    if(p == listline)
      continue;

    if(p)
      *p = '\0';

    p = listline;
    while(isspace(*p))
      ++p;

    // strtok for Default.  Save the other token to defaultEngine
    if(!(_strnicmp("default", listline, strlen("default")*sizeof(char)))) {
      p = strtok(listline, "\t ");
      p = strtok(NULL, "\n");
      defaultEngine = (char *)malloc(strlen(p) + 1);
      strcpy(defaultEngine, p);
      continue;
    }

    if(*p) {
      p2 = strchr(p, '{');
      if(p2) {
        p2 = p;
        if(*p2 == '\"')
          p2 = strtok(p, "\"");
        else
          p2 = strtok(p, " \t");
        if(p2 && *p2) {
          tmphist = (struct MenuHist *)malloc(sizeof(struct MenuHist));
          tmphist->hm = (HMENU *)malloc(sizeof(HMENU));
          *(tmphist->hm) = CreatePopupMenu();
          tmphist->curitem = 0;
          tmphist->prev = mhist;
          item.dwTypeData = p2;
          item.cch = sizeof(p2);
          item.fMask = MIIM_SUBMENU | MIIM_TYPE;
          item.hSubMenu = *(tmphist->hm);
          InsertMenuItem(*(mhist->hm), mhist->curitem, TRUE, &item);
          mhist = tmphist;
        }
      } else {
        p2 = strchr(p, '}');
        if(p2) {
          tmphist = mhist;
          mhist = mhist->prev;
          free(tmphist->hm);
          free(tmphist);
          ++(mhist->curitem);
        } else {
          p2 = p;
          if(*p2) {
            HistoryAdd(&searchEngines, p2, &nSearchEngines);
            p2 = strtok(p, "\t ");
            if(p2 && *p2) {
              item.fMask = MIIM_TYPE | MIIM_ID;
              item.hSubMenu = NULL;
              item.dwTypeData = p2;
              item.cch = sizeof(p2);
              InsertMenuItem(/*hSearchEngineMenu*/*(mhist->hm), /*item.wID - HMI_USER_SE*/mhist->curitem, TRUE, &item);
              ++item.wID;
              ++(mhist->curitem);
            }
          }
        }
      }
    }
  }

  free(mhist);
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

// Parses szString, splitting it up into tokens.  Pays attention to single and
// double quotes.
//
//	original src - D:\VC\LiteStep\ls-b24\lsapi\lsapi.c
//	tanuki modify version
//

int cz_LCTokenize (LPCSTR szString, LPSTR *lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	int		index = 0;
	char	quoteChar = 0;

	char	buffer[MAX_LINE_LENGTH];
	char	output[MAX_LINE_LENGTH];
	char	*pOutput = NULL;
	DWORD	dwBufferCount = 0;

	strcpy (buffer, szString);

	pOutput = output;

	while (buffer[index] && dwBufferCount < dwNumBuffers)
	{
		BOOL skipWhitespace = FALSE;

		switch (buffer[index])
		{
		case '"':
		case '\'':
			{
//	tanuki modify start
				*pOutput++ = buffer[index];
				*pOutput = '\0';
				if (!quoteChar)
				{
					quoteChar = buffer[index];
				}
				else
				{
					if (quoteChar == buffer[index])
					{
						quoteChar = 0;
						strcpy (*lpszBuffers, output);
						lpszBuffers++;
						dwBufferCount++;

						if (dwBufferCount < dwNumBuffers)
						{
							(*lpszBuffers)[0] = '\0';
						}
						pOutput = output;
						*pOutput = '\0';
						skipWhitespace = TRUE;
					}
				}
				break;
//	tanuki modify end
			}
		case ' ':
		case '\t':
			{
				if (!quoteChar)
				{
					if (strlen (output))
					{
						strcpy (*lpszBuffers, output);
						lpszBuffers++;
						dwBufferCount++;
						if (dwBufferCount < dwNumBuffers)
						{
							(*lpszBuffers)[0] = '\0';
						}
						pOutput = output;
						*pOutput = '\0';
						skipWhitespace = TRUE;
					}
				}
				else
				{
					*pOutput++ = buffer[index];
					*pOutput = '\0';
				}
				break;
			}
		default:
			{
				*pOutput++ = buffer[index];
				*pOutput = '\0';
				break;
			}
		}

		index++;

		if (skipWhitespace)
		{
			while (isspace (buffer[index]))
			{
				index++;
			}
		}
	}

	if (strlen (output))
	{
		if (dwBufferCount < dwNumBuffers)
		{
			dwBufferCount++;

			strcat (*lpszBuffers, output);
		}
	}

	if (szExtraParameters && dwBufferCount == dwNumBuffers)
	{
		strcpy (szExtraParameters, buffer + index);
	}

	return dwBufferCount;
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

  while(LCReadNextConfig(f, "*CommandAlias", buffer, sizeof(buffer))) {
    if(cz_LCTokenize(buffer, tokens, 3, extra) >= 3) {					//	tanuki modify
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
*      Alias name                                          *
*    - char *args                                          *
*      Any extra arguments to this alias' command          *
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
    while(*p) {
      if(*p == ',')
        *p = '.';
      ++p;
    }
  }

  Evaluate(command, &error, val, sizeof(val), cs);

  if(!error || error == ERROR_CONVERSIONS_DONE) {
    if(cs->CommaDelimiter) {
      p = val;
      while(*p) {
        if(*p == '.')
          *p = ',';
        ++p;
      }
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

	if( current ){
		while(current->prev && strlen(current->prev->path)) {
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
  if( current ){
	SetWindowText(hText,current->path);
  }
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
  if( current ){
	SetWindowText(hText,current->path);
  }
  HistoryRemoveAll(&files, &nFiles);
	return;
}


/***********************************************************
* void ExecCommand()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - const char *szCommand                               *
*      Pointer to the command to execute                   *
*    - BOOL noaddtohist                                    *
*      This specifies whether the command should be added  *
*      to the history.                                     *
*                         *  *  *  *                       *
* This function checks for a valid command and sends it to *
* the proper place - ParseBang, ParseSearch, or ShellEx    *
***********************************************************/

void ExecCommand(const char *szCommand, BOOL noaddtohist)
{
  int index;
	char *newcmd, *args, *p, command[4096], dir[_MAX_DIR], full_dir[_MAX_DRIVE + _MAX_DIR], explore[] = "explore", open[] = "open"; // Can't trust _MAX_PATH for command
  HINSTANCE val=0;
  SHELLEXECUTEINFO si;

	if(!szCommand || strlen(szCommand) < 1)
		return;

  VarExpansion(command, szCommand);

  if(!noaddtohist) {
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
      memset(&si, 0, sizeof(si));
      si.cbSize = sizeof(SHELLEXECUTEINFO);
      si.hwnd = hWnd;
      si.lpFile = newcmd;
      si.lpParameters = args;
      si.nShow = SW_SHOWNORMAL;
      si.fMask = SEE_MASK_DOENVSUBST | SEE_MASK_FLAG_NO_UI;
      if(cs->ExplorePaths && PathIsDirectory(newcmd)) {
        val = ShellExecute(hWnd, "explore", newcmd, args, NULL, SW_SHOWNORMAL);
      } else {
        _splitpath(newcmd, full_dir, dir, NULL, NULL);
        si.lpVerb = NULL;
        si.lpDirectory = full_dir;
        strcat(full_dir, dir);
        (HINSTANCE)ShellExecuteEx(&si);
        val = (HINSTANCE)GetLastError();
        //val = ShellExecute(hWnd, "open", newcmd, args, NULL, SW_SHOWNORMAL);
      }

      if(((long)val == ERROR_PATH_NOT_FOUND || (long)val == ERROR_FILE_NOT_FOUND) && !PathIsDirectory(newcmd)) {
        if(!strstr(command, "\\")) {
          if(cs->AssumeNetAddress) {
            if(args)
              newcmd[strlen(newcmd)] = ' ';
            newcmd = (char *)malloc(strlen(command) + 8);
            strcpy(newcmd, "http://");
            strcat(newcmd, command);
            si.lpParameters = NULL;
            si.lpDirectory = NULL;
            si.lpFile = newcmd;
            ShellExecuteEx(&si);
            //ShellExecute(hmWnd, "open", newcmd, "", "", SW_SHOWNORMAL);
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
            if(cs->ExplorePaths && PathIsDirectory(newcmd)) {
              val = ShellExecute(hWnd, "explore", newcmd, NULL, NULL, SW_SHOWNORMAL);
            } else {
              si.lpVerb = open;
              si.lpDirectory = NULL;
              ShellExecuteEx(&si);
              val = (HINSTANCE)GetLastError();
              //val = ShellExecute(hWnd, "open", newcmd, NULL, NULL, SW_SHOWNORMAL);
            }

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
*    - const char *args                                    *
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
    } else if(wParam == VK_ESCAPE) {
      SetWindowText(hText, "");
      HistoryMoveToHead(&current);
    } else if(wParam == VK_TAB) {
      // If we want file autocompletes, populate the files list and set text to the first one.
      // Consecutive tabs will select the next file in the list...
      // Move cursor to start of next word and select rest of the line.
      if(cs->TabFileComplete) {
        HANDLE handle;
        WIN32_FIND_DATA found;
        BOOL bContinue;
			  char path_buffer[_MAX_PATH];
			  char drive[_MAX_DRIVE];
			  char dir[_MAX_DIR];

        if(nFiles == 0) {
		      GetWindowText(hText, buf, sizeof(buf));
		      strcat(buf, "*");
		      handle = FindFirstFile(buf, &found);
          bContinue = (handle != INVALID_HANDLE_VALUE);

          while(bContinue) {
            if(*(found.cFileName) && strcmp(found.cFileName, ".") && strcmp(found.cFileName, "..")) {
              HistoryAdd(&files, found.cFileName, &nFiles);
            }
            bContinue = FindNextFile(handle, &found);
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
    } else if(wParam == VK_TAB || wParam == VK_ESCAPE)
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
#endif //LSXCOMMANDCLOCK_EXPORTS
  case WM_LBUTTONDOWN:
#ifndef LSXCOMMANDCLOCK_EXPORTS
    if(((*(cs->Clock) && cs->ClockDisappears) || (cs->ScrollWinAmp && cs->WinAmpDisappears && FindWindow(cs->MusicPlayer, NULL))) && GetFocus() != hText) {
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
#else //LSXCOMMANDCLOCK_EXPORTS
    {
      HWND active = GetForegroundWindow();
      if(curTimer == WINAMP_TIMER) {
        KillTimer(hWnd, ID_WINAMPTIMER);
        TimerProc(hWnd, 0, ID_CLOCKTIMER, 0);
        SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
        curTimer = CLOCK_TIMER;
      } else if(cs->ScrollWinAmp)
        SetTimer(hWnd, ID_WINAMPTIMER, 100, WinAmpTimerProc);

      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
      }
      SetForegroundWindow(active);
      return 0;
    }
#endif //LSXCOMMANDCLOCK_EXPORTS
    break;
  case WM_KILLFOCUS:
    if(visible && cs->HideOnUnfocus) {
	    ShowWindow(hWnd, SW_HIDE);
		//20021114
        if(cs->OnUnfocusCommand != NULL)
			LSExecute(NULL,cs->OnUnfocusCommand, 0);
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
  case WM_CLOSE:
    return 0;
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
		//hFont = CreateFont(cs->TextSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,cs->TextFontFace);
		hFont = CreateFont(
			cs->TextSize,				// height of font
			0,							// average character width
			0,							// angle of escapement
			0,							// base-line orientation angle
			(cs->m_bTextBold ? FW_BOLD: FW_NORMAL),	// font weight
			cs->m_bTextItalic,							// italic attribute option
			cs->m_bTextUnderline,					    // underline attribute option
			0,							// strikeout attribute option
			DEFAULT_CHARSET,			// character set identifier
			0,							// output precision
			0,							// clipping precision
			0,							// output quality
			0,							// pitch and family
			cs->TextFontFace			// typeface name
		);
		hText = CreateWindowEx(0,"EDIT","",WS_CHILD|ES_AUTOHSCROLL,cs->LeftBorderSize,cs->TopBorderSize,cs->width-(cs->LeftBorderSize+cs->RightBorderSize),cs->height-(cs->TopBorderSize+cs->BottomBorderSize),hWnd,0,hInst,0);
		hBGBrush = CreateSolidBrush(cs->BGColor);
		hHollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		hBGBitmap = *(cs->Background) ? LoadLSImage(cs->Background, NULL) : NULL;
		if( cs->m_bStretchImage )
			GetLSBitmapSize ( hBGBitmap, &cs->m_nWidth, &cs->m_nHeight );
		if(!hText) {
			MessageBox(hWnd,"Error creating window",szApp,MB_OK);
		}
		wpOld = (WNDPROC)SetWindowLong(hText,GWL_WNDPROC,(long)EditProc);
		//DragAcceptFiles(hBG, TRUE);
		SendMessage(hText,WM_SETFONT,(WPARAM)hFont,FALSE);
		//ShowWindow(hBG,SW_SHOW);
		ShowWindow(hText,SW_SHOWNOACTIVATE);
		//20021115
		SendMessage(GetLitestepWnd(), LM_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);
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
	  if( cs->m_bStretchImage ){
		  SetStretchBltMode( hdc, STRETCH_DELETESCANS ); 
		  if( !StretchBlt( hdc, 0, 0, cs->width, cs->height, src, 0, 0, cs->m_nWidth, cs->m_nHeight, SRCCOPY ) )
			break;
	  }
	  else{
		if(!BitBlt(hdc, 0, 0, cs->width, cs->height, src, 0, 0, SRCCOPY))
			break;
		//TransparentBltLS((HDC)wParam, 0, 0, cs->width, cs->height, src, 0, 0, RGB(255,0,255));
	  }
      EndPaint(hWnd, &ps);
      DeleteDC(src);
      return 0;
    }
    break;
  case WM_DESTROY:
	    //20021115
		SendMessage(GetLitestepWnd(), LM_UNREGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);
        if (lsboxed)
		{
			cs->WaitForBox = TRUE;
			visible = TRUE;
			SetWindowLong(hWnd, GWL_STYLE, (GetWindowLong(hWnd, GWL_STYLE) &~ WS_CHILD)|WS_POPUP);
			SetParent(hWnd, 0);
			SetWindowPos(hWnd,HWND_TOP,cs->x,cs->y,0,0,SWP_NOSIZE|SWP_HIDEWINDOW);
			lsboxed = FALSE;
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
	//20021115
	case LM_GETREVID:
		{
			char* buf = (char*)(lParam);
	
			switch (wParam)
			{
				case 0:
				case 1:
					sprintf(buf, "LsxCommand %s ", rcsRevision);
					break;
				default:
					strcpy(buf, "");
			}
			return strlen(buf);
		}
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
#ifndef LSXCOMMANDCLOCK_EXPORTS
  if(GetFocus() != hText && visible && !cs->WaitForBox) {
#else
  if(visible) {
#endif
    time_t tVal;
    struct tm *stVal;
    char tstring[512];

    time(&tVal);
    stVal = localtime(&tVal);
    strftime(tstring, sizeof(tstring), cs->Clock, stVal);

    SetWindowText(hText, tstring);
#ifndef LSXCOMMANDCLOCK_EXPORTS
    HistoryRemoveAll(&files, &nFiles);
#endif
    if(cs->Transparent) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOWNOACTIVATE);
    }
  }
}


/***********************************************************
* VOID CALLBACK WinAmpTimerProc()                          *
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
* If the user chooses to display the current WinAmp title, *
* this function updates the lsxcommand display.            *
***********************************************************/

VOID CALLBACK WinAmpTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
  char title[_MAX_PATH], curr[_MAX_PATH];

#ifndef LSXCOMMANDCLOCK_EXPORTS
  if(GetFocus() != hText && visible) {
#else
  if(visible) {
#endif
    HWND wahWnd = FindWindow(cs->MusicPlayer, NULL);
    if(wahWnd) {
      if(curTimer == CLOCK_TIMER) {
        KillTimer(hWnd, ID_CLOCKTIMER);
        curTimer = WINAMP_TIMER;
      }
      GetWindowText(wahWnd, title, sizeof(title));
      GetWindowText(hText, curr, sizeof(curr));
      if(strcmp(title, curr))
        SetWindowText(hText, title);
#ifndef LSXCOMMANDCLOCK_EXPORTS
      HistoryRemoveAll(&files, &nFiles);
#endif
      if(cs->Transparent) {
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOWNOACTIVATE);
      }
    } else if(*(cs->Clock) && curTimer != CLOCK_TIMER) {
      SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
      curTimer = CLOCK_TIMER;
    }
  }
}


/***********************************************************
* void BangFocusCommand()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to focus the command box       *
***********************************************************/

void BangFocusCommand(HWND caller, const char *args)
{
	if(!visible) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
		//20021114
		if(cs->OnFocusCommand)
			LSExecute(NULL,cs->OnFocusCommand, 0);
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to toggle the visibility of    *
* the command box.                                         *
***********************************************************/

void BangToggleCommand(HWND caller, const char *args)
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

#ifndef LSXCOMMANDCLOCK_EXPORTS
void BangRescanEngineList(HWND caller, const char *args)
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

void BangClearHistory(HWND caller, const char *args)
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Handles the !Bang Command to rescan the list of search   *
* engines.                                                 *
***********************************************************/

void BangShowContextMenu(HWND caller, const char *args)
{
  char buf[_MAX_PATH];
  long retval;
  POINT pt;

  GetCursorPos(&pt);
  retval = TrackPopupMenu(hContextMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
  ParseContextMenuCommand(retval, buf);
}


/***********************************************************
* void BangExecCommand()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Executes a command.  This enables any part of Litestep   *
* to use the power of LSXCommand's command engine.         *
* Changed from BangCommand(), since that's now a datatype  *
* inside lsapi.h in newer builds.
***********************************************************/

void BangExecCommand(HWND caller, const char *args)
{
  ExecCommand(args, !(cs->AddExternalsToHistory));
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Just shows the command box if it isn't visible.          *
***********************************************************/

void BangShowCommand(HWND caller, const char *args)
{
  if(!visible) {
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Just hides the command box if it is visible.             *
***********************************************************/

void BangHideCommand(HWND caller, const char *args)
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* Opens up an "Open File" dialog box so the user doesn't   *
* have to type...                                          *
***********************************************************/

void BangBrowseFile(HWND caller, const char *args)
{
  char file[_MAX_PATH], file_total[_MAX_PATH], filetitle[_MAX_PATH], title[] = "LSXCommand Open File", *filter = NULL, *p = NULL, *p2 = NULL;
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
  } else {
    filter = (char *)malloc(strlen("All Files (*.*)\0*.*") + 1);
    strcpy(filter, "All Files (*.*)\0*.*");
  }

  strcpy(file, "*.*\0");
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = filter;
  ofn.lpstrCustomFilter = (LPSTR) NULL;
  ofn.nMaxCustFilter = 0L;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = file;
  ofn.nMaxFile = sizeof(file);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = NULL;
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = "";
  ofn.lpstrTitle = title;
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER; /*OFN_EXPLORER; */

  if(GetOpenFileName(&ofn)) {
    p = ofn.lpstrFile;
    strcpy(file_total, p);
    if(file_total[strlen(file_total) - 1] == '\\') {
      p2 = file_total + strlen(file_total);

      p += strlen(p) + 1;
      while(*p) {
        strcpy(p2, p);
        ExecCommand(file_total, TRUE);
        p += strlen(p) + 1;
      }
    } else
      ExecCommand(file_total, TRUE);
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
*    - const char *args                                    *
*      Should be the arguments to the function...          *
*                         *  *  *  *                       *
* This lets the user select a folder to browse.            *
***********************************************************/

void BangBrowseFolder(HWND caller, const char *args)
{
  char title[] = "LSXCommand Browse", path[_MAX_PATH];
  BROWSEINFO bi = {hWnd, NULL, path, "LSXCommand Browse", BIF_RETURNONLYFSDIRS, NULL, 0, 0};
  LPITEMIDLIST pidl;
  IMalloc *pm;

  bi.hwndOwner = hWnd;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = path;
  bi.lpszTitle = title;
  bi.ulFlags = BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;

  pidl = SHBrowseForFolder(&bi);

  if(pidl && SHGetPathFromIDList(pidl, path)) {
    ExecCommand(path, TRUE);
    SHGetMalloc(&pm);
    if (pm) {
      pm->lpVtbl->Free(pm,pidl);
      pm->lpVtbl->Release(pm);
    }
  }
}
#endif //LSXCOMMANDCLOCK_EXPORTS


/***********************************************************
* void BangMove()										                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Space delimited pair of numbers telling how far to  *
*      move left/right, up/down (depending on sign)        *
*                         *  *  *  *                       *
* Moves the main LSXCommand according to the input         *
***********************************************************/

void BangMove(HWND caller, const char *args)
{
  int cx, cy, oldx = cs->x, oldy = cs->y;
  char *str, *xstr, *ystr;

  if(args && *args) {
    if(!_stricmp(args, "home")) {
      cs->x = cs->origx;
      cs->y = cs->origy;
    } else {
      str = (char *)malloc(strlen(args) + 1);
      strcpy(str, args);

      xstr = strtok(str, " \t");
      ystr = strtok(NULL, "");

      if(xstr) {
        if(*xstr == '(' && xstr[strlen(xstr) - 1] == ')') {
          xstr[strlen(xstr) - 1] = '\0';
          cs->x = atoi(xstr + 1);
        } else
          cs->x += atoi(xstr);

        cx = GetSystemMetrics(SM_CXSCREEN);

        if(cs->x < 0)
          cs->x = 0;
        else if(cs->x > (cx - cs->width))
          cs->x = cx - cs->width;
      }

      if(ystr) {
        if(*ystr == '(' && ystr[strlen(ystr) - 1] == ')') {
          ystr[strlen(ystr) - 1] = '\0';
          cs->y = atoi(ystr + 1);
        } else
          cs->y += atoi(ystr);

        cy = GetSystemMetrics(SM_CYSCREEN);

        if(cs->y < 0)
          cs->y = 0;
        else if(cs->y > (cy - cs->height))
          cs->y = cy - cs->height;
      }

      free(str);
    }
  } else {
    POINT cursorPos;

    GetCursorPos(&cursorPos);

    cs->x = cursorPos.x;
    cs->y = cursorPos.y;
  }

  if(oldx != cs->x || oldy != cs->y) {
    //SetWindowPos(hWnd, cs->NoAlwaysOnTop ? HWND_NOTOPMOST : HWND_TOPMOST, cs->x, cs->y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
    MoveWindow(hWnd, cs->x, cs->y, cs->width, cs->height, TRUE);
    visible = TRUE;
  }

  if(!args || !(*args))
    BangFocusCommand(caller, NULL);
}


/***********************************************************
* void BangToggleTimer()                                   *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Specifies which time to switch to                   *
*                         *  *  *  *                       *
* Toggles which timer is currently active.                 *
***********************************************************/

void BangToggleTimer(HWND caller, const char *args) {

  HWND active = GetForegroundWindow();

  if(curTimer == WINAMP_TIMER) {
    KillTimer(hWnd, ID_WINAMPTIMER);
    TimerProc(hWnd, 0, ID_CLOCKTIMER, 0);
    SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
    curTimer = CLOCK_TIMER;
  } else if(cs->ScrollWinAmp) {
      SetTimer(hWnd, ID_WINAMPTIMER, 100, WinAmpTimerProc);
  }

  if(cs->Transparent) {
    ShowWindow(hWnd, SW_HIDE);
    ShowWindow(hWnd, SW_SHOW);
  }
  SetForegroundWindow(active);
}

#ifndef LSXCOMMANDCLOCK_EXPORTS
/***********************************************************
* void BangSetText()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Specifies the text to place in LSXCommand.          *
*                         *  *  *  *                       *
* Replaces the text inside LSXCommand and brings focus to  *
* the end of the line.                                     *
***********************************************************/

void BangSetText(HWND caller, const char *args)
{
  if(args) {
    SetWindowText(hText, args);
    SetFocus(hText);
    SendMessage(hText, EM_SETSEL, strlen(args), strlen(args));

    if(cs->Transparent) {
      ShowWindow(hWnd, SW_HIDE);
      ShowWindow(hWnd, SW_SHOW);
      SetFocus(hText);
    }
  }
}
#endif LSXCOMMANDCLOCK_EXPORTS

/***********************************************************
* void BangBoxHook()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND caller                                         *
*      The HWND of the window that called this !Bang       *
*      Command.                                            *
*    - const char *args                                    *
*      Options passed by LsBox to LsxCommand               *
*                         *  *  *  *                       *
***********************************************************/
void BangBoxHook(HWND caller, const char *args)
{
  char *handle = strrchr(args,' ');

  if (handle) 
  {
	  HWND boxwnd = (HWND)atoi(handle+1);
	  if (boxwnd) 
	  {
		  lsboxed = TRUE;

		  if (boxwnd != GetParent(hWnd))
		  {
			  SetWindowLong(hWnd, GWL_STYLE, (GetWindowLong(hWnd, GWL_STYLE) &~ WS_POPUP)|WS_CHILD);
			  SetParent(hWnd, boxwnd);
			  // THis would cause problems in Win2k
			  // SetWindowLong(hWnd, GWL_STYLE, (GetWindowLong(hWnd, GWL_STYLE) &~ WS_CHILD)|WS_POPUP);
			  if (cs->WaitForBox)
				  cs->WaitForBox = FALSE;
			  //20021114
			  if(cs->HiddenOnStart)
				  ShowWindow(hWnd, SW_HIDE);
			  else
				  ShowWindow(hWnd, SW_SHOWNOACTIVATE);
		  }
	  }
  }
	return;
}




/***********************************************************
* int initModuleEx()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - HWND parent                                         *
*      The HWND of the parent window to this module        *
*    - HINSTANCE dll                                       *
*      The HINSTANCE of this DLL.                          *
*    - LPCSTR szPath                                       *
*      A pointer to the Litestep path                      *
*                         *  *  *  *                       *
* Handles all initialization of LSXCommand.  Returns 0     *
***********************************************************/

int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath)
{
	WNDCLASS wc;
	HBRUSH hBr;

  setlocale(LC_ALL, "");

	hInst = dll;
  cs = ReadSettings(szPath);
  if( !cs ){
	  return 1;
  }
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
  AddBangCommand("!COMMAND", BangExecCommand);
  AddBangCommand("!COMMANDSHOW", BangShowCommand);
  AddBangCommand("!COMMANDHIDE", BangHideCommand);
  AddBangCommand("!COMMANDBROWSEFILE", BangBrowseFile);
  AddBangCommand("!COMMANDBROWSEFOLDER", BangBrowseFolder);
  AddBangCommand("!COMMANDMOVE", BangMove);
  AddBangCommand("!COMMANDTOGGLETIMER", BangToggleTimer);
  AddBangCommand("!COMMANDSETTEXT", BangSetText);
  AddBangCommand("!COMMANDBOXHOOK", BangBoxHook);
#else //LSXCOMMANDCLOCK_EXPORTS
  AddBangCommand("!TOGGLECOMMANDCLOCK",BangToggleCommand);
  AddBangCommand("!FOCUSCOMMANDCLOCK",BangFocusCommand);
  AddBangCommand("!COMMANDCLOCKSHOW", BangShowCommand);
  AddBangCommand("!COMMANDCLOCKHIDE", BangHideCommand);
  AddBangCommand("!COMMANDCLOCKMOVE", BangMove);
  AddBangCommand("!COMMANDCLOCKTOGGLETIMER", BangToggleTimer);
  AddBangCommand("!COMMANDCLOCKBOXHOOK", BangBoxHook);
#endif
	
  if(!cs->HiddenOnStart) {
	  if(cs->WaitForBox)
		  ShowWindow(hWnd,SW_HIDE);
	  else
		  ShowWindow(hWnd,SW_SHOWNOACTIVATE);
		visible = TRUE;
  } else {
    ShowWindow(hWnd, SW_HIDE);
    visible = FALSE;
  }

  if(*(cs->Clock)) {
    SetTimer(hWnd, ID_CLOCKTIMER, 1000, TimerProc);
    curTimer = CLOCK_TIMER;
  }

  if(cs->ScrollWinAmp) {
    SetTimer(hWnd, ID_WINAMPTIMER, 100, WinAmpTimerProc);
  }

  //Initialize input to nothing
  SetWindowText(hText, "");

	return 0;
}


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
  return initModuleEx(parent, dll, wd->lsPath);
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
  RemoveBangCommand("!TOGGLECOMMAND");
  RemoveBangCommand("!FOCUSCOMMAND");
  RemoveBangCommand("!COMMANDRESCANENGINES");
  RemoveBangCommand("!COMMANDCLEARHISTORY");
  RemoveBangCommand("!COMMANDSHOWCONTEXTMENU");
  RemoveBangCommand("!COMMAND");
  RemoveBangCommand("!COMMANDSHOW");
  RemoveBangCommand("!COMMANDHIDE");
  RemoveBangCommand("!COMMANDBROWSEFILE");
  RemoveBangCommand("!COMMANDBROWSEFOLDER");
  RemoveBangCommand("!COMMANDMOVE");
  RemoveBangCommand("!COMMANDTOGGLETIMER");
  RemoveBangCommand("!COMMANDSETTEXT");
  RemoveBangCommand("!COMMANDBOXHOOK");
#else //LSXCOMMANDCLOCK_EXPORTS
  RemoveBangCommand("!TOGGLECOMMANDCLOCK");
  RemoveBangCommand("!FOCUSCOMMANDCLOCK");
  RemoveBangCommand("!COMMANDCLOCKSHOW");
  RemoveBangCommand("!COMMANDCLOCKHIDE");
  RemoveBangCommand("!COMMANDCLOCKMOVE");
  RemoveBangCommand("!COMMANDCLOCKTOGGLETIMER");
  RemoveBangCommand("!COMMANDCLOCKBOXHOOK");
#endif

#ifndef LSXCOMMANDCLOCK_EXPORTS
	WriteHistory();
  HistoryRemoveAll(&current, &nHistoryEntries);
  HistoryRemoveAll(&searchEngines, &nSearchEngines);
  HistoryRemoveAll(&aliases, &nAliases);
  free(defaultEngine);
  DestroyMenu(hHistoryMenu);
  DestroyMenu(hSearchEngineMenu);
  DestroyMenu(hAliasMenu);
  DestroyMenu(hContextMenu);
#endif //LSXCOMMANDCLOCK_EXPORTS
	DestroyWindow(hWnd);
	UnregisterClass(szApp,hInst);
	DeleteObject(hFont);
    DeleteObject(hBGBrush);
    DeleteObject(hHollowBrush);
    DeleteObject(hBGBitmap);
	//20021114
	if (cs)
	{
		if (cs->OnFocusCommand != NULL) {
			//delete [] cs->OnFocusCommand;
			free(cs->OnFocusCommand);
			cs->OnFocusCommand = NULL;
		}
		if (cs->OnUnfocusCommand != NULL) {
			//delete [] cs->OnUnfocusCommand;
			free(cs->OnUnfocusCommand);
			cs->OnUnfocusCommand = NULL;
		}
		free(cs);

	}

	return 0;
}