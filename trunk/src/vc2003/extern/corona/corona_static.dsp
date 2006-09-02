# Microsoft Developer Studio Project File - Name="corona_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=corona_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "corona_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "corona_static.mak" CFG="corona_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "corona_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "corona_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "corona_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_static"
# PROP BASE Intermediate_Dir "Debug_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_static"
# PROP Intermediate_Dir "Debug_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I ""../../../extern/corona/src/libpng-1.2.1"" /I ""../../../extern/corona/src/jpeg-6b"" /I ""../../../extern/corona/src/libungif-4.1.0"" /I "../../../extern/zlib" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /GF /Gm /Gy PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /I ""../../../extern/corona/src/libpng-1.2.1"" /I ""../../../extern/corona/src/jpeg-6b"" /I ""../../../extern/corona/src/libungif-4.1.0"" /I "../../../extern/zlib" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /GF /Gm /Gy PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug_static\corona_static.lib" 
# ADD LIB32 /nologo /out:"Debug_static\corona_static.lib" 

!ELSEIF  "$(CFG)" == "corona_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_static"
# PROP BASE Intermediate_Dir "Release_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_static"
# PROP Intermediate_Dir "Release_static"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I ""../../../extern/corona/src/libpng-1.2.1"" /I ""../../../extern/corona/src/jpeg-6b"" /I ""../../../extern/corona/src/libungif-4.1.0"" /I "../../../extern/zlib" /Zi /W3 /O2 /Og /Ob1 /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /I ""../../../extern/corona/src/libpng-1.2.1"" /I ""../../../extern/corona/src/jpeg-6b"" /I ""../../../extern/corona/src/libungif-4.1.0"" /I "../../../extern/zlib" /Zi /W3 /O2 /Og /Ob1 /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF /Gy PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release_static\corona_static.lib" 
# ADD LIB32 /nologo /out:"Release_static\corona_static.lib" 

!ENDIF

# Begin Target

# Name "corona_static - Win32 Debug"
# Name "corona_static - Win32 Release"
# Begin Group "files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Convert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Corona.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\corona.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\DefaultFileSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\MemoryFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\MemoryFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Open.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenBMP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenGIF.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenJPEG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenPCX.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenPNG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\OpenTGA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Save.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\SavePNG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\SaveTGA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\SimpleImage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\Utility.h
# End Source File
# End Group
# Begin Group "libpng-1.2.1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\png.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\png.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngget.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngset.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngtest.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libpng-1.2.1\pngwutil.c
# End Source File
# End Group
# Begin Group "jpeg-6b"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdct.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jerror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jerror.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jidctred.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jinclude.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jpeglib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jutils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\jpeg-6b\jversion.h
# End Source File
# End Group
# Begin Group "libungif-4.1.0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libungif-4.1.0\dgif_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libungif-4.1.0\gif_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libungif-4.1.0\gif_lib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libungif-4.1.0\gif_lib_private.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extern\corona\src\libungif-4.1.0\gifalloc.c
# End Source File
# End Group
# End Target
# End Project

