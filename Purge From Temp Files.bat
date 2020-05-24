cls
@echo off

echo start %0 %*
::Clean WorkSpaces Directory
::Search for sln File
for /f "delims=" %%i in ('dir /B /S "%~dp0Workspaces\*.sln"') do (
	echo Visual Studio Solution found %%i
	
	for /f "delims=" %%i in ('dir /B /S /A:H "%%~pi\*.suo"') do (
		del /A:H "%%i"
	)
	for /f "delims=" %%i in ('dir /B /S "%%~pi\*.sdf"') do (
		del "%%i"
	)
	for /f "delims=" %%i in ('dir /B /S /A:D "%%~pi\*"') do (
		RMDIR /S /Q "%%i"
	)
)
::Search for vcproj;vcxproj File
for /f "delims=" %%i in ('dir /B /S "%~dp0Projects\*.vcproj";"%~dp0Projects\*.vcxproj"') do (
	echo Visual Studio Project found %%i
	
	for /f "delims=" %%i in ('dir /B /S /A:H "%%~pi\*.suo"') do (
		del /A:H "%%i"
	)
	for /f "delims=" %%i in ('dir /B /S "%%~pi\*.sdf";"%%~pi\*.filters";"%%~pi\*.user";"%%~pi\*.sln"') do (
		del "%%i"
	)
	for /f "delims=" %%i in ('dir /B /S /A:D "%%~pi\*"') do (
		RMDIR /S /Q "%%i"
	)
)
::Search for Build Directory
if exist "%~dp0Build" RMDIR /Q /S "%~dp0Build"

::Search for Build Directory
if exist "%~dp0Delivery" RMDIR /Q /S "%~dp0Delivery"

:Exit
echo stop %0 %*
pause