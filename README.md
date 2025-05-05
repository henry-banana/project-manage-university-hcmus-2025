# manage-university-project
buiding the project using cmake and mingw

## Build Instructions

To build the project using CMake and MinGW, run the following command:

# Yêu cầu configuration và build cho Windows 10/11 với MinGW (GCC) và CMake:
- CMake
- MinGW (GCC)

```powershell
if (-not (Test-Path build)) { mkdir build }; cmake -S . -B build -G "MinGW Makefiles"
``` 

```powershell
cmake --build build --config release
``` 

# Hoặc nếu bạn không muốn chỉ định generator, bạn có thể để CMake tự động chọn generator phù hợp với hệ thống của bạn bằng cách bỏ qua tham số `-G`:

```powershell
if (-not (Test-Path build)) { mkdir build }; cmake -S . -B build  # Bỏ qua -G để CMake tự chọn generator phù hợp
``` 

```powershell
cmake --build build --config release
```