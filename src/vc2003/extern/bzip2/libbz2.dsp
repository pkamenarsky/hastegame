# Microsoft Developer Studio Project File - Name="libbz2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libbz2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libbz2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libbz2.mak" CFG="libbz2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libbz2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libbz2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libbz2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" PRECOMP_VC7_TOBEREMOVED /Fp".\Debug/libbz2.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" PRECOMP_VC7_TOBEREMOVED /Fp".\Debug/libbz2.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\libbz2.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\libbz2.tlb" /win32 
# ADD BASE RSC /l 1041 /d "_DEBUG" 
# ADD RSC /l 1041 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ELSEIF  "$(CFG)" == "libbz2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp".\Release/libbz2.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /Fp".\Release/libbz2.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\libbz2.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\libbz2.tlb" /win32 
# ADD BASE RSC /l 1041 /d "NDEBUG" 
# ADD RSC /l 1041 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ENDIF

# Begin Target

# Name "libbz2 - Win32 Debug"
# Name "libbz2 - Win32 Release"
# Begin Source File

SOURCE=..\..\..\extern\bzip2\blocksort.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\bzlib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\bzlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\bzlib_private.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\crctable.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\decompress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\huffman.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\bzip2\randtable.c
# End Source File
# End Target
# End Project

