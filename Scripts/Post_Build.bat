cls
@echo off

setlocal
set Post_BuildPath=%~dp0Post_Build

:Start
echo  Start %0 %*

if not exist "%Post_BuildPath%" do goto Exit

:LaunchPreBuildScripts
echo search for "%Post_BuildPath%\*.bat
for /f %%P in ('dir /B "%Post_BuildPath%\*.bat"') do (
	call "%Post_BuildPath%\%%P"
)

:Exit
echo End %0 %*
endlocal
pause
Exit