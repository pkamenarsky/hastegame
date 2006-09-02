# Microsoft Developer Studio Project File - Name="ion" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ion - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "ion.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "ion.mak" CFG="ion - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "ion - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ion - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ion - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../../bin"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../extern/bzip2" /I "../../extern/zlib" /I "../../extern/tinyxml" /I "../../extern/corona/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "TIXML_USE_STL" /D "_MBCS" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../extern/bzip2" /I "../../extern/zlib" /I "../../extern/tinyxml" /I "../../extern/corona/src" /I "../../../../freetype-2.1.10\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "TIXML_USE_STL" /D "_MBCS" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../extern/bzip2/Debug/libbz2.lib ../extern/zlib/Debug/zlib.lib ../extern/physfs/physfs_static_debug/physfs_static.lib ../extern/tinyxml/Debug_STL/tinyxmld_STL.lib ../extern/corona/Debug_static/corona_static.lib freetype2110MT_D.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"..\..\..\bin\ion_d.dll" /implib:"$(OutDir)/ion_d.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../extern/bzip2/Debug/libbz2.lib ../extern/zlib/Debug/zlib.lib ../extern/physfs/physfs_static_debug/physfs_static.lib ../extern/tinyxml/Debug_STL/tinyxmld_STL.lib ../extern/corona/Debug_static/corona_static.lib freetype2110MT_D.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"..\..\..\bin\ion_d.dll" /implib:"$(OutDir)/ion_d.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ion - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../../bin"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /I "../../extern/bzip2" /I "../../extern/zlib" /I "../../extern/tinyxml" /I "../../extern/corona/src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "TIXML_USE_STL" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MT /W3 /GX /Zi /I "../../extern/bzip2" /I "../../extern/zlib" /I "../../extern/tinyxml" /I "../../extern/corona/src" /I "../../../../freetype-2.1.10\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "TIXML_USE_STL" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../extern/bzip2/Release/libbz2.lib ../extern/zlib/Release/zlib.lib ../extern/physfs/physfs_static_release/physfs_static.lib ../extern/tinyxml/Release_STL/tinyxml_STL.lib ../extern/corona/Release_static/corona_static.lib freetype2110MT.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /implib:"$(OutDir)/ion.lib" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../extern/bzip2/Release/libbz2.lib ../extern/zlib/Release/zlib.lib ../extern/physfs/physfs_static_release/physfs_static.lib ../extern/tinyxml/Release_STL/tinyxml_STL.lib ../../extern/corona/Release_static/corona_static.lib ../../../../freetype-2.1.10/objs/freetype2110MT.lib /nologo /subsystem:windows /dll /machine:IX86 /implib:"$(OutDir)/ion.lib" /pdbtype:sept /opt:ref /opt:icf
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "ion - Win32 Debug"
# Name "ion - Win32 Release"
# Begin Group "audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\audio\audioserver.cpp
DEP_CPP_AUDIO=\
	"..\..\audio\audioserver.hh"\
	"..\..\audio\sound.hh"\
	"..\..\audio\voice.hh"\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\audio\audioserver.hh
# End Source File
# Begin Source File

SOURCE=..\..\audio\sound.cpp
DEP_CPP_SOUND=\
	"..\..\audio\sound.hh"\
	"..\..\audio\voice.hh"\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\audio\sound.hh
# End Source File
# Begin Source File

SOURCE=..\..\audio\voice.cpp
DEP_CPP_VOICE=\
	"..\..\audio\sound.hh"\
	"..\..\audio\voice.hh"\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\audio\voice.hh
# End Source File
# End Group
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\base\base_all.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\basememobj.cpp
DEP_CPP_BASEM=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\basememobj.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\baserefcounted.cpp
DEP_CPP_BASER=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\dll.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\baserefcounted.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\bz2pkhandler.cpp
DEP_CPP_BZ2PK=\
	"..\..\base\bz2pkhandler.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\pkdatablock.hh"\
	"..\..\base\streamable.hh"\
	"..\..\extern\bzip2\bzlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\bz2pkhandler.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\datablock.cpp
