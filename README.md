## blocs game engine

<br>

<div align="center">

<image alt="blocs logo" src="https://user-images.githubusercontent.com/64439681/181867600-7b1d4d3c-82f7-4a07-8345-831b53629ff2.svg">

<br>
<br>

[![License](https://img.shields.io/badge/license-MIT-red.svg?style=flat-square)](https://en.wikipedia.org/wiki/C%2B%2B14)
[![Language](https://img.shields.io/badge/language-C++-yellow.svg?style=flat-square)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg?style=flat-square)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build](https://img.shields.io/github/actions/workflow/status/benjitrosch/blocs/release.yml?style=flat-square)](https://github.com/benjitrosch/blocs/actions)

</div>

<h3 align="center">🚧🚧🚧 UNDER CONSTRUCTION! USE AT YOUR OWN RISK! 🚧🚧🚧</h3>

<h4 align="center">

  [Graphics](/include/blocs/graphics/README.md)
  |
  [Entity Component System](/include/blocs/ecs/README.md)
  |
  [Debug](/include/blocs/debug/README.md)
  
</h4>

### Quick start

In `main.cpp`:

```cpp
#include <blocs.h>
using namespace blocs;

void Game::start() {}
void Game::update() {}
void Game::render() {}

int main(int argc, char* argv[])
{
  return Game("my cool game", 1920, 1080)
    .run(60);
}
```

### Installation

Requires C++20 and [CMake 3.8+](https://cmake.org/)

Add the `blocs` repository as a git submodule or use CMake's `FetchContent` module. To reference the library use `target_link_libraries(${CMAKE_PROJECT_NAME} blocs)`.

Alternatively, you can use the [hub](https://github.com/benjitrosch/blocs-explorer) for easy install.

### Build

To compile and build external dependencies run:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

To build for [WASM](https://en.wikipedia.org/wiki/WebAssembly) using [Emscripten](https://emscripten.org/) run:

```bash
# for OSX and Linux
source ~/emsdk/emsdk_env.sh
# for Windows
emsdk_env.bat

emcmake cmake . -B wasm-build
cmake --build wasm-build
```

---

### Library Dependencies

Platform:
  * [SDL 2.26.0](https://www.libsdl.org/)

Graphics:
  * [OpenGL 3.3](https://www.opengl.org/)
  * [stb_image, stb_truetype](https://github.com/nothings/stb)

---

### License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/benjitrosch/blocs/blob/main/LICENSE) file for details.
