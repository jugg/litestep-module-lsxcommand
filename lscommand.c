#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include "lscommand.h"
#include "lsapi.h"

#define MAX_RUN_STRING 1024
#define MAX_RUN_HISTORY 10

char szHistory[MAX_RUN_HISTORY][MAX_RUN_STRING];
int nHistoryIndex;
HWND hWnd,hText, hBG;
HFONT hFont;
WNDPROC wpOld, wpBG;
HINSTANCE hInst;
BOOL visible = FALSE, editfirst = FALSE, staticfirst = FALSE;
char *szApp = "LSCommand";
CommandSettings cs;


void HistoryInit()
{
	// i got some of the history init stuff from re5ources popups.
	HKEY hKey;
	char pszValueData[MAX_RUN_STRING];
	char* pszKey = "Software\\lscommand\\history";
	DWORD nType;
	DWORD nValueLength;
	BOOL bSuccess=TRUE;
	int i;
	
	// Open the registry key
	if(bSuccess)
		bSuccess = RegCreateKey(HKEY_CURRENT_USER, pszKey, &hKey) == ERROR_SUCCESS;

	if(bSuccess)
	{
		char pszName[2];

		ZeroMemory(pszName, sizeof(pszName));

		for(i=0; i < MAX_RUN_HISTORY; i++)
		{
			sprintf(pszName,"%i",i);
			nValueLength = sizeof(pszValueData);
			bSuccess = RegQueryValueEx(hKey, pszName, NULL, &nType, (PBYTE)pszValueData, &nValueLength) == ERROR_SUCCESS;
			if(bSuccess)
			{
				strcpy(szHistory[i], (char*)pszValueData);
			}
		}
	}

	if(bSuccess)
		RegCloseKey(hKey);

	nHistoryIndex = 0;
}


void HistoryPrev(HWND hText)
{
	char *szBuf = "";
	if(nHistoryIndex==0)
		szBuf = "";
	else {
		szBuf = szHistory[nHistoryIndex-2];
		nHistoryIndex--;
	}
	SetWindowText(hText,szBuf);
	return;
}


void HistoryNext(HWND hText)
{
	char *szBuf = "";
	if(nHistoryIndex==MAX_RUN_STRING)
		szBuf = szHistory[nHistoryIndex];
	else if(strlen(szHistory[nHistoryIndex])<1) {
		if(nHistoryIndex>0)
			szBuf = szHistory[nHistoryIndex-1];
		else
			szBuf = "";
	}
	else {
		szBuf = szHistory[nHistoryIndex];
		nHistoryIndex++;
	}
	SetWindowText(hText,szBuf);
	return;
}

void HistoryAdd(char* pszCommand)
{
	int i;
	for(i=MAX_RUN_HISTORY;i>=0;i--)
		if(szHistory[i] && i+1<MAX_RUN_HISTORY) strcpy(szHistory[i+1],szHistory[i]);
	strcpy(szHistory[0],pszCommand);
	return;
}


int initModule(HWND parent, HINSTANCE dll, wharfDataType* wd)
{
	return initModuleEx(parent,dll,wd->lsPath);
}


void FocusCommand(HWND caller,char *args)
{
	if(!visible) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
	}
	SetForegroundWindow(hText);
	SetFocus(hText);
}


void ToggleCommand(HWND caller, char *args)
{
	if(visible) {
		ShowWindow(hWnd,SW_HIDE);
		if(cs.ClearOnHide) SetWindowText(hText,"");
		visible = FALSE;
	}
	else {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		SetForegroundWindow(hText);
		SetFocus(hText);
		visible = TRUE;
	}
	return;
}


CommandSettings ReadSettings()
{
	CommandSettings settings;
	GetRCString("CommandTextFontFace",settings.TextFontFace,"Arial",256);
	settings.BGColor = GetRCColor("CommandBGColor",RGB(255,255,255));
	settings.TextColor = GetRCColor("CommandTextColor",RGB(0,0,0));
	settings.TextSize = GetRCInt("CommandTextSize",14);
	settings.x = GetRCInt("CommandX",0);
	settings.y = GetRCInt("CommandY",0);
	settings.width = GetRCInt("CommandWidth",160);
	settings.height = GetRCInt("CommandHeight",20);
	settings.BorderSize = GetRCInt("CommandBorderSize",2);
	settings.BorderColor = GetRCColor("CommandBorderColor",RGB(102,102,102));
	settings.BevelBorder = GetRCBool("CommandBevelBorder",TRUE);
	if(settings.BevelBorder) settings.BorderSize = 0; // if bevel then no border
	settings.NoAlwaysOnTop = GetRCBool("CommandNotAlwaysOnTop",TRUE);
	settings.notmoveable = GetRCBool("CommandNotMoveable",TRUE);
	settings.NoClearOnCommand = GetRCBool("CommandNoClearOnCommand",TRUE);
	settings.HideOnCommand = GetRCBool("CommandHideOnCommand",TRUE);
	settings.HiddenOnStart = GetRCBool("CommandHiddenOnStart",TRUE);
	settings.ClearOnHide = GetRCBool("CommandClearOnHide",TRUE);
	settings.NoCursorChange = GetRCBool("CommandNoCursorChange",TRUE);
	return settings;
}


