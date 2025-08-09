# 2D Game Engine

A lightweight 2D game engine focused on **data-oriented design**, with a custom **Entity-Component-System (ECS)**, **SDL** rendering, asset management, and planned **embedded Lua scripting**.

> [!WARNING]
> This software is still a work in progress and is actively being developed.

## Features

* **ECS core**: simple, cache-friendly component storage and system scheduling
* **Rendering**: 2D renderer (SDL)
* **Assets**: texture loading, level data, and resource lifetime management
* **Utilities**: input handling, timing, logging
* **C++23**: making use of modern language features whenever possible

## Tech

* C++23, SDL2
* Makefile-based build

## Getting Started

### Prerequisites

* C++ compiler with C++23 support (clang++ or g++)
* SDL2 development libraries
* Lua (e.g., Lua 5.4)
* Make

On macOS (Homebrew):

```bash
brew install sdl2 lua
```
### Build & Run

```bash
make
./build/engine    # or the binary produced by your Makefile
```

### Project Structure

```
├─ assets/        # sprites and levels
├─ libs/          # third-party
├─ src/           # engine source (ECS, renderer, systems)
└─ Makefile
```
