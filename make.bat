@echo off

set PROJECT_ROOT_DIR=%CD%

cd _build

mingw32-make install
if not %ERRORLEVEL% == 0 (
  echo mingw32-make did not run successfully
  exit /B %ERRORLEVEL%
)

cd "%PROJECT_ROOT_DIR%"

exit /B %ERRORLEVEL%
