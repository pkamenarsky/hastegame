# Microsoft Developer Studio Project File - Name="haste" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=haste - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "haste.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "haste.mak" CFG="haste - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "haste - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "haste - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "haste - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../bin/ion.lib ../extern/newton/newton.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "haste - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../../bin/ion_d.lib ../extern/newton/newton.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/haste_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "haste - Win32 Release"
# Name "haste - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\eexport.cpp
# End Source File
# Begin Source File

SOURCE=.\g_audio.cpp
# End Source File
# Begin Source File

SOURCE=.\g_entity.cpp
# End Source File
# Begin Source File

SOURCE=.\g_game.cpp
# End Source File
# Begin Source File

SOURCE=.\g_physics.cpp
# End Source File
# Begin Source File

SOURCE=.\g_ui.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\mycode.cpp
# End Source File
# Begin Source File

SOURCE=.\r_hud.cpp
# End Source File
# Begin Source File

SOURCE=.\trace.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\cvars.hh
# End Source File
# Begin Source File

SOURCE=.\eexport.h
# End Source File
# Begin Source File

SOURCE=.\ent_api.h
# End Source File
# Begin Source File

SOURCE=.\g_audio.h
# End Source File
# Begin Source File

SOURCE=.\g_entity.h
# End Source File
# Begin Source File

SOURCE=.\g_game.h
# End Source File
# Begin Source File

SOURCE=.\g_physics.h
# End Source File
# Begin Source File

SOURCE=.\g_ui.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\input.h
# End Source File
# Begin Source File

SOURCE=.\r_hud.h
# End Source File
# Begin Source File

SOURCE=.\trace.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "EntLib"

# PROP Default_Filter "*.c;*.h;*.cpp"
# Begin Source File

SOURCE=.\EntLib\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\EntLib\entity.h
# End Source File
# End Group
# Begin Group "entities"

# PROP Default_Filter "*c;*.h;+.cpp"
# Begin Source File

SOURCE=.\entities\ent_func_fovwarp.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_func_fovwarp.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_grenade.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_grenade.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_haste.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_haste.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_info_player_deathmatch.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_info_player_deathmatch.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_light.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_light.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_phys_model.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_phys_model.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_player.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_player.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_rocket.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_rocket.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_exit.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_exit.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_push.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_push.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_restart.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_restart.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_timer.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_trigger_timer.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_weapon_grenadelauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_weapon_grenadelauncher.h
# End Source File
# Begin Source File

SOURCE=.\entities\ent_weapon_rocketlauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\entities\ent_weapon_rocketlauncher.h
# End Source File
# End Group
# Begin Group "ION"

# PROP Default_Filter "*.cpp; *.hh; *.h"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\skelapp.cpp
# End Source File
# Begin Source File

SOURCE=.\skelapp.hh
# End Source File
# End Group
# Begin Source File

SOURCE=.\codelog.txt
# End Source File
# Begin Source File

SOURCE=..\..\bin\basehaste\ui\test.gui
# End Source File
# End Target
# End Project
