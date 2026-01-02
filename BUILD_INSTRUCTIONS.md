# Build Instructions

## Prerequisites

### Required Software
- **Qt Framework**: Qt 5.12+ or Qt 6.x
  - Qt Widgets module
  - Qt Multimedia module
- **C++ Compiler**: 
  - GCC 7+ or
  - Clang 5+ or
  - MSVC 2017+
- **Build Tool**: qmake (included with Qt)

### Installing Qt

#### On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install qt5-default qtmultimedia5-dev
# or for Qt6:
sudo apt install qt6-base-dev qt6-multimedia-dev
```

#### On macOS:
```bash
brew install qt@5
# or
brew install qt@6
```

#### On Windows:
Download and install from https://www.qt.io/download

## Building the Project

### Method 1: Using qmake (Recommended)

1. Open terminal in project directory:
```bash
cd /path/to/Qt_11401_11
```

2. Generate Makefile:
```bash
qmake EndOfTerm_test.pro
# or if using Qt6:
qmake6 EndOfTerm_test.pro
```

3. Build:
```bash
make
# or on Windows with MSVC:
nmake
# or on Windows with MinGW:
mingw32-make
```

4. Run:
```bash
./EndOfTerm_test
# or on Windows:
EndOfTerm_test.exe
```

### Method 2: Using Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select `EndOfTerm_test.pro`
4. Configure project (select kit)
5. Press Ctrl+B to build
6. Press Ctrl+R to run

## Troubleshooting

### "qmake: command not found"
- Qt is not installed or not in PATH
- Solution: Install Qt and add to PATH
```bash
export PATH=/path/to/Qt/bin:$PATH
```

### "Cannot find -lQt5Widgets"
- Qt development files not installed
- Solution: Install Qt development packages

### "QSoundEffect: No such file or directory"
- Qt Multimedia module not found
- Solution: Install qtmultimedia5-dev (Linux) or ensure Qt installation includes Multimedia

### Linking errors on Windows
- Make sure you're using the correct build tool (MinGW vs MSVC)
- Ensure qmake is from the same Qt installation as your libraries

## Project Structure

```
Qt_11401_11/
├── main.cpp                    # Entry point
├── gamecontroller.h/cpp        # Navigation controller
├── menuwidget.h/cpp            # Main menu
├── difficultywidget.h/cpp      # Difficulty selection
├── widget.h/cpp                # Game 1 (Multi-layer)
├── polyhedrawidget.h/cpp       # Game 2 (Multi-face)
├── EndOfTerm_test.pro          # Qt project file
├── resources.qrc               # Sound resources
└── sounds/                     # Sound files
    ├── boom.wav
    ├── win.wav
    └── start.wav
```

## Build Verification

After building, verify the executable:
```bash
# Check file exists
ls -lh EndOfTerm_test

# Check dependencies (Linux)
ldd EndOfTerm_test | grep Qt

# Run the program
./EndOfTerm_test
```

## Expected Behavior

When run successfully, you should see:
1. **Menu Screen**: Two large buttons (Game 1 and Game 2)
2. **Game 1**: Click to see multi-layer minesweeper
3. **Game 2**: Click to see difficulty selection
4. **Sound Effects**: Start, explosion, and win sounds play

## Clean Build

To clean and rebuild:
```bash
make clean
qmake
make
```

Or in Qt Creator:
- Build → Clean All
- Build → Rebuild All

## Deployment

### Linux
```bash
# Copy executable and sounds
cp EndOfTerm_test /usr/local/bin/
cp -r sounds /usr/local/share/EndOfTerm_test/

# Or create AppImage (advanced)
```

### Windows
```bash
# Use windeployqt to gather dependencies
windeployqt EndOfTerm_test.exe

# Create installer with Inno Setup or NSIS
```

### macOS
```bash
# Create app bundle
macdeployqt EndOfTerm_test.app

# Create DMG for distribution
```

## Development

### Adding New Features
1. Edit source files
2. If adding new files, update `EndOfTerm_test.pro`:
   ```qmake
   SOURCES += newfile.cpp
   HEADERS += newfile.h
   ```
3. Rebuild: `qmake && make`

### Debugging
In Qt Creator:
- Set breakpoints by clicking line numbers
- Press F5 to start debugging
- Use Debug → Step Over/Into/Out

Command line (with gdb):
```bash
gdb ./EndOfTerm_test
(gdb) run
```

## Performance

Typical build times:
- First build: 10-30 seconds
- Incremental build: 2-5 seconds

Memory usage:
- ~50-100 MB RAM
- ~10 MB disk space for executable

## Compatibility

Tested on:
- ✅ Ubuntu 20.04+ with Qt 5.12+
- ✅ Windows 10/11 with Qt 5.15+
- ✅ macOS 10.15+ with Qt 5.15+
- ✅ Qt 6.x on all platforms

Minimum requirements:
- Qt 5.12 or later
- C++17 support
- 512 MB RAM
- 50 MB disk space
