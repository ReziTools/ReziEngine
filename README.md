# ReziEngine
## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari
## Numerical approaches for resolving indeterminacies for various types of mechanical loads

# Building instructions

## Dependencies:
- C++ compiler compatible with ISO C++17 - Clang/GCC (MSVC not supported)
- POSIX subsystem - MSYS2+MinGW (Windows only)
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) - Optional for web support
- [Raylib](https://github.com/raysan5/raylib/releases/tag/5.0) 5.0
- [Eigen](https://gitlab.com/libeigen/eigen/-/releases/3.4.0) 3.4.0
- [CMake](https://cmake.org/download/) 3.29.1 + Ninja

## Build steps (native application):

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
./build/ReziEdit/ReziEdit.exe # On Windows using MSYS2
```

## Build steps (web application):
Building for web is currently only supported on Linux systems, you may use WSL on Windows.
1. Clone using git or download from the releases section:
```bash
git clone https://github.com/impact112/ReziEngine
```
2. Navigate into the project directory:
```bash
cd ReziEngine
```
3. Ensure emsdk is installed and active:
```bash
emsdk update
emsdk install latest
emsdk activate latest
```
4. Build the program using CMake:
```bash
emcmake cmake -S . -B build_web -DPLATFORM=Web -G "Ninja"
cmake --build build
```

