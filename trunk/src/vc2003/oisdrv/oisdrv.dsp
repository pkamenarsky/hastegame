# Microsoft Developer Studio Project File - Name="oisdrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=oisdrv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "oisdrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "oisdrv.mak" CFG="oisdrv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "oisdrv - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "oisdrv - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "oisdrv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../../bin"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../bin"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion_d.lib ../extern/ois/Debug/OIS_d.lib /nologo /dll /out:"..\..\..\bin\oisdrv_d.dll" /incremental:yes /debug /pdb:"..\..\..\bin\oisdrv_d.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/oisdrv_d.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion_d.lib ../extern/ois/Debug/OIS_d.lib /nologo /dll /out:"..\..\..\bin\oisdrv_d.dll" /incremental:yes /debug /pdb:"..\..\..\bin\oisdrv_d.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/oisdrv_d.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "oisdrv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../../bin"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion.lib ../extern/ois/Release/OIS.lib /nologo /dll /out:"..\..\..\bin\oisdrv.dll" /incremental:no /debug /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/oisdrv.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion.lib ../extern/ois/Release/OIS.lib /nologo /dll /out:"..\..\..\bin\oisdrv.dll" /incremental:no /debug /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/oisdrv.lib" /machine:ix86 

!ENDIF

# Begin Target

# Name "oisdrv - Win32 Debug"
# Name "oisdrv - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\oisdrv\oisjoystick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oiskeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oismouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oisserver.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\oisdrv\oisjoystick.hh
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oiskeyboard.hh
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oismouse.hh
# End Source File
# Begin Source File

SOURCE=..\..\oisdrv\oisserver.hh
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
# End Group
# End Target
# End Project

