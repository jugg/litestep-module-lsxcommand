#ifndef __WHARFDATA_H
#define __WHARFDATA_H

/* ----------------------------------------------------------------- */

typedef struct {
    HWND Handle;
    BOOL Visible;
    RECT Position;
    int Desk;
    } windowType;

typedef struct {
    int borderSize;
    int pos;
    int winListSize;
    windowType *winList;
    int trayIconSize;
    int lm78Unit;
    int lm78MaxCpu;
    int lm78MaxMb;
    int lm78Cpu;
    int taskBar;
    int msTaskBar;
    int taskBarFore;
    int taskBarBack;
    int taskBarText;
    int vwmVelocity;
    int autoHideWharf;
    int autoHideTaskbar;
    int VWMDistance;
    int autoHideDelay;
    int showBeta;
    int usClock;
    int VWMNoGathering;
    int VWMNoAuto;
    int xmouseDelay;
    int xmouseBringToTop;
    char *pixmapDir;
    char *defaultBmp;
    int vwmBackColor;
    int vwmSelBackColor;
    int vwmForeColor;
    int vwmBorderColor;
    char *backsaver;
    int taskBarFore2;
    int stripBar;
    char *lsPath;
    } wharfDataType;

/* ----------------------------------------------------------------- */

const long magicDWord = 0x49474541;

/* ----------------------------------------------------------------- */

#endif