DEP_CPP_DATAB=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\datablock.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\dll.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\dynobject.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\dynobject_win32.cpp
DEP_CPP_DYNOB=\
	"..\..\base\dll.hh"\
	"..\..\base\dynobject.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\fpscalc.cpp
DEP_CPP_FPSCA=\
	"..\..\base\dll.hh"\
	"..\..\base\fpscalc.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\timer.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\fpscalc.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\ignorecase.c
DEP_CPP_IGNOR=\
	"..\..\base\ignorecase.h"\
	"..\..\extern\physfs\physfs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\ignorecase.h
# End Source File
# Begin Source File

SOURCE=..\..\base\iontypes.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\localfile.cpp
DEP_CPP_LOCAL=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localfile.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\localfile.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\localstreamable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\base\localstreamable.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\log.cpp
DEP_CPP_LOG_C=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\nullstream.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\log.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\managedmemobj.cpp
DEP_CPP_MANAG=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\managedmemobj.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\nullstream.cpp
DEP_CPP_NULLS=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\nullstream.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\nullstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\numericarrays.cpp
DEP_CPP_NUMER=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\numericarrays.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\physfsfile.cpp
DEP_CPP_PHYSF=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\ignorecase.h"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\physfsfile.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\physfs\globbing.h"\
	"..\..\extern\physfs\physfs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\physfsfile.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\pkdatablock.cpp
DEP_CPP_PKDAT=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\pkdatablock.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\pkdatablock.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\refcounted.cpp
DEP_CPP_REFCO=\
	"..\..\base\dll.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\refcounted.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\refcounted.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\refcountedsource.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\refptr.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\stdoutstream.cpp
DEP_CPP_STDOU=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\stdoutstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\stlallocator.cpp
DEP_CPP_STLAL=\
	"..\..\base\stlallocator.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\stlallocator.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\streamable.cpp
DEP_CPP_STREA=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\streamable.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\string.cpp
DEP_CPP_STRIN=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\string.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\string.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\stringlist.cpp
DEP_CPP_STRING=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\stringlist.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\timer.cpp
DEP_CPP_TIMER=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\timer.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\timer.hh
# End Source File
# Begin Source File

SOURCE=..\..\base\zpkhandler.cpp
DEP_CPP_ZPKHA=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\pkdatablock.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\zpkhandler.hh"\
	"..\..\extern\zlib\zconf.h"\
	"..\..\extern\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\base\zpkhandler.hh
# End Source File
# End Group
# Begin Group "input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\input\input_all.hh
# End Source File
# Begin Source File

SOURCE=..\..\input\inputdevice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\input\inputdevice.hh
# End Source File
# Begin Source File

SOURCE=..\..\input\inputserver.cpp
DEP_CPP_INPUT=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\string.hh"\
	"..\..\input\inputdevice.hh"\
	"..\..\input\inputserver.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\input\inputserver.hh
# End Source File
# Begin Source File

SOURCE=..\..\input\keyboardsym.hh
# End Source File
# Begin Source File

SOURCE=..\..\input\mousestates.hh
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\math\aabbvolume.cpp
DEP_CPP_AABBV=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\aabbvolume.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\arcball.cpp
DEP_CPP_ARCBA=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\arcball.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\arcball.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\filterbase.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\frustum.cpp
DEP_CPP_FRUST=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\frustum.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\interpolator.cpp
DEP_CPP_INTER=\
	"..\..\base\dll.hh"\
	"..\..\math\interpolator.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\interpolator.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\lerp.cpp
DEP_CPP_LERP_=\
	"..\..\base\dll.hh"\
	"..\..\math\interpolator.hh"\
	"..\..\math\lerp.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\lerp.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\math_all.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\matrix.cpp
