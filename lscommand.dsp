# Microsoft Developer Studio Project File - Name="lscommand" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=lscommand - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lscommand.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lscommand.mak" CFG="lscommand - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lscommand - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "lscommand - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lscommand - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G4 /MD /W3 /Gi /vd0 /GX /O1 /Ob2 /I "c:\litestep\source\ls-b24\litestep" /I "c:\litestep\source\ls-b24\lsapi" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DLL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib lsapi.lib shlwapi.lib /nologo /base:"0x15000000" /dll /machine:I386 /out:"Release/lsxcommand.dll" /libpath:"c:\litestep\source\ls-b24\lsapi\Release" /opt:nowin98
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\lsxcommand.dll c:\litestep\bebox\bemodules\lsxcommand.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "lscommand - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "lscommand_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "c:\litestep\source\ls-b24\litestep" /I "c:\litestep\source\ls-b24\lsapi" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "lscommand_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lsapi.lib shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/lsxcommand.dll" /pdbtype:sept /libpath:"c:\litestep\source\ls-b24\lsapi\Debug"
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\lsxcommand.dll c:\litestep\bebox\bemodules\debug\lsxcommand.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "lscommand - Win32 Release"
# Name "lscommand - Win32 Debug"
# Begin Group "LSXCommand"

# PROP Default_Filter ""
# Begin Group "Headers"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\exports.h
# End Source File
# Begin Source File

SOURCE="..\ls-b24\lsapi\lsapi.h"
# End Source File
# Begin Source File

SOURCE=.\lsxcommand.h
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\calculator.c
# End Source File
# Begin Source File

SOURCE=.\history.c
# End Source File
# Begin Source File

SOURCE=.\lsxcommand.c
# End Source File
# Begin Source File

SOURCE=.\menu.c
# End Source File
# End Group
# Begin Group "Rescources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# End Group
# Begin Group "Original LSCommand"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lscommand.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\lscommand.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
