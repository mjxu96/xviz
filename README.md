# XVIZ In C++

## Minimum Requirements
1. CMake 3.5.0+
2. Protobuf 3.11.0+

## Example

Please see [example.cpp](https://github.com/wx9698/xviz/blob/master/main.cpp) and [CMakeLists.txt](https://github.com/wx9698/xviz/blob/master/CMakeLists.txt) for more information.

## Build

1. Build example
```bash
mkdir build && cd build
cmake ../
make example -j12
../bin/example
```

2. Build tests
```bash
mkdir build && cd build
cmake ../
make xviz-test -j12
../bin/xviz-test
```

## Related Work
1. [uber xviz](https://github.com/uber/xviz)
2. [cmpute xviz.py](https://github.com/cmpute/xviz.py)