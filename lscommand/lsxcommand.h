#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "resource.h"

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
	char TextFontFace[256], SearchEngineList[_MAX_PATH], ContextMenuOrder[4], Background[_MAX_PATH], Clock[256], TextAlign[15], BrowserPath[_MAX_PATH];
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
};

#ifndef LSXCOMMANDCLOCK_EXPORTS

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