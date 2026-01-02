# Game 2 UI Mockup - Visual Guide

## Main Window Layout

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║ ← Back    [New Game]   💡 Hint        Mines: 15    Time: 42                  ║
╠═══════╦═══════════════════════════════════════════════╦═══════════════════════╣
║       ║                                               ║  Adjacent Faces       ║
║ Faces ║              Face: 0                          ║                       ║
║       ║                                               ║ ┌──────────────────┐  ║
║ ┌───┐ ║     ┌──┬──┬──┬──┬──┬──┐                     ║ │ Face 2           │  ║
║ │ 0 │ ║     │  │  │  │  │  │  │                     ║ │ ┌─┬─┬─┬─┬─┬─┐   │  ║
║ └───┘ ║     ├──┼──┼──┼──┼──┼──┤                     ║ │ │ │1│1│ │ │ │   │  ║
║       ║     │  │1 │1 │  │  │  │                     ║ │ ├─┼─┼─┼─┼─┼─┤   │  ║
║ ┌───┐ ║     ├──┼──┼──┼──┼──┼──┤                     ║ │ │ │2│💣│1│ │ │   │  ║
║ │ 1 │ ║     │  │2 │💣│1 │  │  │                     ║ │ └─┴─┴─┴─┴─┴─┘   │  ║
║ └───┘ ║     ├──┼──┼──┼──┼──┼──┤                     ║ └──────────────────┘  ║
║       ║     │  │3 │2 │2 │  │  │                     ║                       ║
║ ┌───┐ ║     ├──┼──┼──┼──┼──┼──┤                     ║ ┌──────────────────┐  ║
║ │ 2 │ ║     │  │⚑│  │  │  │  │                     ║ │ Face 3           │  ║
║ └───┘ ║     ├──┼──┼──┼──┼──┼──┤                     ║ │ ┌─┬─┬─┬─┬─┬─┐   │  ║
║       ║     │  │  │  │  │  │  │                     ║ │ │ │ │ │ │ │ │   │  ║
║ ┌───┐ ║     └──┴──┴──┴──┴──┴──┘                     ║ │ ├─┼─┼─┼─┼─┼─┤   │  ║
║ │ 3 │ ║                                               ║ │ │ │ │ │ │ │ │   │  ║
║ └───┘ ║       Legend:                                ║ │ └─┴─┴─┴─┴─┴─┘   │  ║
║       ║       [Dark] = Unrevealed                    ║ └──────────────────┘  ║
║ ┌───┐ ║       [Light] = Revealed                     ║                       ║
║ │ 4 │ ║       💣 = Mine                               ║ ┌──────────────────┐  ║
║ └───┘ ║       1-8 = Adjacent mine count              ║ │ Face 4           │  ║
║       ║       ⚑ = Flagged                            ║ │ ┌─┬─┬─┬─┬─┬─┐   │  ║
║ ┌───┐ ║                                               ║ │ │ │ │ │ │ │ │   │  ║
║ │ 5 │ ║                                               ║ │ ├─┼─┼─┼─┼─┼─┤   │  ║
║ └───┘ ║                                               ║ │ │ │1│1│ │ │ │   │  ║
║       ║                                               ║ │ └─┴─┴─┴─┴─┴─┘   │  ║
║       ║                                               ║ └──────────────────┘  ║
║       ║                                               ║                       ║
║       ║                                               ║ ┌──────────────────┐  ║
║       ║                                               ║ │ Face 5           │  ║
║       ║                                               ║ │ ┌─┬─┬─┬─┬─┬─┐   │  ║
║       ║                                               ║ │ │ │ │ │ │ │ │   │  ║
║       ║                                               ║ │ ├─┼─┼─┼─┼─┼─┤   │  ║
║       ║                                               ║ │ │ │ │ │ │ │ │   │  ║
║       ║                                               ║ │ └─┴─┴─┴─┴─┴─┘   │  ║
║       ║                                               ║ └──────────────────┘  ║
╚═══════╩═══════════════════════════════════════════════╩═══════════════════════╝
```

## Color Scheme Examples

### Tetrahedron (4 Faces)
```
Face 0: Red     (#FF6464)
Face 1: Green   (#64FF64)
Face 2: Blue    (#6464FF)
Face 3: Yellow  (#FFFF64)
```

### Cube (6 Faces)
```
Face 0: Red     (#FF5050) - Front
Face 1: Orange  (#FFA500) - Back
Face 2: Yellow  (#FFFF64) - Left
Face 3: Green   (#64FF64) - Right
Face 4: Blue    (#6496FF) - Top
Face 5: Purple  (#C864FF) - Bottom
```

### Dodecahedron (12 Faces)
```
Face 0:  Red         (#FF6464)
Face 1:  Red-Orange  (#FFB464)
Face 2:  Yellow      (#FFFF64)
Face 3:  Yellow-Grn  (#B4FF64)
Face 4:  Green       (#64FF64)
Face 5:  Green-Cyan  (#64FFB4)
Face 6:  Cyan        (#64FFFF)
Face 7:  Cyan-Blue   (#64B4FF)
Face 8:  Blue        (#6464FF)
Face 9:  Blue-Purple (#B464FF)
Face 10: Purple      (#FF64FF)
Face 11: Purple-Red  (#FF64B4)
```

## Adjacent Face Preview Details

### Size and Appearance
- **Cell Size**: 8-12 pixels (adaptive based on grid size)
- **Border**: 2px solid #555
- **Background**: Face color (darker shade)
- **Text**: Small font (8px) for numbers
- **Non-interactive**: Labels only, no click handlers

### Information Displayed
- **Unrevealed cells**: Dark background matching face color
- **Revealed empty cells**: Light background
- **Revealed number cells**: Light background with colored number
  - 1 = Blue (#0000FF)
  - 2 = Green (#008000)
  - 3 = Red (#FF0000)
  - 4 = Navy (#000080)
- **Revealed mines**: 💣 symbol on light background

### Preview Panel Features
- **Scrollable**: Can handle many adjacent faces (up to 5 for dodecahedron)
- **Labeled**: Each preview shows "Face X" at the top
- **Grouped**: All adjacent faces shown together
- **Real-time updates**: Changes immediately when cells are revealed

## Interactive Flow

### 1. Starting a Game
```
[Menu] → [Game 2] → [Select Difficulty]
   ↓
[Tetrahedron] or [Cube] or [Dodecahedron]
   ↓
Main game window opens with:
- Face 0 displayed in center
- All face thumbnails on left
- Adjacent face previews on right (showing neighbors of Face 0)
```

### 2. Playing the Game
```
Left Click on Cell → Reveal cell
   ↓
If mine → Game Over, show all mines
If safe → Show adjacent mine count
   ↓
If count = 0 → Flood fill reveal neighbors
   ↓
Update adjacent face previews (if affected)
```

```
Right Click on Cell → Toggle flag
   ↓
Update mine counter
```

```
Click Face Thumbnail → Switch to that face
   ↓
Rebuild main grid
   ↓
Rebuild adjacent face previews
```

### 3. Adjacent Face Preview Updates
```
Reveal cell on current face → Update main grid
Reveal cell on adjacent face → Update that face's preview
Flood fill across faces → Update all affected previews
```

## Example Play Session (Cube)

**Initial State:**
- Face 0 selected (Front face)
- Adjacent faces shown: 2 (Left), 3 (Right), 4 (Top), 5 (Bottom)
- All cells unrevealed

**First Click:**
- Click cell [3,3] on Face 0
- Safe cell with count 2
- Main grid updates to show "2"
- Adjacent previews remain dark (nothing revealed on them yet)

**Second Click:**
- Click cell [2,3] on Face 0
- Safe cell with count 0
- Flood fill reveals surrounding empty area
- Some cells on Face 0 revealed
- If flood reaches Face 2, that preview updates

**Switch Face:**
- Click Face 2 thumbnail
- Main grid rebuilds showing Face 2
- Adjacent previews rebuild showing: 0 (Front), 1 (Back), 4 (Top), 5 (Bottom)
- Previously revealed cells on Face 0 now visible in preview

## Benefits Visualization

### Without Adjacent Previews:
```
Player needs to remember:
- What was revealed on Face A
- What was revealed on Face B
- How Face A connects to Face B
- Which cells are at the edge

Result: Lots of face switching, mental overhead
```

### With Adjacent Previews:
```
Player can see:
✓ Current face cells (interactive)
✓ Adjacent face cells (read-only preview)
✓ Which faces connect where
✓ Numbers on adjacent faces for planning

Result: Better spatial understanding, faster gameplay
```

## Technical Notes

### Preview Update Optimization
- Only update previews when their face has changes
- Batch updates during flood fill operations
- Use QSet to track which faces need updates

### Memory Efficiency
- Previews created on-demand when switching faces
- Old previews destroyed before creating new ones
- Labels are lightweight compared to buttons

### User Experience
- Clear visual distinction: buttons vs labels
- Consistent color coding across all views
- Smooth face switching with instant preview updates
- Responsive to game state changes
