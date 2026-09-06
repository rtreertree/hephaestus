# hephaestus

Cross-platform (Windows / Linux / macOS) engine foundation on bgfx + SDL3.

## Clone

```bash
git clone --recursive https://github.com/you/hephaestus.git
cd hephaestus
# if you forgot --recursive:
git submodule update --init --recursive
```

## Prerequisites

| OS | Needs |
|---|---|
| Windows | VS 2022 (Desktop C++), CMake 3.25+, Ninja |
| Linux | `build-essential cmake ninja-build libx11-dev libxext-dev libxrandr-dev libxi-dev libxcursor-dev libxfixes-dev libwayland-dev libxkbcommon-dev wayland-protocols libgl1-mesa-dev` |
| macOS | Xcode Command Line Tools, CMake 3.25+, Ninja |

## Build

```bash
cmake --preset debug
cmake --build --preset debug
```





Binary: `build/debug/sandbox/sandbox[.exe]`

## Git hooks

Enable the repository pre-commit checks once after cloning:

```bash
git config core.hooksPath .githooks
```

The hook checks staged C and C++ files for trailing whitespace, tabs or mixed
indentation, and opening braces placed on their own line.
