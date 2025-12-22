# Qt_11401_11

## Multi-Layer Minesweeper - Enhanced Edition

### 🎮 Game Enhancements (mainplus branch)

#### 1. **Layer-Specific Color Coding**
- Each layer has a unique color scheme (10 different colors)
- **Unrevealed cells**: Darker shade for better visibility
- **Revealed cells**: Lighter shade to clearly distinguish opened areas
- Color palette includes: Steel Blue, Sienna, Dark Olive Green, and more

#### 2. **Visual Feedback**
- **Explosion Effect**: When hitting a mine, the cell flashes red with a yellow border and displays 💥
- **Mine Numbers**: Color-coded adjacency numbers (1-8) for better readability
  - 1 = Blue, 2 = Green, 3 = Red, 4 = Navy, 5 = Maroon, 6 = Teal, 7 = Black, 8 = Gray
- **Enhanced Dialogs**: Win/lose messages with emojis and styled backgrounds

#### 3. **Audio System**
- Integrated QMultimedia for sound support
- System beep on mine explosion
- Framework ready for custom sound files (reveal.wav, explosion.wav)

#### 4. **UI Beautification**
- Modern dark theme (#2b2b2b background)
- Styled buttons with hover effects
- Color-coded labels (gold for mines, sky blue for timer)
- Improved spacing and padding
- Green "New Game" button with hover effect

### 🎯 How to Play
1. Click cells to reveal them
2. Right-click to place flags on suspected mines
3. Use layer navigation buttons to explore 3D space
4. Win by revealing all safe cells or correctly flagging all mines

### 🛠️ Building the Project
```bash
qmake EndOfTerm_test.pro
make
./EndOfTerm_test
```

### 📝 Technical Details
- **Framework**: Qt 5/6 with Widgets and Multimedia
- **Language**: C++17
- **Features**: Multi-layer 3D minesweeper with customizable grid size
