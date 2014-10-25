@echo off

if not "%1"=="clean" (
	call :subroutine	
) else (
	rd /q /s ..\bin
)
goto :end

:subroutine
	if exist ..\bin echo ""
	if not exist ..bin mkdir ..\bin
	XCOPY /Y .\dll ..\bin
	goto :eof
:end