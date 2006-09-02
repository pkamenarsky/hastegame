# Microsoft Developer Studio Project File - Name="glew_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=glew_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "glew_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "glew_static.mak" CFG="glew_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "glew_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "glew_static - Win32 Debug MX" (based on "Win32 (x86) Static Library")
!MESSAGE "glew_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "glew_static - Win32 Release MX" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "glew_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "./static/debug"
# PROP BASE Intermediate_Dir "./static/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./static/debug"
# PROP Intermediate_Dir "./static/debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "../../../extern/glew/include" /Zi /W3 /Od /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_STATIC" /D "_MBCS" /YX /Fp".\static/debug/glew_static.pch" /Fo".\static/debug/" /Fd".\static/debug/" /GZ /c /GX 
# ADD CPP /nologo /MTd /I "../../../extern/glew/include" /Zi /W3 /Od /D "WIN32" /D "_WIN32" /D "_DEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_STATIC" /D "_MBCS" /YX /Fp".\static/debug/glew_static.pch" /Fo".\static/debug/" /Fd".\static/debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" /d "GLEW_STATIC" 
# ADD RSC /l 1033 /d "_DEBUG" /d "GLEW_STATIC" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\static\debug\glew32sd.lib" 
# ADD LIB32 /nologo /out:".\static\debug\glew32sd.lib" 

!ELSEIF  "$(CFG)" == "glew_static - Win32 Debug MX"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\../../lib"
# PROP BASE Intermediate_Dir ".\static/debug-mx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\../../lib"
# PROP Intermediate_Dir ".\static/debug-mx"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "../../include" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_MX" /D "GLEW_STATIC" /D "_MBCS" /YX /Fp".\static/debug-mx/glew_static.pch" /Fo".\static/debug-mx/" /Fd".\static/debug-mx/" /GZ /c /GX 
# ADD CPP /nologo /MTd /I "../../include" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_MX" /D "GLEW_STATIC" /D "_MBCS" /YX /Fp".\static/debug-mx/glew_static.pch" /Fo".\static/debug-mx/" /Fd".\static/debug-mx/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" /d "GLEW_MX" /d "GLEW_STATIC" 
# ADD RSC /l 1033 /d "_DEBUG" /d "GLEW_MX" /d "GLEW_STATIC" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\glew32mxsd.lib" 
# ADD LIB32 /nologo /out:"..\..\lib\glew32mxsd.lib" 

!ELSEIF  "$(CFG)" == "glew_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "./static/release"
# PROP BASE Intermediate_Dir "./static/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./static/release"
# PROP Intermediate_Dir "./static/release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "../../../extern/glew/include" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_STATIC" /D "_MBCS" /GF /Gy /YX /Fp".\static/release/glew_static.pch" /Fo".\static/release/" /Fd".\static/release/" /c /GX 
# ADD CPP /nologo /MT /I "../../../extern/glew/include" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_STATIC" /D "_MBCS" /GF /Gy /YX /Fp".\static/release/glew_static.pch" /Fo".\static/release/" /Fd".\static/release/" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" /d "GLEW_STATIC" 
# ADD RSC /l 1033 /d "NDEBUG" /d "GLEW_STATIC" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\static\release\glew32s.lib" 
# ADD LIB32 /nologo /out:".\static\release\glew32s.lib" 

!ELSEIF  "$(CFG)" == "glew_static - Win32 Release MX"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../../lib"
# PROP BASE Intermediate_Dir ".\static/release-mx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../../lib"
# PROP Intermediate_Dir ".\static/release-mx"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "../../include" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_MX" /D "GLEW_STATIC" /D "_MBCS" /GF /Gy /YX /Fp".\static/release-mx/glew_static.pch" /Fo".\static/release-mx/" /Fd".\static/release-mx/" /c /GX 
# ADD CPP /nologo /MT /I "../../include" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRALEAN" /D "GLEW_MX" /D "GLEW_STATIC" /D "_MBCS" /GF /Gy /YX /Fp".\static/release-mx/glew_static.pch" /Fo".\static/release-mx/" /Fd".\static/release-mx/" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" /d "GLEW_MX" /d "GLEW_STATIC" 
# ADD RSC /l 1033 /d "NDEBUG" /d "GLEW_MX" /d "GLEW_STATIC" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\glew32mxs.lib" 
# ADD LIB32 /nologo /out:"..\..\lib\glew32mxs.lib" 

!ENDIF

# Begin Target

# Name "glew_static - Win32 Debug"
# Name "glew_static - Win32 Debug MX"
# Name "glew_static - Win32 Release"
# Name "glew_static - Win32 Release MX"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extern\glew\src\glew.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extern\glew\include\Gl\glew.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\glew\include\Gl\wglew.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# End Group
# End Target
# End Project