DEP_CPP_MATRI=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\matrix.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\meshconv.cpp
DEP_CPP_MESHC=\
	"..\..\base\dll.hh"\
	"..\..\math\meshconv.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\meshconv.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\obbvolume.cpp
DEP_CPP_OBBVO=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\obbvolume.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\obbvolume.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\plane.cpp
DEP_CPP_PLANE=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\plane.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\quadraticbezier.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\quaternion.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\rectangle.cpp
DEP_CPP_RECTA=\
	"..\..\base\dll.hh"\
	"..\..\math\rectangle.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\rectangle.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\spherevolume.cpp
DEP_CPP_SPHER=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\spherevolume.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\transform.cpp
DEP_CPP_TRANS=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\transform.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\vectors.cpp
DEP_CPP_VECTO=\
	"..\..\base\dll.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\vectors.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\math\vectors.hh
# End Source File
# Begin Source File

SOURCE=..\..\math\volume.hh
# End Source File
# End Group
# Begin Group "scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\scene\camera.cpp
DEP_CPP_CAMER=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\node.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\camera.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\depthprepassprograms.cpp
DEP_CPP_DEPTH=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\string.hh"\
	"..\..\scene\depthprepassprograms.h"\
	"..\..\video\programarray.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\depthprepassprograms.h
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3programs.cpp
DEP_CPP_DOT3P=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\string.hh"\
	"..\..\scene\dot3programs.h"\
	"..\..\video\programarray.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3programs.h
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3renderer.cpp
DEP_CPP_DOT3R=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\depthprepassprograms.h"\
	"..\..\scene\dot3programs.h"\
	"..\..\scene\dot3renderer.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\pplprograms.h"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjobqueue.hh"\
	"..\..\scene\renderquery.hh"\
	"..\..\scene\shader.hh"\
	"..\..\scene\space.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3renderer.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3shader.cpp
DEP_CPP_DOT3S=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\dot3shader.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\dot3shader.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\ffprenderer.cpp
DEP_CPP_FFPRE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\ffprenderer.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjobqueue.hh"\
	"..\..\scene\renderquery.hh"\
	"..\..\scene\shader.hh"\
	"..\..\scene\space.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\ffprenderer.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\group.cpp
DEP_CPP_GROUP=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\group.hh"\
	"..\..\scene\node.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\group.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\indexlist.cpp
DEP_CPP_INDEX=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\string.hh"\
	"..\..\scene\indexlist.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\indexlist.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\iscloader.cpp
DEP_CPP_ISCLO=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refcountedsource.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\group.hh"\
	"..\..\scene\iscloader.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderablemesh.hh"\
	"..\..\scene\renderablemeshnode.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\mesh.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\iscloader.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\light.cpp
DEP_CPP_LIGHT=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\video\coreenums.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\light.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\lightlist.cpp
DEP_CPP_LIGHTL=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\video\coreenums.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\lightlist.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\node.cpp
DEP_CPP_NODE_=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\node.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\node.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\nodelist.cpp
DEP_CPP_NODEL=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\nodelist.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\nodelist.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\pplprograms.cpp
DEP_CPP_PPLPR=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\string.hh"\
	"..\..\scene\pplprograms.h"\
	"..\..\video\programarray.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\pplprograms.h
# End Source File
# Begin Source File

SOURCE=..\..\scene\q3bsp.cpp
DEP_CPP_Q3BSP=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\physfsfile.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\physfs\physfs.h"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quadraticbezier.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\q3bsp.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjobqueue.hh"\
	"..\..\scene\renderquery.hh"\
	"..\..\scene\space.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\picloader.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\q3bsp.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\q3shaders.cpp
DEP_CPP_Q3SHA=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\physfsfile.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\physfs\physfs.h"\
	"..\..\scene\q3shaders.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\coreenums.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\q3shaders.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderable.cpp
