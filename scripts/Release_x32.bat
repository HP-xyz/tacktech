pause
call vcvars32.bat
call variables.bat
@set PATH=%PATH%;%QT_DIRECTORY%;
cmake .. -DCMAKE_BUILD_TYPE=Release -DQT_DIR=%QT_DIRECTORY% -DCMAKE_QMAKE_EXECUTABLE=%QT_DIRECTORY%\bin\qmake.exe -DEXECUTABLE_OUTPUT_PATH=.\Release_32x -G "NMake Makefiles"
nmake
pause