# Microsoft Developer Studio Generated NMAKE File, Based on gpath.dsp
!IF "$(CFG)" == ""
CFG=gpath - Win32 Debug
!MESSAGE No configuration specified. Defaulting to gpath - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "gpath - Win32 Release" && "$(CFG)" != "gpath - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gpath.mak" CFG="gpath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gpath - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gpath - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gpath - Win32 Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\gpath.exe"


CLEAN :
	-@erase "$(INTDIR)\gpath.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\gpath.exe"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\gpath.pch" /YX /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gpath.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  libogdl.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\gpath.pdb" /machine:I386 /out:"$(OUTDIR)\gpath.exe" 
LINK32_OBJS= \
	"$(INTDIR)\gpath.obj"

"$(OUTDIR)\gpath.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "gpath - Win32 Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\gpath.exe"


CLEAN :
	-@erase "$(INTDIR)\gpath.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gpath.exe"
	-@erase "$(OUTDIR)\gpath.ilk"
	-@erase "$(OUTDIR)\gpath.pdb"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\gpath.pch" /YX /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gpath.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  libogdl.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\gpath.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gpath.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\gpath.obj"

"$(OUTDIR)\gpath.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("gpath.dep")
!INCLUDE "gpath.dep"
!ELSE 
!MESSAGE Warning: cannot find "gpath.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "gpath - Win32 Release" || "$(CFG)" == "gpath - Win32 Debug"
SOURCE=.\gpath.c

"$(INTDIR)\gpath.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

