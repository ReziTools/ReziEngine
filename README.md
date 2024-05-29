# ReziEngine

# This project is discontinued and will be replaced by ReziSim.

## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari
## Numerical approaches for resolving indeterminacies for various types of mechanical loads

Try the latest web version [here](https://reziengine.github.io/latest/ReziEdit.html).

See all the previous web releases [here](https://reziengine.github.io).

# Features
- GUI interface for editing mechanical load systems
- TOML mechanical load system definition
- Native and Web support
- CLI interface

# Building instructions

## Dependencies:
- C++ compiler compatible with ISO C++11 - Clang/GCC (MSVC not supported)
- POSIX subsystem - MSYS2+MinGW (Windows only)
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) - Optional for web support
- [Raylib](https://github.com/raysan5/raylib/releases/tag/5.0) 5.0
- [Eigen](https://gitlab.com/libeigen/eigen/-/releases/3.4.0) 3.4.0
- [toml11](https://github.com/ToruNiina/toml11/releases/tag/v3.8.1) 3.8.1
- [CMake](https://cmake.org/download/) 3.29.1 + Ninja

## Build steps (native application):
The following steps are for \*nix systems.

If building on Windows make sure you are in a MinGW environment like MSYS2 MINGW64 and follow the same steps.

Clang, CMake, Ninja and git must be installed on your system beforehand.

CMake will pull any libraries that the program depends on if not available.
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
cmake -S . -B build -G "Ninja"
cmake --build build
```
4. The executables will be in the build/ReziEdit and build/ReziCLI directories:
- On \*nix systems:
```bash
./build/ReziEdit/ReziEdit
./build/ReziCLI/ReziCLI
```
- On Windows systems:
```bash
./build/ReziEdit/ReziEdit.exe
./build/ReziCLI/ReziCLI.exe
```

## Build steps (web application):
Building for web is currently only supported on \*nix systems, you may use MSYS2 on Windows.
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
cmake --build build_web
```
5. The resource files will be inside the build\_web/ReziEdit directories.
