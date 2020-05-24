cls 
@echo off
setlocal

echo Start %0 %*
::%1 -> ProjectOuPutDirectory
::%2 -> PackageName

if "%*" == "" goto Exit

set SourcePath=%~dp0..\..\Sources
set LibsPath=%1
set TargetName=%2
set OutDirectory=%~dp0..\..\Delivery

echo Source Path at %SourcePath%
echo Lib Path at %LibsPath%
echo Target Name %TargetName%
echo OutDirectory %OutDirectory%

:SourcePathTest
if "%SourcePath%" == "" goto Exit
if not exist "%SourcePath%" goto Exit

:LibsPathTest
if "%LibsPath%" == "" goto Exit
if not exist "%LibsPath%" goto Exit



for /f "delims=|" %%f in ('dir /B/S "%LibsPath%\*.lib"') goto OutDirectoryTest
for /f "delims=|" %%f in ('dir /B/S "%LibsPath%\*.dll"') goto OutDirectoryTest
goto Exit

:OutDirectoryTest
if "%OutDirectory%" == "" goto Exit
if exist "%OutDirectory%" RMDIR "%OutDirectory%"
MD "%OutDirectory%"

if not exist "%OutDirectory%" goto Exit
echo OutDirectory Path at %OutDirectory%

:MakePackage
echo MakePackage : 
echo Sources at %SourcePath%
echo Libs at %LibsPath%
echo Make package in %OutDirectory%

:PackageHeader
for /f "delims=|" %%f in ('dir /B/S "%SourcePath%\.h"') (
	echo %%f
)

:PackageTemplateHeader
for /f "delims=|" %%f in ('dir /B/S "%SourcePath%\.hpp"') (
	echo %%f
)

:PackageLibs
for /f "delims=|" %%f in ('dir /B/S "%LibsPath%\.lib"') (
	echo %%f
)
for /f "delims=|" %%f in ('dir /B/S "%LibsPath%\.dll"') (
	echo %%f
)

:Exit
echo End %0 %*
pause