DEP_CPP_RENDE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderable.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablelist.cpp
DEP_CPP_RENDER=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablelist.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablemesh.cpp
DEP_CPP_RENDERA=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablemesh.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\mesh.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablemesh.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablemeshnode.cpp
DEP_CPP_RENDERAB=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablemesh.hh"\
	"..\..\scene\renderablemeshnode.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjobqueue.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\mesh.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderablemeshnode.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderer.cpp
DEP_CPP_RENDERE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderer.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderer.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjob.cpp
DEP_CPP_RENDERJ=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjob.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjoblist.cpp
DEP_CPP_RENDERJO=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjoblist.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjoblist.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjobqueue.cpp
DEP_CPP_RENDERJOB=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderjob.hh"\
	"..\..\scene\renderjobqueue.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderjobqueue.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderquery.cpp
DEP_CPP_RENDERQ=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderquery.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\renderquery.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\scene_all.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\shader.cpp
DEP_CPP_SHADE=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\scene\shader.hh"\
	"..\..\video\coreenums.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\shader.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\simplespace.cpp
DEP_CPP_SIMPL=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\frustum.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\camera.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderable.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\renderquery.hh"\
	"..\..\scene\simplespace.hh"\
	"..\..\scene\space.hh"\
	"..\..\video\coreenums.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\simplespace.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\space.hh
# End Source File
# Begin Source File

SOURCE=..\..\scene\xmlshader.cpp
DEP_CPP_XMLSH=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\tinyxml\tinystr.h"\
	"..\..\extern\tinyxml\tinyxml.h"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\scene\indexlist.hh"\
	"..\..\scene\light.hh"\
	"..\..\scene\lightlist.hh"\
	"..\..\scene\node.hh"\
	"..\..\scene\renderablelist.hh"\
	"..\..\scene\shader.hh"\
	"..\..\scene\xmlshader.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\stateblock.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\scene\xmlshader.hh
# End Source File
# End Group
# Begin Group "video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\video\attributebuffer.cpp
DEP_CPP_ATTRI=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\attributebuffer.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\attributetable.cpp
DEP_CPP_ATTRIB=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\attributetable.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\caps.cpp
DEP_CPP_CAPS_=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\pixelformat.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\caps.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\channeldatatype.cpp
DEP_CPP_CHANN=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\video\channeldatatype.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\channeldatatype.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\color.cpp
DEP_CPP_COLOR=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\video\color.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\color.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\coreenums.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\coronafile.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\cubemap.cpp
DEP_CPP_CUBEM=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\cubemap.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\draw2dcache.cpp
DEP_CPP_DRAW2=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\color.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\draw2dcache.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vcache.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\draw2dcache.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\font.cpp
DEP_CPP_FONT_=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localfile.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\rectangle.hh"\
	"..\..\math\transform.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\color.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\draw2dcache.hh"\
	"..\..\video\font.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\picsaver.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\font.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\geometrystream.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\indexformat.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\indexiterator.cpp
DEP_CPP_INDEXI=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\indexiterator.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\indexstream.cpp
DEP_CPP_INDEXS=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\indexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\loadobj.cpp
DEP_CPP_LOADO=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\meshconv.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\loadobj.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\mesh.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\loadobj.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\memindexstream.cpp
DEP_CPP_MEMIN=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\memindexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\mempool.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\memvertexstream.cpp
DEP_CPP_MEMVE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\memvertexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\mesh.cpp
DEP_CPP_MESH_=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\aabbvolume.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\plane.hh"\
	"..\..\math\spherevolume.hh"\
	"..\..\math\vectors.hh"\
	"..\..\math\volume.hh"\
	"..\..\video\attributebuffer.hh"\
	"..\..\video\attributetable.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\memindexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\memvertexstream.hh"\
	"..\..\video\mesh.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\mesh.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\occlusionquery.cpp
# End Source File
# Begin Source File

