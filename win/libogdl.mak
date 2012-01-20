# Microsoft Developer Studio Generated NMAKE File, Based on libogdl.dsp
!IF "$(CFG)" == ""
CFG=libogdl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libogdl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libogdl - Win32 Release" && "$(CFG)" != "libogdl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libogdl.mak" CFG="libogdl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libogdl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libogdl - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "libogdl - Win32 Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\libogdl.lib"


CLEAN :
	-@erase "$(INTDIR)\graph.obj"
	-@erase "$(INTDIR)\ogdllog.obj"
	-@erase "$(INTDIR)\ogdlparser.obj"
	-@erase "$(INTDIR)\path.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libogdl.lib"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libogdl.pch" /YX /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libogdl.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libogdl.lib" 
LIB32_OBJS= \
	"$(INTDIR)\path.obj" \
	"$(INTDIR)\ogdllog.obj" \
	"$(INTDIR)\ogdlparser.obj" \
	"$(INTDIR)\graph.obj"

"$(OUTDIR)\libogdl.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libogdl - Win32 Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\libogdl.lib"


CLEAN :
	-@erase "$(INTDIR)\graph.obj"
	-@erase "$(INTDIR)\ogdllog.obj"
	-@erase "$(INTDIR)\ogdlparser.obj"
	-@erase "$(INTDIR)\path.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libogdl.lib"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libogdl.pch" /YX /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libogdl.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libogdl.lib" 
LIB32_OBJS= \
	"$(INTDIR)\path.obj" \
	"$(INTDIR)\ogdllog.obj" \
	"$(INTDIR)\ogdlparser.obj" \
	"$(INTDIR)\graph.obj"

"$(OUTDIR)\libogdl.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("libogdl.dep")
!INCLUDE "libogdl.dep"
!ELSE 
!MESSAGE Warning: cannot find "libogdl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libogdl - Win32 Release" || "$(CFG)" == "libogdl - Win32 Debug"
SOURCE=.\graph.c

"$(INTDIR)\graph.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ogdllog.c

"$(INTDIR)\ogdllog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ogdlparser.c

"$(INTDIR)\ogdlparser.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\path.c

"$(INTDIR)\path.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

