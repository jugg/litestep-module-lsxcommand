#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "wharfdata.h"

struct CommandSettings {
	char TextFontFace[256], SearchEngineList[_MAX_PATH];
	int TextSize;
	int x;
	int y;
	int width;
	int height;
	int BorderSize;
  int MaxHistoryEntries;
	BOOL NoCursorChange;
	BOOL BevelBorder;
	BOOL NoAlwaysOnTop;
	BOOL notmoveable;
	BOOL NoClearOnCommand;
	BOOL HideOnCommand;
	BOOL HiddenOnStart;
	BOOL ClearOnHide;
  BOOL SelectAllOnFocus;
	COLORREF BGColor;
	COLORREF TextColor;
	COLORREF BorderColor;
};

struct  History {
  char *path;
  struct History *next, *prev;
};

__declspec( dllexport ) int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd);
__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
__declspec( dllexport ) int quitModule(HINSTANCE dll);

#endif