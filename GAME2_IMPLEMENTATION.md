# Game 2 Implementation: 3D Hollow Minesweeper

## Overview
Game 2 is a 3D hollow minesweeper game with three difficulty levels based on different polyhedra. This implementation includes a critical feature for helping players visualize the 3D structure: **Adjacent Face Previews**.

## Difficulty Levels

### Easy: Tetrahedron (4 Faces)
- **Faces**: 4 triangular faces
- **Grid Size**: 5x5 per face
- **Mines**: 8
- **Adjacency**: Each face is adjacent to 3 other faces

### Medium: Cube (6 Faces)
- **Faces**: 6 square faces
- **Grid Size**: 6x6 per face
- **Mines**: 15
- **Adjacency**: 
  - Front/Back faces: adjacent to Left, Right, Top, Bottom
  - Left/Right faces: adjacent to Front, Back, Top, Bottom
  - Top/Bottom faces: adjacent to Front, Back, Left, Right

### Hard: Dodecahedron (12 Faces)
- **Faces**: 12 pentagonal faces (using internal grid representation)
- **Grid Size**: 5x5 per face
- **Mines**: 25
- **Adjacency**: Each face is adjacent to 5 other faces

## Key Features

### 1. Edge-Adjacency Only
All cell adjacency calculations use **edge-adjacency only** (4-directional), not vertex-adjacency (8-directional). This is implemented in `getEdgeAdjacentCells()`:

```cpp
QVector<QPair<int, int>> PolyhedraWidget::getEdgeAdjacentCells(int face, int r, int c) const
{
    QVector<QPair<int, int>> neighbors;
    
    // Only consider 4-directional (edge) adjacency within the same face
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < rowsPerFace && nc >= 0 && nc < colsPerFace) {
            neighbors.append({nr, nc});
        }
    }
    
    return neighbors;
}
```

### 2. Face Thumbnails (Left Panel)
- Shows all faces with corresponding colors
- Click to switch the main view to that face
- Each thumbnail is labeled with the face number (F0, F1, etc.)
- Color-coded to match the main face view

### 3. Adjacent Face Previews (Right Panel) - NEW FEATURE
This is the most important new feature that helps players understand the 3D structure.

#### What It Shows:
- **Mini-grids** of all faces adjacent to the current face
- **Read-only view** (non-interactive) showing:
  - Revealed cells with their mine counts
  - Unrevealed cells (darker background)
  - Mine symbols (💣) if revealed
  - Color-coded numbers based on adjacent mine count
- **Face labels** identifying which face each preview represents

#### Implementation Details:

##### Data Structure:
```cpp
struct AdjacentFacePreview {
    int faceIndex;              // Which face this preview represents
    QFrame *frame;              // Container frame
    QLabel *label;              // Face identifier label
    QGridLayout *gridLayout;    // Layout for cell grid
    QVector<QVector<QLabel*>> cells;  // The cell labels
};
QVector<AdjacentFacePreview> adjacentPreviews;
```

##### Key Functions:

1. **buildAdjacentPreviews()**: Called when switching faces
   - Clears existing previews
   - Creates new preview for each adjacent face
   - Sets up mini-grids with proper colors
   - Adds to the right panel layout

2. **updateAdjacentPreview(int adjacentFaceIdx)**: Updates a specific adjacent face
   - Iterates through all cells in that face
   - Updates cell appearance based on revealed state
   - Shows numbers, mines, or blank cells as appropriate

3. **updateAllAdjacentPreviews()**: Batch update all previews
   - Called after initial creation
   - Updates all adjacent face previews at once

##### When Previews Are Updated:
- **New Game**: Initial setup with all cells unrevealed
- **Switch Face**: Rebuilds previews for the new current face's adjacent faces
- **Reveal Cell**: Updates preview if the revealed cell is on an adjacent face
- **Flood Fill**: Updates all adjacent face previews that had cells revealed

### 4. UI Layout

```
┌─────────────────────────────────────────────────────────┐
│  Controls: [Back] [New Game] [Hint]  Mines: X  Time: Y │
├──────┬──────────────────────────────────────┬───────────┤
│      │                                      │ Adjacent  │
│ Face │         Main Face View               │  Faces    │
│ Thmb │         (Interactive)                │ Preview   │
│      │                                      │ (F1)      │
│ [F0] │     ┌─┬─┬─┬─┬─┬─┐                   │ ┌─┬─┬─┐   │
│ [F1] │     ├─┼─┼─┼─┼─┼─┤                   │ ├─┼─┼─┤   │
│ [F2] │     ├─┼─┼─┼─┼─┼─┤                   │ └─┴─┴─┘   │
│ [F3] │     ├─┼─┼─┼─┼─┼─┤                   │           │
│ [F4] │     ├─┼─┼─┼─┼─┼─┤                   │ (F2)      │
│ [F5] │     ├─┼─┼─┼─┼─┼─┤                   │ ┌─┬─┬─┐   │
│      │     └─┴─┴─┴─┴─┴─┘                   │ ├─┼─┼─┤   │
│      │                                      │ └─┴─┴─┘   │
│      │     Face: 0                          │           │
└──────┴──────────────────────────────────────┴───────────┘
```

