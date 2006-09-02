# Microsoft Developer Studio Project File - Name="tinyxmlSTL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tinyxmlSTL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tinyxmlSTL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tinyxmlSTL.mak" CFG="tinyxmlSTL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tinyxmlSTL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tinyxmlSTL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tinyxmlSTL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug_STL"
# PROP BASE Intermediate_Dir ".\Debug_STL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug_STL"
# PROP Intermediate_Dir ".\Debug_STL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "TIXML_USE_STL" /D "_MBCS" /Fp".\Debug_STL/tinyxmlSTL.pch" /Fo".\Debug_STL/" /Fd".\Debug_STL/" /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "TIXML_USE_STL" /D "_MBCS" /Fp".\Debug_STL/tinyxmlSTL.pch" /Fo".\Debug_STL/" /Fd".\Debug_STL/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Debug_STL\tinyxmld_STL.lib" 
# ADD LIB32 /nologo /out:".\Debug_STL\tinyxmld_STL.lib" 

!ELSEIF  "$(CFG)" == "tinyxmlSTL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release_STL"
# PROP BASE Intermediate_Dir ".\Release_STL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release_STL"
# PROP Intermediate_Dir ".\Release_STL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "TIXML_USE_STL" /D "_MBCS" /GF /Gy /Fp".\Release_STL/tinyxmlSTL.pch" /Fo".\Release_STL/" /Fd".\Release_STL/" /FR /c /GX 
# ADD CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "TIXML_USE_STL" /D "_MBCS" /GF /Gy /Fp".\Release_STL/tinyxmlSTL.pch" /Fo".\Release_STL/" /Fd".\Release_STL/" /FR /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Release_STL\tinyxml_STL.lib" 
# ADD LIB32 /nologo /out:".\Release_STL\tinyxml_STL.lib" 

!ENDIF

# Begin Target

# Name "tinyxmlSTL - Win32 Debug"
# Name "tinyxmlSTL - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\tinyxml\tinyxml.h
# End Source File
# End Group
# End Target
# End Project

