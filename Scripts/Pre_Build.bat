cls
@echo off

setlocal
set Pre_BuildPath=%~dp0Pre_Build
:Start
echo  Start %0 %*

if not exist "%Pre_BuildPath%" do goto Exit

:LaunchPreBuildScripts
echo search for "%Pre_BuildPath%\*.bat
for /f %%P in ('dir /B "%Pre_BuildPath%\*.bat"') do (
	call "%Pre_BuildPath%\%%P"
)

:Exit
echo End %0 %*
endlocal
pause
Exit