SOURCE=..\..\video\occlusionquery.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\picbuffer.cpp
DEP_CPP_PICBU=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localfile.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\picbuffer.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\picloader.cpp
DEP_CPP_PICLO=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\corona\src\corona.h"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\coronafile.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\picloader.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\picloader.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\picsaver.cpp
DEP_CPP_PICSA=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\log.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\stdoutstream.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\extern\corona\src\corona.h"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\coronafile.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\picsaver.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\picsaver.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\pixelformat.cpp
DEP_CPP_PIXEL=\
	"..\..\base\dll.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\pixelformat.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\pixelformat.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\program.cpp
DEP_CPP_PROGR=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\program.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\programarray.cpp
DEP_CPP_PROGRA=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\programarray.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\programarray.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\propertytable.cpp
DEP_CPP_PROPE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\baserefcounted.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\quaternion.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\propertytable.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\propertytable.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\simplepicbuffer.cpp
DEP_CPP_SIMPLE=\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\simplepicbuffer.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\simplepicbuffer.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\stateblock.cpp
DEP_CPP_STATE=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\stateblock.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\stateblock.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\texture.cpp
DEP_CPP_TEXTU=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\string.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\texture.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\texture.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\texture2d.cpp
DEP_CPP_TEXTUR=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\simplepicbuffer.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\texture2d.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\vcache.cpp
DEP_CPP_VCACH=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vcache.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\vcache.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexformat.cpp
DEP_CPP_VERTE=\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\streamable.hh"\
	"..\..\video\vertexformat.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexformat.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexiterator.cpp
DEP_CPP_VERTEX=\
	"..\..\base\basememobj.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexiterator.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexstream.cpp
DEP_CPP_VERTEXS=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\streamable.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\vertexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\video_all.hh
# End Source File
# Begin Source File

SOURCE=..\..\video\videodevice.cpp
DEP_CPP_VIDEO=\
	"..\..\base\basememobj.hh"\
	"..\..\base\datablock.hh"\
	"..\..\base\dll.hh"\
	"..\..\base\iontypes.hh"\
	"..\..\base\localstreamable.hh"\
	"..\..\base\managedmemobj.hh"\
	"..\..\base\numericarrays.hh"\
	"..\..\base\refcounted.hh"\
	"..\..\base\refptr.hh"\
	"..\..\base\streamable.hh"\
	"..\..\base\string.hh"\
	"..\..\base\stringlist.hh"\
	"..\..\math\matrix.hh"\
	"..\..\math\vectors.hh"\
	"..\..\video\caps.hh"\
	"..\..\video\channeldatatype.hh"\
	"..\..\video\coreenums.hh"\
	"..\..\video\cubemap.hh"\
	"..\..\video\geometrystream.hh"\
	"..\..\video\indexformat.hh"\
	"..\..\video\indexiterator.hh"\
	"..\..\video\indexstream.hh"\
	"..\..\video\mempool.hh"\
	"..\..\video\occlusionquery.hh"\
	"..\..\video\picbuffer.hh"\
	"..\..\video\pixelformat.hh"\
	"..\..\video\program.hh"\
	"..\..\video\texture.hh"\
	"..\..\video\texture2d.hh"\
	"..\..\video\vertexformat.hh"\
	"..\..\video\vertexiterator.hh"\
	"..\..\video\vertexstream.hh"\
	"..\..\video\videodevice.hh"\
	
# End Source File
# Begin Source File

SOURCE=..\..\video\videodevice.hh
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\video\dllmain.cpp

!IF  "$(CFG)" == "ion - Win32 Debug"

# PROP Intermediate_Dir "$(IntDir)/$(InputName)3.obj"
# ADD CPP /nologo /GX /Fo"$(IntDir)/$(InputName)3.obj" /GZ

!ELSEIF  "$(CFG)" == "ion - Win32 Release"

# PROP Intermediate_Dir "$(IntDir)/$(InputName)3.obj"
# ADD CPP /nologo /GX /Fo"$(IntDir)/$(InputName)3.obj"

!ENDIF 

# End Source File
# End Target
# End Project
