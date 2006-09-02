# Microsoft Developer Studio Project File - Name="OIS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OIS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OIS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OIS.mak" CFG="OIS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OIS - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "OIS - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OIS - Win32 OIS_ReleaseDll" (based on "Win32 (x86) Static Library")
!MESSAGE "OIS - Win32 OIS_DebugDll" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OIS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "../../../extern/ois/includes" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_STLP_DEBUG" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /Gd /GZ /c /GX 
# ADD CPP /nologo /MTd /I "../../../extern/ois/includes" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_STLP_DEBUG" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /Gd /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\OIS_d.lib" 
# ADD LIB32 /nologo /out:"Debug\OIS_d.lib" 

!ELSEIF  "$(CFG)" == "OIS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "../../../extern/ois/includes" /Zi /W3 /O2 /Ob1 /Oy /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp"$(IntDir)/$(TargetName).pch" /c /GX 
# ADD CPP /nologo /MT /I "../../../extern/ois/includes" /Zi /W3 /O2 /Ob1 /Oy /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp"$(IntDir)/$(TargetName).pch" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\OIS.lib" 
# ADD LIB32 /nologo /out:"Release\OIS.lib" 

!ELSEIF  "$(CFG)" == "OIS - Win32 OIS_ReleaseDll"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\dll"
# PROP BASE Intermediate_Dir "..\dll\release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\dll"
# PROP Intermediate_Dir "..\dll\release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /I "..\includes" /Zi /W3 /O2 /Ob1 /Oy /D "WIN32" /D "NDEBUG" /D "OIS_NONCLIENT_BUILD" /D "OIS_DYNAMIC_LIB" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp"$(IntDir)/$(TargetName).pch" /c /GX 
# ADD CPP /nologo /MD /I "..\includes" /Zi /W3 /O2 /Ob1 /Oy /D "WIN32" /D "NDEBUG" /D "OIS_NONCLIENT_BUILD" /D "OIS_DYNAMIC_LIB" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp"$(IntDir)/$(TargetName).pch" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ELSEIF  "$(CFG)" == "OIS - Win32 OIS_DebugDll"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\dll"
# PROP BASE Intermediate_Dir "..\dll\debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\dll"
# PROP Intermediate_Dir "..\dll\debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /I "..\includes" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_STLP_DEBUG" /D "OIS_NONCLIENT_BUILD" /D "OIS_DYNAMIC_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /Gd /GZ /c /GX 
# ADD CPP /nologo /MDd /I "..\includes" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_STLP_DEBUG" /D "OIS_NONCLIENT_BUILD" /D "OIS_DYNAMIC_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /Gd /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ENDIF

# Begin Target

# Name "OIS - Win32 Debug"
# Name "OIS - Win32 Release"
# Name "OIS - Win32 OIS_ReleaseDll"
# Name "OIS - Win32 OIS_DebugDll"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm"
# Begin Source File

SOURCE=..\..\..\extern\ois\src\OISEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\OISForceFeedback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\OISInputManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\OISKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\OISObject.cpp
# End Source File
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\ois\src\win32\Win32ForceFeedback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\win32\Win32InputManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\win32\Win32JoyStick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\win32\Win32KeyBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\src\win32\Win32Mouse.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc"
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISException.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISInputManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISJoyStick.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISMouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISPrereqs.h
# End Source File
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32ForceFeedback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32InputManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32JoyStick.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32KeyBoard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32Mouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\win32\Win32Prereqs.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISEffect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISForceFeedback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\ois\includes\OISInterface.h
# End Source File
# End Group
# End Group
# End Target
# End Project

