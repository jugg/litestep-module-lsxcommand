/******************************************************************************
*                                                                             *
* This is a part of the LsxCommand LiteStep plugin Source code.               *
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

#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "resource.h"
#include "aggressiveoptimize.h"

/* Timer Enumeration */
enum Timers {
  NO_TIMER = 0,
  CLOCK_TIMER = ID_CLOCKTIMER,
  WINAMP_TIMER = ID_WINAMPTIMER
};

#ifndef LSXCOMMANDCLOCK_EXPORTS

/* Used for Context Menu entries */
#define HMI_USER_ABOUT WM_USER
#define HMI_USER_SE WM_USER + 1
#define HMI_USER_ALIAS HMI_USER_SE + nSearchEngines + 1
#define HMI_USER_HISTORY HMI_USER_ALIAS + nAliases + 1
#define HMI_USER_CUT HMI_USER_HISTORY + cs->MaxHistoryMenuEntries + 1
#define HMI_USER_COPY HMI_USER_CUT + 1
#define HMI_USER_PASTE HMI_USER_COPY + 1

#endif

/* Data Structures */
struct CommandSettings {
	//image sizes
	int m_nWidth;
	int m_nHeight;
	BOOL m_bStretchImage;
	//font
	BOOL m_bTextBold;
	BOOL m_bTextUnderline;
	BOOL m_bTextItalic; 
	//go on with what was before
	char TextFontFace[256], SearchEngineList[_MAX_PATH], ContextMenuOrder[4], Background[_MAX_PATH], Clock[256], TextAlign[15], BrowserPath[_MAX_PATH], MusicPlayer[128];
	int TextSize;
	int x, origx;
	int y, origy;
	int width;
	int height;
	int BorderSize;
	int TopBorderSize;
	int BottomBorderSize;
	int LeftBorderSize;
	int RightBorderSize;
  int Transparent;
#ifndef LSXCOMMANDCLOCK_EXPORTS
  int MaxHistoryEntries;
  int MaxHistoryMenuEntries;
  int ContextMenuStandardItems;
#endif // LSXCOMMANDCLOCK_EXPORTS
	BOOL NoCursorChange;
	BOOL BevelBorder;
	BOOL NoAlwaysOnTop;
	BOOL notmoveable;
	BOOL HiddenOnStart;
	BOOL WaitForBox;
  BOOL SelectAllOnFocus;
  BOOL SelectAllOnMouseFocus;
  BOOL ScrollWinAmp;
  BOOL HideOnUnfocus;
  BOOL AddExternalsToHistory;
#ifndef LSXCOMMANDCLOCK_EXPORTS
  BOOL AssumeNetAddress;
  BOOL NoWarnOnError;
  BOOL ClearHistoryOnStartup;
  BOOL NoTabMicroComplete;
  BOOL NewestHistoryItemsOnTop;
  BOOL ContextMenuAutoPopup;
  BOOL ContextMenuAboveBox;
  BOOL ContextMenuExecute;
  BOOL UnixHistory;
  BOOL ExplorePaths;
  BOOL AutoComplete;
  BOOL CommaDelimiter;
  BOOL RPNCalculator;
  BOOL ClockDisappears;
	BOOL NoClearOnCommand;
    BOOL HideOnCommand;
    BOOL ClearOnHide;
    BOOL WinAmpDisappears;
    BOOL TabFileComplete;
#endif // LSXCOMMANDCLOCK_EXPORTS
    COLORREF BGColor;
    COLORREF TextColor;
    COLORREF BorderColor;
    //20021114
    char* OnFocusCommand;
    char* OnUnfocusCommand;
};

#ifndef LSXCOMMANDCLOCK_EXPORTS

struct  History {
    char *path;
    struct History *next, *prev;
};

/* Settings */
extern struct CommandSettings *cs;

/* Execution */
extern void ExecCommand(const char *szCommand, BOOL alias);

/* History Services - history.c */
extern void HistoryMoveNext(struct History **hist, int i);
extern void HistoryMovePrev(struct History **hist, int i);
extern void HistoryMoveToHead(struct History **hist);
extern void HistoryMoveToTail(struct History **hist);
extern int HistoryFindIndexOf(struct History **hist, char *pszValue);
extern BOOL HistoryMoveToEntry(struct History **hist, char *pszValue, BOOL tokenize);
extern struct History *HistoryAdd(struct History **hist, char *pszValue, int *count);
extern void HistoryFree(struct History **hist);
extern void HistoryRemoveEntry(struct History **hist, int *count);
extern struct History *HistoryRemoveAll(struct History **hist, int *count);

/* Calculator Services - calculator.c */
#define STATE_NONE 0
#define STATE_OPERAND 1
#define STATE_UNARYOP 2
#define STATE_OPERATOR 3
#define UNARY_NEG "(-)"
#define UNARY_DEC "d"
#define UNARY_BIN "b"
#define UNARY_HEX "h"
#define ERROR_POSTFIX_CONVERSION 1
#define ERROR_NULL_EXPRESSION    2
#define ERROR_CONVERSIONS_DONE   3
#define ERROR_EVALUATE           4

/*extern double Evaluate(char *expr, int *error, char *replacement, size_t replace_size);*/

/* Menu Services - menu.c */
extern void MenuDeleteItem(HMENU hMenu, int index);
extern void MenuAddItem(HMENU hMenu, char *pszValue, long def_id, BOOL insertAtTop);

#else // LSXCOMMANDCLOCK_EXPORTS

/* Timer Procedures - must be declared for LSXCommandClock */
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
VOID CALLBACK WinAmpTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

#endif //LSXCOMMANDCLOCK_EXPORTS

#endif //LSCOMMAND_H