# ODAS Directory Cleanup

This document describes deprecated files and directories that can be safely removed.

## Redundant Build Directories

The following directories are from old build attempts and can be safely deleted:

```bash
# Old/redundant build directories (~80MB total)
rm -rf build-windows/
rm -rf build-windows-simple/
rm -rf fftw-mingw-build/
rm -rf windows-deps/
```

These directories were used during development and testing but are no longer needed because:
- **build-windows/** and **build-windows-simple/** - Old build outputs (empty bin directories)
- **fftw-mingw-build/** - Old FFTW build (not used by current ODAS Windows build)
- **windows-deps/** - Old dependency management (build script now manages dependencies in build-mingw/deps)

## Current Build System

The current Windows build system uses:
- **build-mingw/** - Active build directory (created by `build-scripts/build_mingw.sh`)
  - Contains compiled executables, DLLs, and dependencies
  - This directory should be kept (or can be cleaned with `--clean` flag)
- **build/** - Linux build directory
  - This directory should be kept for Linux builds

## Cleanup Command

To clean up all redundant directories at once:

```bash
# From the odas/ directory
rm -rf build-windows build-windows-simple fftw-mingw-build windows-deps
```

This will free up approximately 80MB of disk space.

## Note

After cleanup, the directory structure will be:

```
odas/
├── build-scripts/     # Build scripts and toolchain files
├── docs/              # Documentation
├── build-mingw/       # Windows build output (keep)
├── build/             # Linux build output (keep)
├── config/            # Configuration files
├── src/               # Source code
├── include/           # Headers
└── CMakeLists.txt     # CMake configuration
```
