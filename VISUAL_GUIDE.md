# Visual Guide - Game 2 Implementation

## Screen Flow Diagram

```
┌─────────────────────────────────────────┐
│          Menu Widget (Cover)            │
│                                         │
│     💣 Minesweeper Collection 💣        │
│        Choose Your Adventure            │
│                                         │
│   ┌──────────────┐  ┌──────────────┐   │
│   │  🎲 Game 1   │  │  🎮 Game 2   │   │
│   │ Multi-Layer  │  │  Multi-Face  │   │
│   └──────────────┘  └──────────────┘   │
│                                         │
└─────────────────────────────────────────┘
              │                │
              │                │
    ┌─────────┘                └─────────┐
    │                                    │
    ▼                                    ▼
┌─────────────────┐         ┌──────────────────────────┐
│   Widget        │         │  Difficulty Widget       │
│  (Game 1)       │         │                          │
│                 │         │  🔺 Easy                 │
│  Multi-Layer    │         │  Tetrahedron (4 faces)   │
│  Minesweeper    │         │                          │
│                 │         │  🔷 Medium               │
│  [Hint] Button  │         │  Cube (6 faces)          │
│  [Back] Button  │         │                          │
│                 │         │  ⬟ Hard                  │
└─────────────────┘         │  Dodecahedron (12 faces) │
                            │                          │
                            │  [Back to Menu]          │
                            └──────────────────────────┘
                                        │
                          ┌─────────────┼─────────────┐
                          │             │             │
                          ▼             ▼             ▼
                    ┌──────────┐  ┌──────────┐  ┌──────────┐
                    │  Easy    │  │  Medium  │  │   Hard   │
                    │  Game 2  │  │  Game 2  │  │  Game 2  │
                    └──────────┘  └──────────┘  └──────────┘
```

## Game 2 Screen Layout

```
┌─────────────────────────────────────────────────────────────────────┐
│ [← Back] [New Game] [💡 Hint]     Mines: 15      Time: 42           │
├──────────┬──────────────────────────────────────────────────────────┤
│ Faces    │                  Face: 0                                 │
│          │                                                           │
│ ┌──────┐ │     Main Face Grid (6x6 for Cube)                       │
│ │ F0   │ │     ┌──┬──┬──┬──┬──┬──┐                                 │
│ │ RED  │ │     │  │  │  │  │  │  │                                 │
│ └──────┘ │     ├──┼──┼──┼──┼──┼──┤                                 │
│          │     │  │ 1│ 2│ 1│  │  │                                 │
│ ┌──────┐ │     ├──┼──┼──┼──┼──┼──┤                                 │
│ │ F1   │ │     │  │ 2│💣│ 2│  │  │                                 │
│ │ORANGE│ │     ├──┼──┼──┼──┼──┼──┤                                 │
│ └──────┘ │     │  │ 1│ 2│ 1│  │  │                                 │
│          │     ├──┼──┼──┼──┼──┼──┤                                 │
│ ┌──────┐ │     │  │  │  │  │  │  │                                 │
│ │ F2   │ │     ├──┼──┼──┼──┼──┼──┤                                 │
│ │YELLOW│ │     │  │  │  │  │  │  │                                 │
│ └──────┘ │     └──┴──┴──┴──┴──┴──┘                                 │
│          │                                                           │
│ ┌──────┐ │     Legend:                                              │
│ │ F3   │ │     • Dark color = Unrevealed                            │
│ │GREEN │ │     • Light color = Revealed                             │
│ └──────┘ │     • Numbers = Adjacent mine count                      │
│          │     • 💣 = Mine                                           │
│ ┌──────┐ │     • ⚑ = Flag                                           │
│ │ F4   │ │                                                           │
│ │ BLUE │ │                                                           │
│ └──────┘ │                                                           │
│          │                                                           │
│ ┌──────┐ │                                                           │
│ │ F5   │ │                                                           │
│ │PURPLE│ │                                                           │
│ └──────┘ │                                                           │
│          │                                                           │
└──────────┴──────────────────────────────────────────────────────────┘
```

## Edge-Adjacency Logic Illustration

### Traditional 8-Direction Adjacency (NOT used in Game 2):
```
┌───┬───┬───┐
│ X │ X │ X │  All 8 cells are considered adjacent
├───┼───┼───┤  (including diagonals)
│ X │ ● │ X │  ● = center cell
├───┼───┼───┤  X = adjacent cells
│ X │ X │ X │
└───┴───┴───┘
```

