#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "wharfdata.h"

typedef struct {
	char TextFontFace[256];
	int BGColor;
	int TextColor;
	int TextSize;
	int x;
	int y;
	int width;
	int height;
	int BorderColor;
	int BorderSize;
	BOOL NoCursorChange;
	BOOL BevelBorder;
	BOOL NoAlwaysOnTop;
	BOOL notmoveable;
	BOOL NoClearOnCommand;
	BOOL HideOnCommand;
	BOOL HiddenOnStart;
	BOOL ClearOnHide;
} CommandSettings;

__declspec( dllexport ) int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd);
__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
__declspec( dllexport ) int quitModule(HINSTANCE dll);

#endif