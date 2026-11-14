[![](https://img.shields.io/badge/c++-black?logo=c++&style=for-the-badge)](https://learnxinyminutes.com/c++/)

# Orgcpp
Small library to parse org-mode files.

### Usage
#### Usage 1: headlines, listitems and texts
<table>
<tr>
<th>Input</th>
<th>C++</th>
</tr>
<tr>
<td>
<pre>
* Headline 1
Text headline 1
- Listitem 1
  Text listitem 1
** Headline 2
- Listitem 2
  - Listitem 3
    Text listitem 2
* Headline 3
- Listitem 4
</pre>
</td>
<td>
<pre>
RootElement *root   = orgcpp::parse("./path/to/org/file");
Headline *headline1 = root->get<Headline>(0);
Text     *text1     = headline1->get<Text>(0);
Listitem *listitem1 = headline1->get<Listitem>(1);
Headline *headline2 = headline1->get<Headline>(2);
Listitem *listitem2 = headline2->get<Listitem>(0);
Listitem *listitem3 = listitem2->get<Listitem>(0);
Text     *text2     = listitem3>get<Text>(0);
Headline *headline3 = root->get<Headline>(1);
Listitem *listitem4 = headline3->get<Listitem>(0);
</pre>
</td>
</tr>
</table>

##### Headline
Has attributes head and body.
```
std::string head = headline1->getAttribute<Region>("head")->string();
std::string body = headline1->getAttribute<Region>("body")->string();
```
Optionally has attribute content representing all its content (head+body):
```
std::string body = headline1->getAttribute<Region>("content")->string();
```

##### Listitem
Also has attributes head and body.
```
std::string head = listitem1->getAttribute<Region>("head")->string();
std::string body = listitem1->getAttribute<Region>("body")->string();
```
Also has the attribute content:
```
std::string body = headline1->getAttribute<Region>("content")->string();
```

##### Text
Has the attribute content
```
std::string content = text1->getAttribute<Region>("content")->string();
```
Also has the attribute head, but it is just a slice of the content:
```
std::string head = text1->getAttribute<Region>("head")->string();
```

### Debugging information
To see debug information about the parsing process, surround the parse function
with `config::debug = true` like:
```c++
config::debug = true;
RootElement *root = orgcpp::parse("./path/to/org/file");
config::debug = false;
```
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
cmake --build build --target orgcpp-unit-tests
./build/orgcpp-unit-tests
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
npx nodemon --exec "cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja"  && cmake --build build && ./build/orgcpp/orgcpp-unit-tests" --watch src -e cpp,hpp,txt
```
