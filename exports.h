#ifndef EXPORTS_H
#define EXPORTS_H

#include "wharfdata.h"

/* Exported Functions */
__declspec( dllexport ) int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd);
__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
__declspec( dllexport ) int quitModule(HINSTANCE dll);

#endif