# Rezistenta (titlu provizoriu)
## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari

# Instructiuni compilare

## Programe/librarii necesare:
- Compilator C++ compatibil cu C++11 - Clang/GCC/MSVC
- (Emscripten SDK)[https://emscripten.org/docs/getting_started/downloads.html] - Optional pentru suport web
- (Raylib)[https://github.com/raysan5/raylib/releases/tag/5.0] 5.0
- (Eigen)[https://gitlab.com/libeigen/eigen/-/releases/3.4.0] 3.4.0
- (CMake)[https://cmake.org/download/] 3.29.1 + Ninja/GNU make

## Pasi compilare:

1. Clonati sau descarcati proiectul de la sectiunea Releases:
```bash
git clone https://github.com/impact112/rezistenta
```
2. Navigati in directoriul proiectului
```bash
cd rezistenta
mkdir build
cd build
```
3. Compilati folosind CMake si make:
```bash
cmake ..
make
```
Sau folosind CMake si ninja:
```bash
cmake .. -GNinja
ninja
```

