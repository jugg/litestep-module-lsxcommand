#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#ifndef LSXCOMMANDCLOCK_EXPORTS

#define HMI_USER_ABOUT WM_USER
#define HMI_USER_SE WM_USER + 1
#define HMI_USER_ALIAS HMI_USER_SE + nSearchEngines + 1
#define HMI_USER_HISTORY HMI_USER_ALIAS + nAliases + 1
#define HMI_USER_CUT HMI_USER_HISTORY + cs->MaxHistoryMenuEntries + 1
#define HMI_USER_COPY HMI_USER_CUT + 1
#define HMI_USER_PASTE HMI_USER_COPY + 1

/* Data Structures */
struct CommandSettings {
	char TextFontFace[256], SearchEngineList[_MAX_PATH], ContextMenuOrder[4], Background[_MAX_PATH], Clock[256], TextAlign[15], BrowserPath[_MAX_PATH];
	int TextSize;
	int x;
	int y;
	int width;
	int height;
	int BorderSize;
  int MaxHistoryEntries;
  int MaxHistoryMenuEntries;
  int Transparent;
  int ContextMenuStandardItems;
	BOOL NoCursorChange;
	BOOL BevelBorder;
	BOOL NoAlwaysOnTop;
	BOOL notmoveable;
	BOOL NoClearOnCommand;
	BOOL HideOnCommand;
	BOOL HiddenOnStart;
	BOOL ClearOnHide;
  BOOL SelectAllOnFocus;
  BOOL SelectAllOnMouseFocus;
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
  BOOL ScrollWinAmp;
  BOOL WinAmpDisappears;
  BOOL TabFileComplete;
  BOOL HideOnUnfocus;
  BOOL AddExternalsToHistory;
	COLORREF BGColor;
	COLORREF TextColor;
	COLORREF BorderColor;
};

struct  History {
  char *path;
  struct History *next, *prev;
};

/* Settings */
extern struct CommandSettings *cs;

/* Execution */
extern void ExecCommand(char *command, BOOL alias);

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

extern double Evaluate(char *expr, int *error, char *replacement, size_t replace_size);

/* Menu Services - menu.c */
extern void MenuDeleteItem(HMENU hMenu, int index);
extern void MenuAddItem(HMENU hMenu, char *pszValue, long def_id, BOOL insertAtTop);

#else // LSXCOMMANDCLOCK_EXPORTS

struct CommandSettings {
	char TextFontFace[256], Background[_MAX_PATH], Clock[256];
	int TextSize;
	int x;
	int y;
	int width;
	int height;
	int BorderSize;
  /*
  int MaxHistoryEntries;
  int MaxHistoryMenuEntries;
  */
  int Transparent;
  /*
  int ContextMenuStandardItems;
  */
	BOOL NoCursorChange;
	BOOL BevelBorder;
	BOOL NoAlwaysOnTop;
	BOOL notmoveable;
  /*
	BOOL NoClearOnCommand;
	BOOL HideOnCommand;
  */
	BOOL HiddenOnStart;
  /*
	BOOL ClearOnHide;
  */
  BOOL SelectAllOnFocus;
  BOOL SelectAllOnMouseFocus;
  /*
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
  */
  BOOL ScrollWinAmp;
  /*
  BOOL WinAmpDisappears;
  BOOL TabFileComplete;
  */
  BOOL HideOnUnfocus;
	COLORREF BGColor;
	COLORREF TextColor;
	COLORREF BorderColor;
};

/* Timer Procedure must be declared for LSXCommandClock */
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

#endif //LSXCOMMANDCLOCK_EXPORTS

#endif //LSCOMMAND_H