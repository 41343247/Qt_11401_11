# Qt_11401_11

## Minesweeper Collection - Two Game Modes

This project contains two distinct minesweeper games with unique 3D gameplay mechanics.

### 🎮 Game 1: Multi-Layer Minesweeper

Classic minesweeper extended into multiple layers, creating a true 3D experience.

#### Key Features
- **Layer-Specific Color Coding**: Each layer has a unique color scheme (10 different colors)
- **Unrevealed cells**: Darker shade for better visibility
- **Revealed cells**: Lighter shade to clearly distinguish opened areas
- **Explosion Effect**: Cell flashes red with yellow border and displays 💥
- **Color-coded Numbers**: 1-8 mine counts with distinct colors
- **Audio System**: Sound effects for explosions and wins
- **Modern UI**: Dark theme with styled buttons and hover effects

#### How to Play
1. Click cells to reveal them
2. Right-click to place flags on suspected mines
3. Use layer navigation to explore 3D space
4. Win by revealing all safe cells

### 🎲 Game 2: Multi-Face Minesweeper (3D Hollow Polyhedra)

Unique minesweeper on the surface of 3D polyhedra - a truly innovative gameplay experience!

#### Three Difficulty Levels

**🔺 Easy: Tetrahedron**
- 4 triangular faces
- 5×5 grid per face
- 8 mines total
- Each face adjacent to 3 others

**🔷 Medium: Cube**
- 6 square faces  
- 6×6 grid per face
- 15 mines total
- Each face adjacent to 4 others

**⬟ Hard: Dodecahedron**
- 12 pentagonal faces
- 5×5 grid per face
- 25 mines total
- Each face adjacent to 5 others

#### Unique Features

**Adjacent Face Previews** - The killer feature!
- Right panel shows mini-grids of all adjacent faces
- See revealed cells and numbers on neighboring faces
- Non-interactive views update automatically
- Helps understand 3D spatial relationships
- Reduces need for constant face switching

**Edge-Adjacency Only**
- Only cells sharing an edge are considered adjacent
- No diagonal (vertex) adjacency
- More strategic gameplay

**Face Navigation**
- Thumbnail panel on left shows all faces
- Click any thumbnail to switch to that face
- Color-coded for easy identification
- Main view shows interactive grid

#### How to Play
1. Select difficulty (Easy/Medium/Hard)
2. Click cells to reveal on current face
3. Right-click to flag suspected mines
4. Use face thumbnails to switch views
5. Check adjacent face previews for hints
6. Win by revealing all non-mine cells

### 🛠️ Building the Project

```bash
qmake EndOfTerm_test.pro
make
./EndOfTerm_test
```

### 📝 Technical Details
- **Framework**: Qt 5/6 with Widgets and Multimedia
- **Language**: C++17
- **Architecture**: Model-View design with proper separation
- **Features**: 
  - Multi-layer 3D minesweeper (Game 1)
  - Multi-face hollow polyhedra minesweeper (Game 2)
  - Sound effects and visual feedback
  - Advanced UI with preview panels

### 📚 Documentation

For detailed information about Game 2:
- **GAME2_IMPLEMENTATION.md**: Complete technical documentation
- **GAME2_UI_MOCKUP.md**: Visual guide with UI mockups
- **GAME2_SUMMARY.md**: Implementation summary and compliance checklist

### 🎯 Game Selection

When you launch the application:
1. Choose between Game 1 (Multi-Layer) or Game 2 (Multi-Face)
2. For Game 2, select difficulty level
3. Start playing and enjoy!
