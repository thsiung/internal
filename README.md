The library for internal use
============================

`dmitigr_internal` - general purpose internal library of components, including
programs, functions, classes - everything, which can be used in various projects.

This library doesn't pretends to be used by anyone directly. The main goal of
this library is to store the (potentially) reusable code and share it for our
own projects. Every bit can be changed without care of the backward compatibility.

Installation and consuming
==========================

Currently, header-only version of `dmitigr_internal` is good enough for all
Dmitigr software dependent on `dmitigr_internal`. Thus, no build is required.

Dependencies
------------

- [CMake] build system version 3.13+;
- Optional, C++17 compiler ([GCC] 7.3+ or [Microsoft Visual C++][Visual_Studio] 15.7+).

Build time settings
-------------------

As stated above, usually no build is required. But if one wants, both shared and
static library can be build.

Settings that may be specified at build time by using [CMake] variables are:
  1. the type of the build;
  2. installation directories.

Details:

|CMake variable|Possible values|Default on Unix|Default on Windows|
|:-------------|:--------------|:--------------|:-----------------|
|**The type of the build**||||
|CMAKE_BUILD_TYPE|Debug \| Release \| RelWithDebInfo \| MinSizeRel|Debug|Debug|
|**Installation directories**||||
|CMAKE_INSTALL_PREFIX|*an absolute path*|"/usr/local"|"%ProgramFiles%\dmitigr_internal"|
|DMITIGR_INTERNAL_CMAKE_INSTALL_DIR|*a path relative to CMAKE_INSTALL_PREFIX*|"share/dmitigr_internal/cmake"|"cmake"|
|DMITIGR_INTERNAL_BIN_INSTALL_DIR|*a path relative to CMAKE_INSTALL_PREFIX*|"bin"|"bin"|
|DMITIGR_INTERNAL_DOC_INSTALL_DIR|*a path relative to CMAKE_INSTALL_PREFIX*|"share/dmitigr_internal/doc"|"doc"|
|DMITIGR_INTERNAL_LIB_INSTALL_DIR|*a path relative to CMAKE_INSTALL_PREFIX*|"lib"|"lib"|
|DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR|*a path relative to CMAKE_INSTALL_PREFIX*|"include"|"include"|

Installation on Linux
---------------------

    $ git clone https://github.com/dmitigr/internal.git
    $ mkdir -p internal/build
    $ cd internal/build
    $ cmake ..
    $ make install

Installation on Microsoft Windows
---------------------------------

Run the Developer Command Prompt for Visual Studio and type:

    > git clone https://github.com/dmitigr/internal.git
    > mkdir internal\build
    > cd internal\build
    > cmake -G "Visual Studio 15 2017 Win64" ..
    > cmake -P cmake_install.cmake

Consuming
---------

If you are using CMake the consuming of the `dmitigr_internal` library is very simple:

```cmake
cmake_minimum_required(VERSION 3.13)
project(foo)
find_package(dmitigr_internal REQUIRED CONFIGS dmitigr_internal_interface-config.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
add_executable(foo foo.cpp)
target_link_libraries(foo dmitigr_internal)
```

The above code snippet is minimal CMakeLists.txt that enough to build the
application `foo` that depends on the `dmitigr_internal` header-only library.

License
=======

This software is distributed under zlib license. For conditions of distribution
and use, see files `LICENSE.txt` or `internal.hpp`.

Copyright
=========

Copyright (C) Dmitry Igrishin

[CMake]: https://cmake.org/
[GCC]: https://gcc.gnu.org/
[Visual_Studio]: https://www.visualstudio.com/
