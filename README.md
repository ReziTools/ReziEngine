# ReziEngine
## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari
## Numerical approaches for ressolving indeterminacies for various types of mechanical loads

# Building instructions

## Dependencies:
- C++ compatibile cu compiler - Clang/GCC
- POSIX subsystem - MSYS2+MinGW (Windows only)
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) - Optional for web support
- [Raylib](https://github.com/raysan5/raylib/releases/tag/5.0) 5.0
- [Eigen](https://gitlab.com/libeigen/eigen/-/releases/3.4.0) 3.4.0
- [CMake](https://cmake.org/download/) 3.29.1 + Ninja

## Build steps:

1. Clone using git or download from the releases section:
```bash
git clone https://github.com/impact112/ReziEngine
```
2. Navigate into the project directory:
```bash
cd ReziEngine
```
3. Build the program using CMake:
```bash
cmake -B build -G "Ninja"
cmake --build build
```
4. The executable will be in the build/ReziEdit directory and will be named ReziEdit(.exe)
```bash
./build/ReziEdit/ReziEdit # On Linux/FreeBSD/macOS
./build/ReziEdit/ReziEdit.exe # On windows using MSYS2
```


