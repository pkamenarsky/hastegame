# Microsoft Developer Studio Project File - Name="physfs_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=physfs_static - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "physfs_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "physfs_static.mak" CFG="physfs_static - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "physfs_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "physfs_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "physfs_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\physfs_static_release"
# PROP BASE Intermediate_Dir ".\physfs_static_release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\physfs_static_release"
# PROP Intermediate_Dir ".\physfs_static_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "../../../extern/physfs" /W3 /O2 /Ob1 /G6 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "PHYSFS_SUPPORTS_QPAK=1" /D "PHYSFS_SUPPORTS_ZIP=1" /D "PHYSFS_SUPPORTS_HOG=1" /D "PHYSFS_SUPPORTS_GRP=1" /D "PHYSFS_SUPPORTS_WAD=1" /D "PHYSFS_SUPPORTS_MVL=1" /D "_MBCS" /GF /Gy /YX /Fp".\physfs_static_release/physfs_static.pch" /Fo".\physfs_static_release/" /Fd".\physfs_static_release/" /c /GX 
# ADD CPP /nologo /MT /I "../../../extern/physfs" /W3 /O2 /Ob1 /G6 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "PHYSFS_SUPPORTS_QPAK=1" /D "PHYSFS_SUPPORTS_ZIP=1" /D "PHYSFS_SUPPORTS_HOG=1" /D "PHYSFS_SUPPORTS_GRP=1" /D "PHYSFS_SUPPORTS_WAD=1" /D "PHYSFS_SUPPORTS_MVL=1" /D "_MBCS" /GF /Gy /YX /Fp".\physfs_static_release/physfs_static.pch" /Fo".\physfs_static_release/" /Fd".\physfs_static_release/" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\physfs_static_release\physfs_static.lib" 
# ADD LIB32 /nologo /out:".\physfs_static_release\physfs_static.lib" 

!ELSEIF  "$(CFG)" == "physfs_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\physfs_static_debug"
# PROP BASE Intermediate_Dir ".\physfs_static_debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\physfs_static_debug"
# PROP Intermediate_Dir ".\physfs_static_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "../../../extern/physfs" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_LIB" /D "PHYSFS_SUPPORTS_QPAK=1" /D "PHYSFS_SUPPORTS_ZIP=1" /D "PHYSFS_SUPPORTS_HOG=1" /D "PHYSFS_SUPPORTS_GRP=1" /D "PHYSFS_SUPPORTS_WAD=1" /D "PHYSFS_SUPPORTS_MVL=1" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /Fp".\physfs_static_debug/physfs_static.pch" /Fo".\physfs_static_debug/" /Fd".\physfs_static_debug/" /FR /TC /GZ /c /GX 
# ADD CPP /nologo /MTd /I "../../../extern/physfs" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_LIB" /D "PHYSFS_SUPPORTS_QPAK=1" /D "PHYSFS_SUPPORTS_ZIP=1" /D "PHYSFS_SUPPORTS_HOG=1" /D "PHYSFS_SUPPORTS_GRP=1" /D "PHYSFS_SUPPORTS_WAD=1" /D "PHYSFS_SUPPORTS_MVL=1" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /Fp".\physfs_static_debug/physfs_static.pch" /Fo".\physfs_static_debug/" /Fd".\physfs_static_debug/" /FR /TC /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\physfs_static_debug\physfs_static.lib" 
# ADD LIB32 /nologo /out:".\physfs_static_debug\physfs_static.lib" 

!ENDIF

# Begin Target

# Name "physfs_static - Win32 Release"
# Name "physfs_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\dir.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\globbing.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\grp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\hog.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\mvl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\physfs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\physfs_byteorder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\qpak.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\wad.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\platform\win32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\archivers\zip.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extern\physfs\acconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\globbing.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\physfs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\physfs\physfs_internal.h
# End Source File
# End Group
# End Target
# End Project

