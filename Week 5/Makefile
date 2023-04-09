INC     = /I .\include
FLAGS   = /EHsc /nologo /diagnostics:caret /std:c++latest /utf-8 /permissive- /Zc:wchar_t $(INC)
DEV     = $(FLAGS) /ZI /fsanitize=address
OPT     = $(FLAGS) /Ox /O2 /Ot /favor:INTEL64
WARNING = /W2 /WL
DEFINES = /D _USE_MATH_DEFINES /D _UNICODE /D UNICODE
OPT_OBJ = .\obj\opt\DiskManager.obj \
		  .\obj\opt\Page.obj
DEV_OBJ = .\obj\debug\DiskManager.obj \
		  .\obj\debug\Page.obj

run: build
	.\bin\opt\main.exe

run_dev: build_dev
	.\bin\debug\main.exe

build: .\bin\opt\main.exe

build_dev: .\bin\debug\main.exe

check:
	CL .\src\*.cpp $(FLAGS) $(WARNING) $(DEFINES) /Zs

.\bin\opt\test.exe: .\include\*.hpp $(OPT_OBJ) .\obj\opt\test.obj
	CL $(OPT_OBJ) .\obj\opt\test.obj $(OPT) $(WARNING) $(DEFINES) /Fe.\bin\opt\test.exe

.\bin\debug\test.exe: .\include\*.hpp $(DEV_OBJ) .\obj\debug\test.obj
	CL $(DEV_OBJ) .\obj\debug\test.obj $(DEV) $(WARNING) $(DEFINES) /Fe.\bin\debug\test.exe

# {.\src\}.cpp{.\bin\opt\}.exe: .\include\*.hpp $(OPT_OBJ) .\obj\opt\$(*B).obj
# 	CL $< $(OPT_OBJ) .\obj\opt\$(*B).obj $(OPT) $(WARNING) $(DEFINES) /Fe$@
	
# {.\src\}.cpp{.\bin\debug\}.exe: .\include\*.hpp $(DEV_OBJ) .\obj\debug\$(*B).obj
# 	CL $< $(DEV_OBJ) .\obj\debug\$(*B).obj $(DEV) $(WARNING) $(DEFINES) /Fe$@
	
{.\src\}.cpp{.\obj\debug\}.obj:
	CL $< $(DEV) $(WARNING) $(DEFINES) /c /Fo$@

{.\src\}.cpp{.\obj\opt\}.obj:
	CL $< $(OPT) $(WARNING) $(DEFINES) /c /Fo$@

.cpp.exe:
	CL $< $(OPT) $(WARNING) $(DEFINES) /Fe$@
	DEL $*.obj

clean:
	-DEL .\bin\opt\ .\bin\debug\ .\obj\opt\ .\obj\debug\ *.pdb *.exe *.exp *.obj *.idb *.lib /Q /F
