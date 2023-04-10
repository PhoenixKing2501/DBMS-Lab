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

run_join: .\bin\opt\join.exe
	.\bin\opt\join.exe 2> .\logs\join.log

run_join_dev: .\bin\debug\join.exe
	.\bin\debug\join.exe 2> .\logs\join.log

run_select: .\bin\opt\select.exe
	.\bin\opt\select.exe 2> .\logs\select.log

run_select_dev: .\bin\debug\select.exe
	.\bin\debug\select.exe 2> .\logs\select.log

.\bin\opt\test.exe: .\include\*.hpp $(OPT_OBJ) .\obj\opt\test.obj
	CL $(OPT_OBJ) .\obj\opt\test.obj $(OPT) $(WARNING) $(DEFINES) /Fe.\bin\opt\test.exe

.\bin\debug\test.exe: .\include\*.hpp $(DEV_OBJ) .\obj\debug\test.obj
	CL $(DEV_OBJ) .\obj\debug\test.obj $(DEV) $(WARNING) $(DEFINES) /Fe.\bin\debug\test.exe

.\bin\opt\select.exe: .\include\*.hpp $(OPT_OBJ) .\obj\opt\select.obj
	CL $(OPT_OBJ) .\obj\opt\select.obj $(OPT) $(WARNING) $(DEFINES) /Fe.\bin\opt\select.exe

.\bin\debug\select.exe: .\include\*.hpp $(DEV_OBJ) .\obj\debug\select.obj
	CL $(DEV_OBJ) .\obj\debug\select.obj $(DEV) $(WARNING) $(DEFINES) /Fe.\bin\debug\select.exe

.\bin\opt\join.exe: .\include\*.hpp $(OPT_OBJ) .\obj\opt\join.obj
	CL $(OPT_OBJ) .\obj\opt\join.obj $(OPT) $(WARNING) $(DEFINES) /Fe.\bin\opt\join.exe

.\bin\debug\join.exe: .\include\*.hpp $(DEV_OBJ) .\obj\debug\join.obj
	CL $(DEV_OBJ) .\obj\debug\join.obj $(DEV) $(WARNING) $(DEFINES) /Fe.\bin\debug\join.exe
	
{.\src\}.cpp{.\obj\debug\}.obj:
	CL $< $(DEV) $(WARNING) $(DEFINES) /c /Fo$@

{.\src\}.cpp{.\obj\opt\}.obj:
	CL $< $(OPT) $(WARNING) $(DEFINES) /c /Fo$@

.cpp.exe:
	CL $< $(OPT) $(WARNING) $(DEFINES) /Fe$@
	DEL $*.obj

clean:
	-DEL .\bin\opt\ .\bin\debug\ .\obj\opt\ .\obj\debug\ *.pdb *.exe *.exp *.obj *.idb *.lib /Q /F
