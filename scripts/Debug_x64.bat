pause
call vcvars64.bat
call variables.bat
@set PATH=%PATH%;%QT_DIRECTORY%;
cmake .. -DCMAKE_BUILD_TYPE=Debug -DQT_DIR=%QT_DIRECTORY% -DCMAKE_QMAKE_EXECUTABLE=%QT_DIRECTORY%\bin\qmake.exe -DEXECUTABLE_OUTPUT_PATH=.\Debug_64x -G "CodeBlocks - NMake Makefiles"
nmake
pause