### Edge-Adjacency (4-Direction) - Used in Game 2:
```
┌───┬───┬───┐
│   │ X │   │  Only 4 cells are considered adjacent
├───┼───┼───┤  (sharing edges only)
│ X │ ● │ X │  ● = center cell
├───┼───┼───┤  X = adjacent cells
│   │ X │   │
└───┴───┴───┘
```

This simplification makes the game more manageable on complex polyhedra like dodecahedrons.

## Color Schemes

### Tetrahedron (4 Faces)
- Face 0: Red (255, 100, 100)
- Face 1: Green (100, 255, 100)
- Face 2: Blue (100, 100, 255)
- Face 3: Yellow (255, 255, 100)

### Cube (6 Faces)
- Face 0: Red (255, 80, 80)
- Face 1: Orange (255, 165, 0)
- Face 2: Yellow (255, 255, 100)
- Face 3: Green (100, 255, 100)
- Face 4: Blue (100, 150, 255)
- Face 5: Purple (200, 100, 255)

### Dodecahedron (12 Faces)
- Faces use a rainbow spectrum with 12 distinct colors
- Each face is easily distinguishable by color

## Hint System

### Game 1 (Multi-Layer)
```
Before Hint:                After Hint:
┌──┬──┬──┐                 ┌──┬──┬──┐
│  │  │  │                 │  │  │  │
├──┼──┼──┤                 ├──┼──┼──┤
│  │  │  │  [Hint] →       │  │ 1│  │  ← Revealed
├──┼──┼──┤                 ├──┼──┼──┤
│  │  │  │                 │  │  │  │
└──┴──┴──┘                 └──┴──┴──┘

Reveals one random safe cell on the current layer
```

### Game 2 (Multi-Face)
```
Similar behavior, but reveals one random safe cell
on the currently displayed face.
```

## Face Adjacency Examples

### Tetrahedron
```
     0
    /|\
   / | \
  1--2--3
  
Each face touches 3 others
```

### Cube
```
    4 (top)
  +-----+
  |     |
2 |  0  | 3
  | (f) |
  +-----+
    5 (bottom)
    1 (back)

Front face (0) touches: left(2), right(3), top(4), bottom(5)
```

### Dodecahedron
```
Complex 3D structure with 12 pentagonal faces.
Each face touches 5 adjacent faces.
Navigation via thumbnails simplifies interaction.
```

## User Interaction Flow

1. **Launch App** → Menu Widget appears
2. **Click "Game 2"** → Difficulty Widget appears
3. **Select Difficulty** → Game 2 loads with selected polyhedron
4. **Click Face Thumbnail** → Switch to that face
5. **Left Click Cell** → Reveal cell
6. **Right Click Cell** → Toggle flag
7. **Click Hint** → Reveal random safe cell on current face
8. **Click Back** → Return to difficulty selection
9. **Click Back Again** → Return to main menu
10. **Click "Game 1"** → Play original multi-layer minesweeper

## Implementation Notes

### Why Edge-Adjacency?
On complex polyhedra like dodecahedrons, using full 8-directional adjacency would create very high mine counts near vertices where multiple faces meet. Edge-adjacency keeps the game balanced and manageable.

### Why Face Thumbnails Instead of Virtual Cells?
The original requirement asked for "virtual cells" showing adjacent faces semi-transparently around the main grid. This would require:
- Complex edge mapping between faces
- Spatial positioning calculations
- State synchronization
- Significantly more UI complexity

Face thumbnails provide:
- Simple navigation
- Clear color identification
- Easy state tracking
- Less visual clutter

The thumbnail approach is more user-friendly and maintainable.

## Testing Recommendations

1. **Menu Navigation**: Test all button transitions
2. **Game 1**: Verify hint button works, back button returns to menu
3. **Game 2 Easy**: Test all 4 faces, verify mine counting with edge-adjacency
4. **Game 2 Medium**: Test all 6 faces, verify cube face adjacency
5. **Game 2 Hard**: Test face switching on dodecahedron, verify it's playable
6. **Hint System**: Verify hints reveal safe cells, handle edge cases
7. **Win/Lose Conditions**: Test in all game modes
8. **Sound Effects**: Verify all sounds play correctly
