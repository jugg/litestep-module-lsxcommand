/***********************************************************
*        LSXCommand - Extended LSCommand Module            *
* This module adds to the functionality of limpid's        *
* LSCommand module.  Among the extentions are a resizable  *
* history, AutoCompletion, Internet Searching, and Aliases *
*                         *  *  *  *                       *
* Coding:                                                  *
*   LSXCommand - Visigoth (Shaheen Gandhi in real life)    *
*                e-mail: sgandhi@andrew.cmu.edu            *
*   Original LSCommand - limpid                            *
*                         *  *  *  *                       *
* Last Update:  May 25, 1999  8:00 PM                      *
*                         *  *  *  *                       *
* Copyright (c) 1999 Shaheen Gandhi                        *
***********************************************************/

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include "lsxcommand.h"
#include "lsapi.h"

BOOL visible = FALSE, editfirst = FALSE, staticfirst = FALSE;
char *szApp = "LSXCommand", *szModuleIniPath, *defaultEngine = NULL;
int nHistoryEntries = 0, nSearchEngines = 0, nAliases = 0;
HINSTANCE hInst;
HWND hWnd, hText, hBG;
HFONT hFont;
HBRUSH hBGBrush;
WNDPROC wpOld, wpBG;
struct CommandSettings *cs = NULL;
struct History *current = NULL, *searchEngines = NULL, *aliases = NULL;


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
	struct CommandSettings *settings = (struct CommandSettings *)malloc(sizeof(struct CommandSettings));
  szModuleIniPath = (char *)malloc(strlen(wd->lsPath) + strlen("\\MODULES.INI") + 1);
  strcpy(szModuleIniPath, wd->lsPath);
  strcat(szModuleIniPath, "\\MODULES.INI");
	GetRCString("CommandTextFontFace",settings->TextFontFace,"Arial",256);
	settings->BGColor = GetRCColor("CommandBGColor",RGB(255,255,255));
	settings->TextColor = GetRCColor("CommandTextColor",RGB(0,0,0));
	settings->TextSize = GetRCInt("CommandTextSize",14);
	settings->x = GetRCInt("CommandX",0);
	settings->y = GetRCInt("CommandY",0);
	settings->width = GetRCInt("CommandWidth",160);
	settings->height = GetRCInt("CommandHeight",20);
  settings->MaxHistoryEntries = GetRCInt("CommandHistoryEntries", 10) + 1;
	settings->BorderSize = GetRCInt("CommandBorderSize",2);
	settings->BorderColor = GetRCColor("CommandBorderColor",RGB(102,102,102));
	settings->BevelBorder = GetRCBool("CommandBevelBorder",TRUE);
	if(settings->BevelBorder) settings->BorderSize = 0; // if bevel then no border
	settings->NoAlwaysOnTop = GetRCBool("CommandNotAlwaysOnTop",TRUE);
	settings->notmoveable = GetRCBool("CommandNotMoveable",TRUE);
	settings->NoClearOnCommand = GetRCBool("CommandNoClearOnCommand",TRUE);
	settings->HideOnCommand = GetRCBool("CommandHideOnCommand",TRUE);
	settings->HiddenOnStart = GetRCBool("CommandHiddenOnStart",TRUE);
	settings->ClearOnHide = GetRCBool("CommandClearOnHide",TRUE);
	settings->NoCursorChange = GetRCBool("CommandNoCursorChange",TRUE);
  settings->SelectAllOnFocus = GetRCBool("CommandSelectAllOnFocus", TRUE);
  GetRCString("CommandSearchEngineList", settings->SearchEngineList, "engines.list", sizeof(settings->SearchEngineList));
	return settings;
}


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

  if(*hist) {
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
*                         *  *  *  *                       *
* Searches the list for a particular entry and stops when  *
* it gets to the first one with a match. The difference    *
* from this function and FindIndexOf() is it searches the  *
* first n characters of each History entry, where n is the *
* length of pszValue.  Returns TRUE upon finding a match,  *
* FALSE if it gets to the end of the list or upon error.   *
***********************************************************/

BOOL HistoryMoveToEntry(struct History **hist, char *pszValue)
{
  if(*hist) {
    HistoryMoveToHead(hist);

    while((*hist)->next) {
      if(!_strnicmp((*hist)->path, pszValue, strlen(pszValue)*sizeof(char)))
        return TRUE;
      HistoryMoveNext(hist, 1);
    }

    if(!_strnicmp((*hist)->path, pszValue, strlen(pszValue)*sizeof(char)))
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
  char *szData = (char *)malloc(strlen(pszValue) + 1);
  struct History *temp = NULL;

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


/***********************************************************
* void HistoryInit()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - struct History **hist                               *
*      Pointer to a pointer to an element of the list of   *
*      history or search engine paths.                     *
*                         *  *  *  *                       *
* Loads past history entries from the modules.ini file.    *
***********************************************************/

void HistoryInit()
{
  int i;
  char path[_MAX_PATH], name[17];

  for(i=0; i < cs->MaxHistoryEntries; ++i) {
    _itoa(i, name, 10);
    GetPrivateProfileString("LSXCOMMAND", name, "", path, sizeof(path), szModuleIniPath);

    if(*path)
      HistoryAdd(&current, path, &nHistoryEntries);
  }

  HistoryAdd(&current, "", &nHistoryEntries);
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
    if(len == 1) {
      HistoryMoveToTail(&current);
    }

    while(current->prev) {
      if(!_strnicmp(current->prev->path, szPath, len)) {
        szText = malloc(strlen(current->prev->path) + 1);
        p = current->prev->path + len*sizeof(char);
        strcpy(szText, szPath);
        strcat(szText, p);
        SetWindowText(hText, szText);
        SendMessage(hText, EM_SETSEL, len, -1);
        HistoryMovePrev(&current, 1); // This just keeps everything flowing smooth if the user
        break;                        // hits up/down next - see WM_CHAR below
      }
      HistoryMovePrev(&current, 1);
    }
  }

  if(szText) {
    free(szText);
    return TRUE;
  } else return FALSE;
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
* Initializes the list of Search Engines.  If no search    *
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
      p = strtok(NULL, "\t\n ");
      defaultEngine = (char *)malloc(strlen(p) + 1);
      strcpy(defaultEngine, p);
      continue;
    }

    if(*(listline))
      HistoryAdd(&searchEngines, listline, &nSearchEngines);
  }
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
  char *p, *p2, *p3, *p4;

  if(strlen(command) == 1) {
    if(!HistoryMoveToEntry(&searchEngines, defaultEngine)) {
      MessageBox(NULL, "Could not find default search engine.  If you did\nnot specify one in your search engine list,\nplease do so.", "LSXCommand", MB_OK);
      return;
    }
  } else if(strlen(command) > 1) {
    ++command;
    if(!HistoryMoveToEntry(&searchEngines, command)) {
      MessageBox(NULL, "Could not find specified search engine.", "LSXCommand", MB_OK);
      return;
    }
  }

  // This is hell, but I like to save memory...
  p = args;
  while(*p) {
    if(*p == ' ')
      *p = '+';
    p++;
  }

  p = (char *)malloc(strlen(searchEngines->path) + 1);
  strcpy(p, searchEngines->path);
  p2 = strtok(p, "\t ");
  p2 = strtok(NULL, "\t\n ");
  p3 = strstr(p2, "<args>");

  p4 = (char *)malloc(strlen(p2) + strlen(args) + 1);
  strncpy(p4, p2, (p3 - p2)*sizeof(char));
  p4[p3 - p2] = '\0';
  strcat(p4, args);
  p3 += 6*sizeof(char);
  strcat(p4, p3);

  ShellExecute(hWnd, "open", p4, "", 0, SW_SHOWDEFAULT);

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
    if(HistoryMoveToEntry(&aliases, command)) {
      buf = (char *)malloc(strlen(aliases->path) + 1);
      strcpy(buf, aliases->path);
      p = strtok(buf, "\t ");
      p = strtok(NULL, "\t\n ");
      p2 = strtok(NULL, "\n");
      if(p2) {
        p3 = (char *)malloc(strlen(p2) + strlen(args) + 2);
        strcpy(p3, p2);
        if(*args) {
          strcat(p3, " ");
          strcat(p3, args);
        }
      }

      ShellExecute(hWnd, "open", p, p3 ? p3 : "", 0, SW_SHOWDEFAULT);
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
* void SearchEngineInit()                                  *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*                         *  *  *  *                       *
* Initializes the list of Aliases.  If no aliases are      *
* found, the feature simply doesn't work.                  *
***********************************************************/

void AliasInit()
{
  FILE *f = LCOpen(NULL);
  char buffer[_MAX_PATH + 20], name[20], path[_MAX_PATH], prefix[15], extra[_MAX_PATH];
  char* tokens[3];

  tokens[0] = prefix;
  tokens[1] = name;
  tokens[2] = path;

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
      }
    }
  }

  if(f)
    LCClose(f);
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
	return;
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
  if(cs->SelectAllOnFocus) SendMessage(hText, EM_SETSEL, 0, -1);
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
		ShowWindow(hWnd,SW_HIDE);
		if(cs->ClearOnHide) SetWindowText(hText,"");
		visible = FALSE;
	}
	else {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		SetForegroundWindow(hText);
		SetFocus(hText);
    if(cs->SelectAllOnFocus) SendMessage(hText, EM_SETSEL, 0, -1);
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

void BangRescanEngineList(HWND caller, char *args)
{
  HistoryRemoveAll(&searchEngines, &nSearchEngines);
  SearchEngineInit(cs);
}


/***********************************************************
* void ExecCommand()                                       *
*                         *  *  *  *                       *
*    Arguments:                                            *
*                                                          *
*    - char *command                                       *
*      Pointer to the command to execute                   *
*                         *  *  *  *                       *
* This function checks for a valid command and sends it to *
* the proper place - ParseBang, ParseSearch, or ShellEx    *
***********************************************************/

void ExecCommand(char *command)
{
	char args[256];

	if(strlen(command)<1)
		return;

  if(HistoryFindIndexOf(&current, command) <= 0) {
    if(!(nHistoryEntries < cs->MaxHistoryEntries)) {
      HistoryMoveToHead(&current);
      HistoryRemoveEntry(&current, &nHistoryEntries);
    }
    HistoryAdd(&current, command, &nHistoryEntries);
  } else {
    // Bring this command to the front of the history.
    HistoryRemoveEntry(&current, &nHistoryEntries);
    HistoryAdd(&current, command, &nHistoryEntries);
  }

	strcpy(args,PathGetArgs(command));
	PathRemoveArgs(command);
	
  if(!AliasExecute(command, args)) {
	  if(command[0] == '!')
		  ParseBangCommand(hWnd,command,args);
    else if(command[0] == '?') {
      if(searchEngines)
        ParseSearchCommand(command, args);
      else
        MessageBox(hWnd, "You have not specified any search engines, or specified\nan invalid search engine list.  You can not use the search feature at this time.", "LSXCommand", MB_OK);
    }
	  else
		  ShellExecute(hWnd,"open",command,args,0,SW_SHOWDEFAULT);
  }
	return;
}


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
	char buf[_MAX_PATH], *buf2, *buf3;
	switch(msg) {
	case WM_KEYDOWN:
		if(wParam==VK_DOWN) {
			HistoryUsePrev(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
		}
		if(wParam==VK_UP) {
			HistoryUseNext(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
		}
		break;
	case WM_CHAR:
		if(wParam==VK_RETURN) {
			GetWindowText(hText,buf,sizeof(buf));
			if(!cs->NoClearOnCommand) SetWindowText(hText,"");
			if(cs->HideOnCommand) {
				ShowWindow(hWnd,SW_HIDE);
				visible = FALSE;
				if(cs->ClearOnHide) SetWindowText(hText,"");
			}
			ExecCommand(buf);
      HistoryMoveToTail(&current);
			return 0;
    } else {
      DWORD nStart, nEnd;
      SendMessage(hText, EM_GETSEL, (WPARAM)&nStart, (LPARAM)&nEnd);
      GetWindowText(hText, buf, sizeof(buf));
      buf2 = (char *)malloc(strlen(buf) + 1);
      buf3 = buf + nEnd*sizeof(char);
      strcpy(buf2, "");
      strncpy(buf2, buf, nStart);
      buf2[nStart] = '\0';
      if(strlen(buf2) < _MAX_PATH-2) {
        strcat(buf2, (const char *)&wParam);
        strcat(buf2, buf3);
        HistoryMoveNext(&current, 1);
        if(AutoComplete(hText, buf2))
          return 0;
        else HistoryMoveToTail(&current);  // We got to something that is not in the history.
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
		return 0;
		}
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
		hBG = CreateWindowEx(0,"STATIC","",WS_CHILD,cs->BorderSize,cs->BorderSize,cs->width-(cs->BorderSize*2),cs->height-(cs->BorderSize*2),hWnd,0,hInst,0);
		wpBG = (WNDPROC)SetWindowLong(hText,GWL_WNDPROC,(long)BGProc);
		//DragAcceptFiles(hBG, TRUE);
		hFont = CreateFont(cs->TextSize,0,0,0,0,0,0,0,0,0,0,0,0,cs->TextFontFace);
		hText = CreateWindowEx(0,"EDIT","",WS_CHILD|ES_AUTOHSCROLL,cs->BorderSize,(cs->height/2)-(cs->TextSize/2),cs->width-(cs->BorderSize*2),cs->TextSize,hWnd,0,hInst,0);
    hBGBrush = CreateSolidBrush(cs->BGColor);
		if(!hText) {
			MessageBox(hWnd,"Error creating window",szApp,MB_OK);
		}
		wpOld = (WNDPROC)SetWindowLong(hText,GWL_WNDPROC,(long)EditProc);
		//DragAcceptFiles(hBG, TRUE);
		SendMessage(hText,WM_SETFONT,(WPARAM)hFont,FALSE);
		ShowWindow(hBG,SW_SHOW);
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
		SetTextColor((HDC)wParam, cs->TextColor);
    SetBkColor((HDC)wParam, cs->BGColor);
    return (int)hBGBrush;
	//	return (int)CreateSolidBrush(cs.BGColor);
	case WM_CTLCOLORSTATIC:
    SetBkColor((HDC)wParam, cs->BGColor);
    return (int)hBGBrush;
	//	return (int)CreateSolidBrush(cs.BGColor);
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
	return DefWindowProc(hWnd,msg,wParam,lParam);
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
	WNDCLASS wc;
	HBRUSH hBr;

	cs = ReadSettings(wd);
	hBr = CreateSolidBrush(cs->BorderColor);
  HistoryInit();
  SearchEngineInit(cs);
  AliasInit();
	hInst = dll;

	memset(&wc,0,sizeof(wc));
	wc.hInstance = hInst;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = szApp;
	wc.hbrBackground = hBr;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	RegisterClass(&wc);
	hWnd = CreateWindowEx(((cs->NoAlwaysOnTop)?0:WS_EX_TOPMOST)|WS_EX_TOOLWINDOW,szApp,"",WS_POPUP|((cs->BevelBorder)?WS_SIZEBOX:0),cs->x,cs->y,cs->width,cs->height,GetDesktopWindow(),0,hInst,0);
	
  //DragAcceptFiles(hWnd, TRUE);
	SetWindowLong(hWnd,GWL_USERDATA,magicDWord);
	AddBangCommand("!TOGGLECOMMAND",BangToggleCommand);
	AddBangCommand("!FOCUSCOMMAND",BangFocusCommand);
  AddBangCommand("!COMMANDRESCANENGINES", BangRescanEngineList);
	
  if(!cs->HiddenOnStart) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
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
	WriteHistory();
  HistoryRemoveAll(&current, &nHistoryEntries);
  HistoryRemoveAll(&searchEngines, &nSearchEngines);
  HistoryRemoveAll(&aliases, &nAliases);
	DestroyWindow(hWnd);
	UnregisterClass(szApp,hInst);
	DeleteObject(hFont);
	return 0;
}