[![](https://img.shields.io/badge/c++-black?logo=c++&style=for-the-badge)](https://learnxinyminutes.com/c++/)

# Orgcpp
Small library to parse org-mode files.

### Usage
TODO

### Build
---
#### Install VCPKG

Official Link: <https://vcpkg.io/en/index.html>

Export vcpkg to the path and export vcpkg root:
```cmd
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

Run bootstrap:
```cmd
cd $VCPKG_ROOT
.\vcpkg\bootstrap-vcpkg.bat # windows
./vcpkg/bootstrap-vcpkg.sh # Unix
```

Then, build at repository root:
```shell
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
cmake -S . -B ./build -G "Ninja"
cmake --build build
```

### Run
---
```shell
./build/orgcpp/orgcpp-cli
```

### Debug
---
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja"
cmake --build build
gdb ./build/orgcpp/orgcpp-cli
```

### Tests
---
```shell
cmake -S . -B build
cmake --build build --target unit-tests
./build/unit-tests
```

### Docs
---
```shell
cmake -S . -B build
cmake --build build --target docs
```
> Link:
> 
> https://natancamargo.github.io/orgcpp/html

### Watch with nodemon
```shell
npx nodemon --exec "cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja"  && cmake --build build && ./build/orgcpp/unit-tests" --watch src -e cpp,hpp,txt
```
