# XVIZ Protocol In C++
[![libxviz CI](https://github.com/mjxu96/xviz/actions/workflows/ci.yml/badge.svg)](https://github.com/mjxu96/xviz/actions/workflows/ci.yml) [![codecov](https://codecov.io/gh/mjxu96/xviz/branch/master/graph/badge.svg?token=JDDAW7YU7B)](https://codecov.io/gh/mjxu96/xviz)

## Overview
This is an _incomplete_ implementation of [XVIZ protocol](https://github.com/aurora-opensource/xviz) in C++.

This project has been migrated to [Conan](https://conan.io/). Please checkout to [old](https://github.com/mjxu96/xviz/tree/old) branch for legacy build mechanism.

## Requirements
1. Conan 1.55.0+
2. C++20 compiler

## Example
Please see [example.cc](examples/example.cc), [example_server.cc](examples/example_server.cc) for more information.

## Use Case
1. [CarlaViz](https://github.com/mjxu96/carlaviz)

## Build and Test

### Download from Conan remote
```bash
conan remote add gitlab https://gitlab.com/api/v4/projects/44861904/packages/conan
conan install xviz
```
### Build xviz builder example
```bash
mkdir build && cd build
conan install -pr gcc11 --build=missing -o build_examples=True ..
conan build .. --build
./examples/example
```

### Build xviz server example
Frontend is needed. You can refer to these two repos for frontend:
1. [CarlaViz Frontend](https://github.com/mjxu96/carlaviz/tree/master/frontend)
2. [Aurora streetscape.gl](https://github.com/aurora-opensource/streetscape.gl)
```bash
mkdir build && cd build
conan install -pr gcc11 --build=missing -o build_examples=True ..
conan build .. --build
./examples/example_server [PNG FILE PATH (this is optional)]
```

### Build tests
```bash
mkdir build && cd build
conan install -pr gcc11 --build=missing -o build_tests=True ..
conan build .. --test
```

## Format script
```bash
find . -iname *.h -not -path "./build/*" -o -iname *.cc -not -path "./build/*" | xargs clang-format -i -style=file
```
