@echo off
setlocal

set "CUBEPROGRAMMER_TOOL_PATH=C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
set "BIN_DIR=%~pd0..\..\..\STM32CubeIDE\FSBL\Debug\"
set "OUT_DIR=%~pd0..\..\..\STM32CubeIDE\FSBL\Debug\"

:: Check if STM32CubeProgrammer is found
if not exist "%CUBEPROGRAMMER_TOOL_PATH%" (
    echo **************************************************************************************************************
    echo *                                                                                                            *
    echo * STM32CubeProgrammer is not found. Make sure the STM32CubeProgrammer is installed in its default directory  *
    echo * You can download it at https://www.st.com/en/development-tools/stm32cubeprog.html                          *
    echo *                                                                                                            *
    echo **************************************************************************************************************
    pause
    exit /b
)

:: Navigate to the script's directory
pushd "%BIN_DIR%"

:: Sign the app 
"%CUBEPROGRAMMER_TOOL_PATH%\STM32_SigningTool_CLI.exe" -s -bin "%BIN_DIR%STM32N6570-DK-FreeRTOS_FSBL.bin" -nk -of 0x80000000 -t fsbl -o "%OUT_DIR%STM32N6570-DK-FreeRTOS_FSBL-trusted.bin" -hv 2.3 -align -dump "%OUT_DIR%STM32N6570-DK-FreeRTOS_FSBL-trusted.bin"

:: Load the binary
"%CUBEPROGRAMMER_TOOL_PATH%\STM32_Programmer_CLI.exe" -c port=SWD mode=HOTPLUG AP=1 -el "%CUBEPROGRAMMER_TOOL_PATH%\ExternalLoader\MX66UW1G45G_STM32N6570-DK.stldr" -d "%OUT_DIR%STM32N6570-DK-FreeRTOS_FSBL-trusted.bin" 0x70000000

:: Return to the original directory
popd

:: Pause the script to view any messages
pause