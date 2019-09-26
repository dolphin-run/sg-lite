
# sg-lite

## Introduction

A lite render engine library with scene&graph model, write with c++11 purely.
OpenGL ES2.0 currently used backend but maybe others in the futrue.

The library has test with some Window System, such as Qt5
Also test with GLFW without Qt.

## Disclaimer
Qt
OpenGL
GLM
GLFW
GLEW

## How to Use

### Conformance

Benchmark        | Description
-----------------|---------------------------------------------------
Parse Validation | Use [JSON_checker](http://www.json.org/JSON_checker/) test suite to test whether the library can identify valid and invalid JSONs. (`fail01.json` is excluded as it is relaxed in [RFC7159]. `fail18.json` is excluded as depth of JSON is not specified.)
Parse Double     | 66 JSONs, each with a decimal value in an array, are parsed. The parsed `double` values are compared to the correct answer.
Parse String     | 9 JSONs, each with a string value in an array, are parsed. The parsed strings are compared to the correct answer.
Roundtrip        | 27 condensed JSONs are parsed and stringified. The results are compared to the original JSONs.


## Build and Run

1. Execute `git submodule update --init` to download all submodules (libraries).
2. Obtain [premake5](http://industriousone.com/premake/download).

For simplicity, on Linux/OSX users can simply run `make` (or `make CONFIG=release_x32`) at project root to run 4-10 above.

Some libraries, such as Boost, POCO, V8, etc., need to be installed by user manually.

## Examples
examples with sg-lite, which can be build&run in multi-platform with Qt or GLFW.

+ example/glfw
example with glfw, which can be build with makefile

+ example/qt5quick
+ example/qt5widget
+ example/qt5window
examples for Qt5(for modern and classic code, QtQuick&QtWidget), which can be all build easily with qt5example.pro

+ example/assets
image and video resources, which used in example.

+ example/example-scene
scenes will be shared with all examples.

*simplescene* render some basic nodes like rectangles and rings, some animation added for interesting.
*scenefantacy* render a binary tree recursively
*scene video* render a video with yuv420 

### Project Directories

Dir        | Description
-----------------|---------------------------------------------------
include |  sg-lite header files
lib     | sg-lite library pre-built.
src     | sg-lite library source code.
example        | examples with sg-lite, which can be build&run in multi-platform with Qt or GLFW.


![Conformance](sample/conformance_overall_Result.png)

to do...