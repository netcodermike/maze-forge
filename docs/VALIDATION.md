# Validation

Authoring date: November 6, 2026. Host: Windows. Target: Windows, macOS, Linux; browser for playing exported mazes.

## Local result

C++ compiler unavailable on authoring host. CI compiles, tests maze invariants, and exports a playable HTML file.

## Reproducible checks

The GitHub Actions workflow builds/runs the following on `ubuntu-latest`:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/maze --width 10 --height 8 --seed 42 --output sample.html
```

## Manual acceptance

Generate minimum/maximum small reasonable dimensions, navigate to the exit, use all movement controls, reveal/reset, and compare two exports with the same seed.

A workflow file is not evidence of a passing run. Observed CI run links and source commits are recorded after publication. Native GUI, sound, sleep/resume, and platform permissions require direct target-platform acceptance even when model tests pass.

## Observed CI result

Passed on November 6, 2026: [target-platform build/check](https://github.com/netcodermike/maze-forge/actions/runs/42198904194).

Verified source commit: `e8106e745fb3da21dcf471d80f2208554a3da836`. This evidence identifies the exact tested source; a later documentation-only commit does not change that source.

The commands listed above passed. Native GUI/manual acceptance is not claimed by this CI result.
