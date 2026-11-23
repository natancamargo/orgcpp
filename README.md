[![](https://img.shields.io/badge/c++-black?logo=c++&style=for-the-badge)](https://learnxinyminutes.com/c++/)

# Orgcpp

A small library for parsing Org-mode files.

### Usage

#### Usage 1: Headlines, Listitems, and Texts

<table>
<tr>
<th>Input</th>
<th>AST</th>
<th>C++</th>
</tr>
<tr>
<td>
<pre>
...
* Headline 1
Text 1
- Listitem 1
  Text 2
** Headline 2
- Listitem 2
  - Listitem 3
    Text 3
* Headline 3
- Listitem 4
</pre>
</td>
<td>
<pre>
Root
 Headline
  Text
  Listitem
   Text
  Headline
   Listitem
    Listitem
     Text
 Headline
  Listitem
</pre>
</td>
<td>
<pre>
RootElement *root    = orgcpp::parse("./path/to/org/file");
Headline    *headline1 = root->get<Headline>(0);
Text        *text1     = headline1->get<Text>(0);
Listitem    *listitem1 = headline1->get<Listitem>(1);
Text        *text2     = listitem1->get<Text>(0);
Headline    *headline2 = headline1->get<Headline>(2);
Listitem    *listitem2 = headline2->get<Listitem>(0);
Listitem    *listitem3 = listitem2->get<Listitem>(0);
Text        *text3     = listitem3->get<Text>(0);
Headline    *headline3 = root->get<Headline>(1);
Listitem    *listitem4 = headline3->get<Listitem>(0);
</pre>
</td>
</tr>
</table>

##### Headline

Has the `head` and `body` attributes:

```c++
std::string head = headline1->getAttribute<Region>("head")->string();
std::string body = headline1->getAttribute<Region>("body")->string();
```

It also has a `content` attribute, which represents its entire content (head + body):

```c++
std::string content = headline1->getAttribute<Region>("content")->string();
```

##### Listitem

Also has the `head` and `body` attributes:

```c++
std::string head = listitem1->getAttribute<Region>("head")->string();
std::string body = listitem1->getAttribute<Region>("body")->string();
```

It also has the `content` attribute:

```c++
std::string content = listitem1->getAttribute<Region>("content")->string();
```

##### Text

Has the `content` attribute:

```c++
std::string content = text1->getAttribute<Region>("content")->string();
```

It also has the `head` attribute, which represents a slice of the content:

```c++
std::string head = text1->getAttribute<Region>("head")->string();
```

### Basics

#### Element

Element is the base class in orgcpp. It represent most of the parsable content of an Org-mode file in the chosen model.

Below are the supported elements:

##### Headline
```org
* Headline
Text
```

##### Listitem
```org
- Listitem
  Text
```

##### Text
```org
Text
```

##### Drawer
```org
:DRAWER:
Text
:END:
```

##### Block
```org
#+begin_src c++
Text
#+end_src
```

##### Table
```org
| header 1 | header 2 |
| cell 1   | cell 2   |
```

##### Link
```org
[[link]]
```

##### Properties
```org
:PROPERTIES:
:Property1: "abc"
:Property2: "cba"
:END:
```

##### Tag
```org
* Meeting with the French group                                        :work:
```

##### Date
```org
* Headline
<2006-11-01 Wed 19:15>

* Headline
DEADLINE: <2004-02-29 Sun>

* Headline
SCHEDULED: <2004-12-25 Sat>
```

##### Keyword
```org
#+KEY: VALUE
```

##### LaTeX
```org
\begin{equation}                        % arbitrary environments,
x=\sqrt{b}                              % including tables, figures, etc.
\end{equation}
```

##### Footnote
```org
[fn:text]
```

#### Attributes

In orgcpp, elements have attributes that can be accessed as follows:

```c++
Region *headRegion = headline1->getAttribute<Region>("head");
```

> **Region**
>
> A Region is a tuple that points to the location of an attribute within the document.

Some elements may not have certain attributes. To check whether an attribute is available, use something like:

```c++
Region *headRegion = headline1->getAttribute<Region>("head");
if (headRegion) {
    ...
}
```

#### Debugging Information

To display debugging information about the parsing process, set `config::debug` to `true` before calling the parse function:

```c++
config::debug = true;
RootElement *root = orgcpp::parse("./path/to/org/file");
config::debug = false;
```

#### AST

To print the AST of a document, use the `print()` function:

```c++
RootElement *root = orgcpp::parse("./path/to/org/file");
root->print();
```

#### Finding
orgcpp allows to find-traverse the ast of an org-mode file using a
condition. It can obtain one or all elements, example:
```c++
  // Find one listitem by type
  Listitem *listitem = dynamic_cast<Listitem *>(root->findOne(
      [](Element *element) { return element->getType() == Type::LISTITEM; }));
  assert(listitem);
  // Find all elements by type
  std::vector<Element *> listitems = root->findAll([](Element *element) {
        return element->getType() == Type::LISTITEM;
      });
  assert(listitems.size() == 4);
```

### Build

#### Install VCPKG

Official website: <https://vcpkg.io/en/index.html>

Add vcpkg to your PATH and set the vcpkg root directory:

```shell
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

Run the bootstrap script:

```shell
cd $VCPKG_ROOT
.\vcpkg\bootstrap-vcpkg.bat # Windows
./vcpkg/bootstrap-vcpkg.sh # Unix
```

Then, build the project from the repository root:

```shell
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
cmake -S . -B ./build -G "Ninja"
cmake --build build
```

### Run

```shell
./build/orgcpp/orgcpp-cli
```

### Debug

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja"
cmake --build build
gdb ./build/orgcpp/orgcpp-cli
```

### Tests

```shell
cmake -S . -B build
cmake --build build --target orgcpp-unit-tests
./build/orgcpp-unit-tests
```

### Watch with Nodemon

```shell
npx nodemon --exec 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja" && cmake --build build && ./build/orgcpp/orgcpp-unit-tests' --watch src -e cpp,hpp,txt
```
