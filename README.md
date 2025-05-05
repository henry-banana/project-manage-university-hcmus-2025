# manage-university-project
buiding the project using cmake and mingw

## Build Instructions

To build the project using CMake and MinGW, run the following command:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
```

```powershell
if (-not (Test-Path build)) { mkdir build }; cmake -S . -B build -G "MinGW Makefiles"
``` 

```powershell
cmake --build build --config release
``` 
