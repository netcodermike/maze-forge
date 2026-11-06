# Maze Forge

Generate a seeded perfect maze in C++ and export a self-contained browser game with movement controls and a route reveal.

[![Build and verify](https://github.com/netcodermike/maze-forge/actions/workflows/verify.yml/badge.svg)](https://github.com/netcodermike/maze-forge/actions/workflows/verify.yml)

**Language:** C++ · **Platform:** Windows, macOS, Linux; browser for playing exported mazes · **App type:** Procedural puzzle game generator

First-release learning implementation, with explicit scope below. This is one independently runnable app, not a placeholder in a combined repository.

## Features

- Seeded randomized depth-first maze carving.
- Configurable 2–80 cell width and height.
- Portable single-file HTML game, keyboard/buttons, restart, and move count.
- BFS solution route reveal with no server or online assets.

## Requirements

C++17 compiler, CMake 3.16+, and a modern browser. No external libraries.

## Quick start

```sh
git clone https://github.com/netcodermike/maze-forge.git
cd maze-forge
cmake -S . -B build
cmake --build build --config Release
# Linux/macOS:
./build/maze --width 20 --height 15 --seed 42 --output maze.html
# Windows with Visual Studio generator:
.\build\Release\maze.exe --output maze.html
# Open maze.html in a browser.
```

Commands run from the repository root. Lines marked for a particular operating system apply only there. Language tools are prerequisites, not installed by the app. Close desktop windows normally; terminal/server apps stop with Ctrl+C unless their usage notes say otherwise.

## Example workflow

Generate a 10×8 maze with seed 42. Open the HTML, move with arrows/WASD or buttons, and reach the pink exit. Restart should clear moves and the revealed route. Generate another seed and compare layouts.

## Architecture, configuration, and data

`main.cpp` represents each cell as four wall bits (north/east/south/west). A stack-based DFS removes paired walls between unvisited neighbors. A BFS predecessor map builds the solution path. The generator writes HTML containing wall/route data and a canvas game. The entire exported game is offline. CMake builds one executable and registers --self-test. The self-test examines 20 seeds, edge counts, a solution, deterministic generation within the same toolchain, and invalid dimensions.

## Verification

Build with the quick-start command where needed, then run:

```sh
ctest --test-dir build -C Release --output-on-failure
```

Manual acceptance: Generate minimum/maximum small reasonable dimensions, navigate to the exit, use all movement controls, reveal/reset, and compare two exports with the same seed.

See [validation record](docs/VALIDATION.md) for observed checks and their limits. CI results verify only what the workflow runs; they do not establish native GUI correctness or production readiness.

## Limitations and next steps

Mazes are rectangular, orthogonal, and perfect (one unique route between cells). There is no editor, save position, difficulty scoring, or 3D view. Route reveal is an assist, not an anti-cheat system. mt19937 is deterministic but reproducibility is scoped to the same build/algorithm implementation. Output paths are overwritten if chosen. Keyboard shortcuts apply while focus is outside buttons.

## Troubleshooting

Install a compiler if CMake cannot select one. Multi-config Visual Studio puts the executable in build/Release; single-config builds use build/maze. Open the generated HTML, not main.cpp. Dimensions outside 2–80 fail explicitly. If keyboard movement does not work, click the canvas.

## Source map

- `CMakeLists.txt`
- `main.cpp`

## Development and license

Read [contributing](CONTRIBUTING.md) before changing the app, and [security/privacy](SECURITY.md) before supplying private data. Use synthetic fixtures for tests. No telemetry, paid API key, or account is required by the app itself. Dependencies, when present, are stated in Requirements.

[MIT license](LICENSE), copyright 2026 netcodermike.
