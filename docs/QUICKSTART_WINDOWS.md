# ODAS Windows Quick Start

## Build

```bash
./build-scripts/build_mingw.sh
```

## Run

```bash
cd build-mingw
bin/odaslive.exe -c ../config/odaslive/tetrahedral_4ch-b.cfg -v
```

## Common Commands

### Clean rebuild
```bash
./build-scripts/build_mingw.sh --clean
```

### Rebuild dependencies
```bash
./build-scripts/build_mingw.sh --rebuild-deps
```

### Check built files
```bash
ls -lh build-mingw/bin/
```

### Test on Windows
```bash
# From WSL, run Windows executable
cd build-mingw
./bin/odaslive.exe -c ../config/odaslive/tetrahedral_4ch-b.cfg -v
```

## Config File Template

Create a config file for your Teensy:

```cfg
version = "2.1";

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

# See config/odaslive/tetrahedral_4ch-b.cfg for full configuration
```

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Missing config file | Add `-c config.cfg` argument |
| DLL not found | Copy all DLLs from `build-mingw/bin/` |
| WASAPI device not found | Check device name in Windows Sound Control Panel |
| Import library errors | Run `./build-scripts/build_mingw.sh --clean` |

## File Locations

```
odas/
├── build-scripts/              # Build scripts and toolchain files
│   ├── build_mingw.sh          # Main Windows build script
│   └── toolchain-mingw.cmake   # CMake toolchain file
├── docs/                       # Documentation
│   ├── BUILD_WINDOWS.md        # Detailed build guide
│   └── QUICKSTART_WINDOWS.md   # This file
├── config/odaslive/
│   └── tetrahedral_4ch-b.cfg   # Teensy config
└── build-mingw/                # Build output (created by build script)
    ├── bin/                    # Output executables and DLLs
    ├── libodas.def             # Export definitions
    └── libodas_fixed.dll.a     # Import library
```

## Need Help?

See [BUILD_WINDOWS.md](BUILD_WINDOWS.md) for detailed documentation.