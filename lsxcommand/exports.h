#ifndef EXPORTS_H
#define EXPORTS_H

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
#include <utility/wharfdata.h>

/* Exported Functions */
__declspec( dllexport ) int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd);
__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
__declspec( dllexport ) int quitModule(HINSTANCE dll);

#endif