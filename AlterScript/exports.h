/******************************************************************************
*                                                                             *
* This is a part of the AlterScript LiteStep module Source code.              *
*                                                                             *
* Copyright (C) 1999 Visigoth (Shaheen Gandhi in real life)                   *
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