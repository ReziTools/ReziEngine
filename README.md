# Rezistenta (titlu provizoriu)
## Abordari numerice pentru ridicarea nedeterminarilor pentru diverse tipuri de solicitari

# Instructiuni compilare

## Programe/librarii necesare:
- CMake + Ninja/make
- Clang (alte compilatoare vor fi suportate in viitor)
- Raylib 5.0.0
- Eigen

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

