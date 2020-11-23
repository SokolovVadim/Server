# Server
[![Build Status](https://travis-ci.com/SokolovVadim/Server.svg?branch=main)](https://travis-ci.com/SokolovVadim/Server)

![UML](https://github.com/SokolovVadim/Server/blob/main/resources/Server.jpg)

![Proxy](https://github.com/SokolovVadim/Server/blob/main/resources/Network.jpg)

## Build 

```
cmake -H. -Bcmake-build-debug 
cmake --build cmake-build-debug
```

## Run as Server

```
./cmake-build-debug/Server server
```

## Run as Client

```
./cmake-build-debug/Server client
```

## Build coverage

```
cmake -DCMAKE_BUILD_TYPE=Coverage --build cmake-build-debug
cd cmake-build-debug/
make
make coverage
```
