# XVIZ In C++ [![Build Status](https://travis-ci.com/wx9698/xviz.svg?branch=master)](https://travis-ci.com/wx9698/xviz)

## Minimum Requirements
1. CMake 3.5.0+
2. Protobuf 3.11.0+

## Example

Please see [example.cpp](https://github.com/wx9698/xviz/blob/master/examples/example.cpp), [example_server.cpp](https://github.com/wx9698/xviz/blob/master/examples/example_server.cpp) and [CMakeLists.txt](https://github.com/wx9698/xviz/blob/master/CMakeLists.txt) for more information.

## Build

1. Build xviz builder example
```bash
mkdir build && cd build
cmake ../
make example -j12
../bin/example
```

2. Build xviz server example
```bash
mkdir build && cd build
cmake ../
make example-server -j12
../bin/example-server [PNG FILE PATH (this is optional)]
```

3. Build tests
```bash
mkdir build && cd build
cmake ../ -DBUILD_TEST=ON
make xviz-test -j12
../bin/xviz-test
```

## Related Work
1. [uber xviz](https://github.com/uber/xviz)
2. [cmpute xviz.py](https://github.com/cmpute/xviz.py)

## Third-Party Libraries
1. [zaphoyd websocketpp](https://github.com/zaphoyd/websocketpp)
2. [chriskohlhoff asio](https://github.com/chriskohlhoff/asio)
3. [mcrodrigues macro-logger](https://github.com/dmcrodrigues/macro-logger)
4. [nlohmann json](https://github.com/nlohmann/json)