## Game Flow

1. **Start Game**: 
   - Select difficulty level (Easy/Medium/Hard)
   - Face 0 is shown by default
   - Adjacent face previews are built for faces adjacent to Face 0

2. **First Click**:
   - Mines are placed safely (avoiding click location and neighbors)
   - Adjacent mine counts are calculated
   - Timer starts

3. **Playing**:
   - Left-click to reveal cells
   - Right-click to flag suspected mines
   - Click face thumbnails to switch views
   - Adjacent face previews update automatically as cells are revealed

4. **Win Condition**:
   - All non-mine cells are revealed
   - Victory dialog shows elapsed time

5. **Lose Condition**:
   - Hit a mine
   - All mines are revealed
   - Game over dialog appears

## Technical Implementation Notes

### Color Scheme
Each face has a unique color:
- **Tetrahedron**: Red, Green, Blue, Yellow
- **Cube**: Red, Orange, Yellow, Green, Blue, Purple
- **Dodecahedron**: 12 distinct colors spanning the spectrum

Colors are used in:
- Face thumbnails
- Main face grid (darker shade for unrevealed, lighter for revealed)
- Adjacent face preview borders (matching face color)

### Performance Optimization
- Previews only show cells that are adjacent to the current face
- Updates are incremental (only affected previews are updated)
- Mini-grids use small cell sizes (8-12 pixels) to fit multiple previews

### User Experience
- Non-interactive previews prevent accidental clicks
- Clear labeling (Face 0, Face 1, etc.) helps orientation
- Color consistency between thumbnails, main view, and previews
- Scroll areas handle different polyhedra with varying face counts

## Files Modified

1. **polyhedrawidget.h**:
   - Added `AdjacentFacePreview` struct
   - Added preview widget members
   - Added preview management functions

2. **polyhedrawidget.cpp**:
   - Modified `buildUI()` to add right panel for previews
   - Implemented `buildAdjacentPreviews()`
   - Implemented `updateAdjacentPreview()`
   - Implemented `updateAllAdjacentPreviews()`
   - Modified `switchToFace()` to rebuild previews
   - Modified `revealCell()` to update previews
   - Modified `revealNeighborsIfEmpty()` to batch-update previews
   - Modified `newGame()` to initialize previews

## Testing Recommendations

1. **Easy Level (Tetrahedron)**:
   - Each face should show 3 adjacent face previews
   - Verify color consistency across all views
   - Test switching between all 4 faces

2. **Medium Level (Cube)**:
   - Each face should show 4 adjacent face previews
   - Test that opposite faces (front/back) show correct adjacency
   - Verify edge cells don't incorrectly affect non-adjacent faces

3. **Hard Level (Dodecahedron)**:
   - Each face should show 5 adjacent face previews (except face 11)
   - Verify complex adjacency relationships
   - Test scrolling in the preview panel

4. **Cross-Face Revealing**:
   - When revealing cells creates a large empty area
   - Verify that adjacent face previews update correctly
   - Check that revealed numbers appear in previews

## Benefits of Adjacent Face Previews

1. **Spatial Understanding**: Players can see what's happening on adjacent faces without switching views
2. **Strategic Planning**: Make informed decisions based on adjacent face information
3. **Reduced Context Switching**: Less need to constantly switch between faces
4. **Learning Aid**: Helps players understand the 3D structure of the polyhedron
5. **Accessibility**: Visual hints reduce cognitive load of tracking 3D relationships

## Compliance with Requirements

✅ **Edge-adjacency only**: Implemented correctly, no vertex adjacency
✅ **Face thumbnails**: Left panel shows all faces with colors
✅ **Main face view**: Interactive grid for the current face
✅ **Adjacent face previews**: Non-interactive views showing revealed states and numbers
✅ **Face switching**: Click thumbnails to switch main view
✅ **Three difficulty levels**: Tetrahedron, Cube, Dodecahedron properly configured
✅ **Hollow 3D structure**: Each polyhedron correctly implements face adjacency relationships
