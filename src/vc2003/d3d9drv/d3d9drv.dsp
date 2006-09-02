# Microsoft Developer Studio Project File - Name="d3d9drv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=d3d9drv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "d3d9drv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "d3d9drv.mak" CFG="d3d9drv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "d3d9drv - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "d3d9drv - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "d3d9drv - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "D3D_DEBUG_INFO" /D "D3D9DRV_USE_CG" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "D3D_DEBUG_INFO" /D "D3D9DRV_USE_CG" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion_d.lib d3d9.lib d3dx9.lib dxerr9.lib cg.lib cgD3D9.lib /nologo /dll /out:"..\..\..\bin\d3d9drv_d.dll" /incremental:yes /debug /pdb:"..\..\..\bin\d3d9drv_d.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/d3d9drv_d.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion_d.lib d3d9.lib d3dx9.lib dxerr9.lib cg.lib cgD3D9.lib /nologo /dll /out:"..\..\..\bin\d3d9drv_d.dll" /incremental:yes /debug /pdb:"..\..\..\bin\d3d9drv_d.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/d3d9drv_d.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "d3d9drv - Win32 Release"

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
# ADD BASE CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "D3D9DRV_USE_CG" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ION_EXPORTS" /D "D3D9DRV_USE_CG" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion.lib d3d9.lib d3dx9.lib dxerr9.lib cg.lib cgD3D9.lib /nologo /dll /out:"..\..\..\bin\d3d9drv.dll" /incremental:no /debug /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/d3d9drv.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../../bin/ion.lib d3d9.lib d3dx9.lib dxerr9.lib cg.lib cgD3D9.lib /nologo /dll /out:"..\..\..\bin\d3d9drv.dll" /incremental:no /debug /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/d3d9drv.lib" /machine:ix86 

!ENDIF

# Begin Target

# Name "d3d9drv - Win32 Debug"
# Name "d3d9drv - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\d3d9drv\cgd3d9fragmentprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\cgd3d9vertexprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9cubemap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9device.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9fragmentprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9indexstream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9texture2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9vertexprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9vertexstream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\tod3d9convs.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\d3d9drv\cgd3d9fragmentprogram.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\cgd3d9vertexprogram.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9cubemap.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9device.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9fragmentprogram.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9indexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9texture2d.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9vertexprogram.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\d3d9vertexstream.hh
# End Source File
# Begin Source File

SOURCE=..\..\d3d9drv\tod3d9convs.hh
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
# End Group
# End Target
# End Project

