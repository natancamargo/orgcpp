[![](https://img.shields.io/badge/c++-black?logo=c++&style=for-the-badge)](https://learnxinyminutes.com/c++/)

<img width="287" height="284" alt="image" src="https://github.com/user-attachments/assets/1b74f362-daa6-4a58-803e-68a42d540fc3" />

## Hello World in C++
Just a cpp hello world with raylib built with cmake.

### Build
```shell
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
cmake -S . -B ./build
cmake --build build
```

### Run
```shell
./build/hello-world/exe
```

### Debug
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
gdb ./build/hello-world/exe
```

### Tests
```shell
cmake -S . -B build
cmake --build build --target unit-tests
./build/tests/unit-tests
```

### Watch with nodemon
```shell
npx nodemon --exec "cmake -S . -B build && cmake --build build && ./build/hello-world/exe" --watch src -e cpp,hpp,txt
```