void ExecCommand(char *command)
{
	char args[256];

	if(strlen(command)<1)
		return;

	HistoryAdd(command);
	nHistoryIndex = 0;

	strcpy(args,PathGetArgs(command));
	PathRemoveArgs(command);
	
	if(command[0] == '!')
		ParseBangCommand(hWnd,command,args);
	else
		ShellExecute(hWnd,"open",command,args,0,SW_SHOWDEFAULT);
	return;
}


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


BOOL CALLBACK EditProc(HWND hText,UINT msg,WPARAM wParam,LPARAM lParam)
{
	char buf[MAX_RUN_STRING];
	switch(msg) {
	case WM_KEYDOWN:
		if(wParam==VK_DOWN) {
			HistoryPrev(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
		}
		if(wParam==VK_UP) {
			HistoryNext(hText);
			SendMessage(hText,EM_SETSEL,0,-1);
			return 0;
		}
		break;
	case WM_CHAR:
		if(wParam==VK_RETURN) {
			GetWindowText(hText,buf,sizeof(buf));
			if(!cs.NoClearOnCommand) SetWindowText(hText,"");
			if(cs.HideOnCommand) {
				ShowWindow(hWnd,SW_HIDE);
				visible = FALSE;
				if(cs.ClearOnHide) SetWindowText(hText,"");
			}
			ExecCommand(buf);
			return 0;
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


BOOL CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg) {
	case WM_CREATE:
		{
		HistoryInit();
		hBG = CreateWindowEx(0,"STATIC","",WS_CHILD,cs.BorderSize,cs.BorderSize,cs.width-(cs.BorderSize*2),cs.height-(cs.BorderSize*2),hWnd,0,hInst,0);
		wpBG = (WNDPROC)SetWindowLong(hText,GWL_WNDPROC,(long)BGProc);
		//DragAcceptFiles(hBG, TRUE);
		hFont = CreateFont(cs.TextSize,0,0,0,0,0,0,0,0,0,0,0,0,cs.TextFontFace);
		hText = CreateWindowEx(0,"EDIT","",WS_CHILD|ES_AUTOHSCROLL,cs.BorderSize,(cs.height/2)-(cs.TextSize/2),cs.width-(cs.BorderSize*2),cs.TextSize,hWnd,0,hInst,0);
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
		if(cs.NoCursorChange) return TRUE;
		else break;
	case WM_NCHITTEST:
		if(cs.notmoveable) return HTNOWHERE;
		else return HTCAPTION;
	////// THERES SOMETHING CAUSEING SOME KIND OF MEM LEAK HERE
	////// IF YOU KNOW WHAT IT IS, LET ME KNOW
	case WM_CTLCOLOREDIT:
		SetTextColor((HDC)wParam,cs.TextColor);
		return (int)CreateSolidBrush(cs.BGColor);
	case WM_CTLCOLORSTATIC:
		return (int)CreateSolidBrush(cs.BGColor);
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


int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath)
{
	WNDCLASS wc;
	HBRUSH hBr;
	cs = ReadSettings();
	hBr = CreateSolidBrush(cs.BorderColor);
	hInst = dll;
	memset(&wc,0,sizeof(wc));
	wc.hInstance = hInst;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = szApp;
	wc.hbrBackground = hBr;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	RegisterClass(&wc);
	hWnd = CreateWindowEx(((cs.NoAlwaysOnTop)?0:WS_EX_TOPMOST)|WS_EX_TOOLWINDOW,szApp,"",WS_POPUP|((cs.BevelBorder)?WS_SIZEBOX:0),cs.x,cs.y,cs.width,cs.height,GetDesktopWindow(),0,hInst,0);
	//DragAcceptFiles(hWnd, TRUE);
	SetWindowLong(hWnd,GWL_USERDATA,magicDWord);
	AddBangCommand("!TOGGLECOMMAND",ToggleCommand);
	AddBangCommand("!FOCUSCOMMAND",FocusCommand);
	if(!cs.HiddenOnStart) {
		ShowWindow(hWnd,SW_SHOWNORMAL);
		visible = TRUE;
	}
	return 0;
}


void WriteHistory()
{
	HKEY hKey;
	char* pszKey = "Software\\lscommand\\history";
	BOOL bSuccess=TRUE;
	int i;
	
	if(bSuccess)
		bSuccess = RegCreateKey(HKEY_CURRENT_USER, pszKey, &hKey) == ERROR_SUCCESS;

	if(bSuccess)
	{
		char pszName[2];

		ZeroMemory(pszName, sizeof(pszName));

		for(i=0; i < MAX_RUN_HISTORY; i++)
		{
			sprintf(pszName,"%i",i);
			RegSetValueEx(hKey,pszName,0,REG_SZ,szHistory[i],strlen(szHistory[i]));
		}
	}

	if(bSuccess)
		RegCloseKey(hKey);

	nHistoryIndex = 0;
}


int quitModule(HINSTANCE dll)
{
	WriteHistory();
	DestroyWindow(hWnd);
	UnregisterClass(szApp,hInst);
	DeleteObject(hFont);
	return 0;
}