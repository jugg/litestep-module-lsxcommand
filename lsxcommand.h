#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#define HMI_USER_ABOUT WM_USER
#define HMI_USER_SE WM_USER + 1
#define HMI_USER_ALIAS HMI_USER_SE + nSearchEngines + 1
#define HMI_USER_HISTORY HMI_USER_ALIAS + nAliases + 1

/* Data Structures */
struct CommandSettings {
	char TextFontFace[256], SearchEngineList[_MAX_PATH], ContextMenuOrder[3];
	int TextSize;
	int x;
	int y;
	int width;
	int height;
	int BorderSize;
  int MaxHistoryEntries;
  int MaxHistoryMenuEntries;
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
	COLORREF BGColor;
	COLORREF TextColor;
	COLORREF BorderColor;
};

struct  History {
  char *path;
  struct History *next, *prev;
};

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

extern double Evaluate(char *expr, struct CommandSettings *cs, BOOL *error);

/* Menu Services - menu.c */

extern void MenuDeleteItem(HMENU hMenu, int index);
extern void MenuAddItem(HMENU hMenu, char *pszValue, long def_id, BOOL insertAtTop);

#endif