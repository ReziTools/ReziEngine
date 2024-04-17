# Rezistenta (titlu program provizoriu)
## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari

# Instructiuni compilare

## Programe/librarii necesare:
- Compilator C++ compatibil cu C++11 - Clang/GCC/MSVC
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) - Optional pentru suport web
- [Raylib](https://github.com/raysan5/raylib/releases/tag/5.0) 5.0
- [Eigen](https://gitlab.com/libeigen/eigen/-/releases/3.4.0) 3.4.0
- [CMake](https://cmake.org/download/) 3.29.1 + Ninja/GNU make

## Pasi compilare:

1. Clonati sau descarcati proiectul de la sectiunea Releases:
```bash
git clone https://github.com/impact112/rezistenta
```
2. Navigati in directoriul proiectului:
```bash
cd rezistenta
mkdir build
```
3. Compilati programul folosind CMake:
```bash
cmake build
cmake --build build
```
4. Executabilul se va afla in directoriul build si se va numi rezistenta(.exe)


