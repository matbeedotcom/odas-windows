# ODAS Windows Build Documentation

This directory contains documentation for building and running ODAS on Windows.

## Quick Start

For a quick start guide, see [QUICKSTART_WINDOWS.md](QUICKSTART_WINDOWS.md).

For detailed build instructions, see [BUILD_WINDOWS.md](BUILD_WINDOWS.md).

## Building for Windows

ODAS can be cross-compiled for Windows from Linux (including WSL) using MinGW-w64. The build process:

1. **Prerequisites**: MinGW-w64, CMake, build-essential
2. **Build**: Run `../build-scripts/build_mingw.sh`
3. **Output**: Windows executables and DLLs in `build-mingw/bin/`

### Simple Build Command

```bash
cd /path/to/odas
./build-scripts/build_mingw.sh
```

This will build:
- `libodas.dll` - Main ODAS library with WASAPI support
- `odaslive.exe` - Real-time audio processing application
- `odasserver.exe` - Server application
- Required dependency DLLs (libconfig, winpthread)

## Running on Windows

### From WSL/Linux
```bash
cd build-mingw
bin/odaslive.exe -c ../config/odaslive/tetrahedral_4ch-b.cfg -v
```

### On Native Windows
1. Copy entire `build-mingw/bin/` directory to Windows
2. Copy your config file (e.g., `config/odaslive/tetrahedral_4ch-b.cfg`)
3. Run: `odaslive.exe -c tetrahedral_4ch-b.cfg -v`

## Configuration

ODAS uses WASAPI for Windows audio input. Example config:

```cfg
raw: {
    fS = 44100;
    hopSize = 512;
    nBits = 16;
    nChannels = 4;

    interface: {
        type = "wasapi";
        devicename = "Teensy Audio";
    }
}
```

Change `devicename` to match your audio device as shown in Windows Sound Control Panel.

## Build Scripts

Build scripts are located in `../build-scripts/`:

- **build_mingw.sh** - Main Windows build script (MinGW cross-compilation)
- **toolchain-mingw.cmake** - CMake toolchain file for MinGW
- **build_fftw_mingw.sh** - FFTW dependency build (if needed)
- **build_windows.sh** - Alternative build script

## Build Options

```bash
# Clean build from scratch
./build-scripts/build_mingw.sh --clean

# Rebuild dependencies
./build-scripts/build_mingw.sh --rebuild-deps

# Show help
./build-scripts/build_mingw.sh --help
```

## Documentation Files

- **README.md** (this file) - Overview and quick links
- **[QUICKSTART_WINDOWS.md](QUICKSTART_WINDOWS.md)** - Quick start guide
- **[BUILD_WINDOWS.md](BUILD_WINDOWS.md)** - Detailed build instructions
- **[DIRECTORY_STRUCTURE.md](DIRECTORY_STRUCTURE.md)** - Complete directory structure reference
- **[CLEANUP.md](CLEANUP.md)** - Cleanup guide for old build artifacts

## Directory Structure

See [DIRECTORY_STRUCTURE.md](DIRECTORY_STRUCTURE.md) for complete directory structure documentation.

Quick overview:
```
odas/
├── build-scripts/           # Build scripts and toolchain files
├── docs/                    # Documentation (this directory)
├── build-mingw/             # Windows build output (auto-generated)
├── config/                  # Configuration files
├── src/                     # ODAS source code
└── include/                 # ODAS headers
```

## Troubleshooting

See [BUILD_WINDOWS.md](BUILD_WINDOWS.md) for detailed troubleshooting information.

Common issues:
- **Missing DLL errors**: Ensure all DLLs are in same directory as executable
- **WASAPI device not found**: Check device name in Windows Sound Control Panel
- **Import library errors**: Run `./build-scripts/build_mingw.sh --clean`

## Windows Features

- **WASAPI Support**: Low-latency multi-channel audio capture
- **Cross-platform Sockets**: Winsock2 for network communication
- **Threading**: winpthread (POSIX threads for Windows)

## Next Steps

1. Read [QUICKSTART_WINDOWS.md](QUICKSTART_WINDOWS.md) for a quick start
2. Read [BUILD_WINDOWS.md](BUILD_WINDOWS.md) for detailed information
3. Check `../config/odaslive/` for example configuration files
4. Visit the [ODAS GitHub wiki](https://github.com/introlab/odas/wiki) for more information
