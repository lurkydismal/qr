@echo off

cls

set files=main.c logic.c ltoa.c malloc.c stdfunc.c
set object_files=main.obj logic.obj ltoa.obj malloc.obj stdfunc.obj

set compiler=cl.exe
set compiler_flags=/EHsc /MD /nologo /GS- /O2 /Ot /c /I ../include /std:c11 /DSERVER

cd src

%compiler% %compiler_flags% %files%

move *.obj ../
cd ../

set linker=Crinkler.exe
set linker_flags=/CRINKLER /TINYHEADER /TINYIMPORT
set linker_libs="kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "ws2_32.lib" "mswsock.lib"

%linker% %linker_flags% %linker_libs% %object_files%

pause
