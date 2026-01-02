#include "polyhedrawidget.h"
#include <QApplication>
#include <QSet>
#include <algorithm>
#include <cassert>

PolyhedraWidget::PolyhedraWidget(PolyhedraType type, QWidget *parent)
    : QWidget(parent), polyType(type)
{
    // Initialize sound effects
    explosionSound = new QSoundEffect(this);
    winSound = new QSoundEffect(this);
    startSound = new QSoundEffect(this);

    explosionSound->setVolume(0.7);
    winSound->setVolume(0.7);
    startSound->setVolume(0.7);

    explosionSound->setSource(QUrl("qrc:/sounds/boom.wav"));
    winSound->setSource(QUrl("qrc:/sounds/win.wav"));
    startSound->setSource(QUrl("qrc:/sounds/start.wav"));

    setupPolyhedraStructure();
    buildUI();
    newGame();
}

PolyhedraWidget::~PolyhedraWidget()
{
}

void PolyhedraWidget::setupPolyhedraStructure()
{
    switch (polyType) {
    case TETRAHEDRON:
        faceCount = 4;
        rowsPerFace = 5;  // Triangular faces
        colsPerFace = 5;
        mineCount = 8;
        
        // Define face adjacency for tetrahedron
        // Each face is adjacent to 3 other faces
        faceAdjacency = {
            {1, 2, 3},  // Face 0
            {0, 2, 3},  // Face 1
            {0, 1, 3},  // Face 2
            {0, 1, 2}   // Face 3
        };
        
        // Define colors for each face
        faceColors = {
            QColor(255, 100, 100),  // Red
            QColor(100, 255, 100),  // Green
            QColor(100, 100, 255),  // Blue
            QColor(255, 255, 100)   // Yellow
        };
        break;

    case CUBE:
        faceCount = 6;
        rowsPerFace = 6;  // Square faces
        colsPerFace = 6;
        mineCount = 15;
        
        // Define face adjacency for cube
        // 0=front, 1=back, 2=left, 3=right, 4=top, 5=bottom
        faceAdjacency = {
            {2, 3, 4, 5},  // Face 0 (front)
            {2, 3, 4, 5},  // Face 1 (back)
            {0, 1, 4, 5},  // Face 2 (left)
            {0, 1, 4, 5},  // Face 3 (right)
            {0, 1, 2, 3},  // Face 4 (top)
            {0, 1, 2, 3}   // Face 5 (bottom)
        };
        
        faceColors = {
            QColor(255, 80, 80),    // Red
            QColor(255, 165, 0),    // Orange
            QColor(255, 255, 100),  // Yellow
            QColor(100, 255, 100),  // Green
            QColor(100, 150, 255),  // Blue
            QColor(200, 100, 255)   // Purple
        };
        break;

    case DODECAHEDRON:
        faceCount = 12;
        rowsPerFace = 5;  // Pentagonal faces
        colsPerFace = 5;
        mineCount = 25;
        
        // Define face adjacency for dodecahedron
        // Each pentagonal face is adjacent to 5 other faces
        faceAdjacency = {
            {1, 2, 3, 4, 5},     // Face 0
            {0, 2, 6, 7, 5},     // Face 1
            {0, 1, 6, 8, 3},     // Face 2
            {0, 2, 8, 9, 4},     // Face 3
            {0, 3, 9, 10, 5},    // Face 4
            {0, 4, 10, 11, 1},   // Face 5
            {1, 2, 8, 11, 7},    // Face 6
            {1, 6, 11, 10, 5},   // Face 7
            {2, 3, 9, 11, 6},    // Face 8
            {3, 4, 10, 11, 8},   // Face 9
            {4, 5, 7, 11, 9},    // Face 10
            {6, 7, 8, 9, 10}     // Face 11 (bottom) - adjacent to 5 faces
        };
        
        faceColors = {
            QColor(255, 100, 100), QColor(255, 180, 100), QColor(255, 255, 100),
            QColor(180, 255, 100), QColor(100, 255, 100), QColor(100, 255, 180),
            QColor(100, 255, 255), QColor(100, 180, 255), QColor(100, 100, 255),
            QColor(180, 100, 255), QColor(255, 100, 255), QColor(255, 100, 180)
        };
        break;
    }
}

void PolyhedraWidget::buildUI()
{
    QString typeName;
    switch (polyType) {
    case TETRAHEDRON: typeName = "Tetrahedron (4 Faces)"; break;
    case CUBE: typeName = "Cube (6 Faces)"; break;
    case DODECAHEDRON: typeName = "Dodecahedron (12 Faces)"; break;
    }

    setWindowTitle("Game 2: " + typeName);
    setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; font-family: Arial; }");

    // Back button
    backBtn = new QPushButton("← Back", this);
    backBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    connect(backBtn, &QPushButton::clicked, this, &PolyhedraWidget::backToMenu);

    // New game button
    newGameBtn = new QPushButton("New Game", this);
    newGameBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #45a049; }");
    connect(newGameBtn, &QPushButton::clicked, this, &PolyhedraWidget::newGame);

    // Hint button
    hintBtn = new QPushButton("💡 Hint", this);
    hintBtn->setStyleSheet("QPushButton { background-color: #FFA500; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #FF8C00; }");
    connect(hintBtn, &QPushButton::clicked, this, &PolyhedraWidget::showHint);

    // Info labels
    mineLabel = new QLabel("Mines: 0", this);
    mineLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    
    timeLabel = new QLabel("Time: 0", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");
    
    faceLabel = new QLabel("Face: 0", this);
    faceLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #90EE90;");

    // Control layout
    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    ctrlLayout->addWidget(backBtn);
    ctrlLayout->addWidget(newGameBtn);
    ctrlLayout->addWidget(hintBtn);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(mineLabel);
    ctrlLayout->addWidget(timeLabel);

    // Timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &PolyhedraWidget::onTimerTick);

    // Thumbnail panel (left side)
    QWidget *thumbnailWidget = new QWidget(this);
    thumbnailWidget->setStyleSheet("QWidget { background-color: #1a1a1a; border-radius: 5px; }");
    thumbnailLayout = new QVBoxLayout(thumbnailWidget);
    thumbnailLayout->setSpacing(5);
    
    QLabel *thumbTitle = new QLabel("Faces", thumbnailWidget);
    thumbTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #fff;");
    thumbTitle->setAlignment(Qt::AlignCenter);
    thumbnailLayout->addWidget(thumbTitle);

    // Create thumbnails
    for (int i = 0; i < faceCount; ++i) {
        QString label = QString("F%1").arg(i);
        FaceThumbnail *thumb = new FaceThumbnail(i, label, faceColors[i], thumbnailWidget);
        connect(thumb, &QPushButton::clicked, [this, i]() { switchToFace(i); });
        thumbnails.append(thumb);
        thumbnailLayout->addWidget(thumb);
    }
    thumbnailLayout->addStretch();

    QScrollArea *thumbScroll = new QScrollArea(this);
    thumbScroll->setWidget(thumbnailWidget);
    thumbScroll->setWidgetResizable(true);
    thumbScroll->setFixedWidth(120);
    thumbScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    thumbScroll->setStyleSheet("QScrollArea { border: none; background-color: #1a1a1a; }");

    // Main face area
    QWidget *faceWidget = new QWidget(this);
    QVBoxLayout *faceLayout = new QVBoxLayout(faceWidget);
    faceLayout->addWidget(faceLabel, 0, Qt::AlignCenter);
    
    mainFaceLayout = new QGridLayout();
    mainFaceLayout->setSpacing(2);
    faceLayout->addLayout(mainFaceLayout);

    // Adjacent faces preview panel (right side)
    adjacentPreviewsWidget = new QWidget(this);
    adjacentPreviewsWidget->setStyleSheet("QWidget { background-color: #1a1a1a; border-radius: 5px; }");
    adjacentPreviewsLayout = new QVBoxLayout(adjacentPreviewsWidget);
    adjacentPreviewsLayout->setSpacing(10);
    
    QLabel *previewTitle = new QLabel("Adjacent Faces", adjacentPreviewsWidget);
    previewTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #fff; background-color: transparent;");
    previewTitle->setAlignment(Qt::AlignCenter);
    adjacentPreviewsLayout->addWidget(previewTitle);
    
    // Container for preview frames (so we can insert without worrying about stretch)
    previewContainer = new QWidget(adjacentPreviewsWidget);
    previewContainerLayout = new QVBoxLayout(previewContainer);
    previewContainerLayout->setSpacing(5);
    previewContainerLayout->setContentsMargins(0, 0, 0, 0);
    adjacentPreviewsLayout->addWidget(previewContainer);
    adjacentPreviewsLayout->addStretch();

    QScrollArea *previewScroll = new QScrollArea(this);
    previewScroll->setWidget(adjacentPreviewsWidget);
    previewScroll->setWidgetResizable(true);
    previewScroll->setMinimumWidth(200);
    previewScroll->setMaximumWidth(300);
    previewScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    previewScroll->setStyleSheet("QScrollArea { border: none; background-color: #1a1a1a; }");

    // Main layout
    QHBoxLayout *gameLayout = new QHBoxLayout();
    gameLayout->addWidget(thumbScroll);
    gameLayout->addWidget(faceWidget, 1);
    gameLayout->addWidget(previewScroll);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(ctrlLayout);
    mainLayout->addLayout(gameLayout);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(mainLayout);
}

void PolyhedraWidget::newGame()
{
    if (!startSound->source().isEmpty()) {
        startSound->play();
    }

    firstClick = true;
    gameOver = false;
    revealedCells = 0;
    elapsedSeconds = 0;
    currentFace = 0;
    timer->stop();
    timeLabel->setText("Time: 0");

    resetModel();
    clearFaceButtons();
    buildFaceButtons();
    updateMineLabel();
    faceLabel->setText(QString("Face: %1").arg(currentFace));
    
    // Build adjacent face previews for the current face
    buildAdjacentPreviews();
}

void PolyhedraWidget::resetModel()
{
    faces.clear();
    faces.resize(faceCount);
    for (int f = 0; f < faceCount; ++f) {
        faces[f].resize(rowsPerFace);
        for (int r = 0; r < rowsPerFace; ++r) {
            faces[f][r].resize(colsPerFace);
            for (int c = 0; c < colsPerFace; ++c) {
                faces[f][r][c] = FaceCell{};
            }
        }
    }
}

void PolyhedraWidget::buildFaceButtons()
{
    buttonGrid.clear();
    buttonGrid.resize(rowsPerFace);
    const int btnSize = 36;

    for (int r = 0; r < rowsPerFace; ++r) {
        buttonGrid[r].resize(colsPerFace);
        for (int c = 0; c < colsPerFace; ++c) {
            FaceCellButton *btn = new FaceCellButton(r, c, this);
            btn->setFixedSize(btnSize, btnSize);
            btn->setText("");
            btn->setFocusPolicy(Qt::NoFocus);

            QColor faceColor = getFaceColor(currentFace);
            btn->setStyleSheet(QString(
                "QPushButton { "
                "  background-color: %1; "
                "  color: #ffffff; "
                "  border: 1px solid #000; "
                "  font-weight: bold; "
                "  font-size: 12px; "
                "} "
                "QPushButton:hover { "
                "  border: 2px solid #fff; "
                "}"
            ).arg(faceColor.darker(120).name()));

            mainFaceLayout->addWidget(btn, r, c);
            buttonGrid[r][c] = btn;

            connect(btn, &FaceCellButton::leftClicked, this, &PolyhedraWidget::onLeftClick);
            connect(btn, &FaceCellButton::rightClicked, this, &PolyhedraWidget::onRightClick);
        }
    }
}

void PolyhedraWidget::clearFaceButtons()
{
    QLayoutItem *it;
    while ((it = mainFaceLayout->takeAt(0)) != nullptr) {
        QWidget *w = it->widget();
        if (w) {
            w->setParent(nullptr);
            delete w;
        }
        delete it;
    }
}

void PolyhedraWidget::placeMinesSafely(int safeFace, int safeR, int safeC)
{
    QVector<int> idxs;
    const int totalCells = faceCount * rowsPerFace * colsPerFace;
    idxs.reserve(totalCells);

    // Get safe cell neighbors once
    auto safeNeighbors = getEdgeAdjacentCells(safeFace, safeR, safeC);
    
    // Build safe cells set for efficient lookup
    QSet<QPair<int, int>> safeArea;
    safeArea.insert({safeR, safeC});
    for (const auto &neighbor : safeNeighbors) {
        safeArea.insert(neighbor);
    }

    for (int f = 0; f < faceCount; ++f) {
        for (int r = 0; r < rowsPerFace; ++r) {
            for (int c = 0; c < colsPerFace; ++c) {
                // Skip cells in safe area on the safe face
                if (f == safeFace && safeArea.contains({r, c})) {
                    continue;
                }
                
                idxs.append(f * rowsPerFace * colsPerFace + r * colsPerFace + c);
            }
        }
    }

    std::shuffle(idxs.begin(), idxs.end(), *QRandomGenerator::global());
    for (int i = 0; i < mineCount && i < idxs.size(); ++i) {
        int val = idxs[i];
        int f = val / (rowsPerFace * colsPerFace);
        int rem = val % (rowsPerFace * colsPerFace);
        int r = rem / colsPerFace;
        int c = rem % colsPerFace;
        faces[f][r][c].isMine = true;
    }
}


void PolyhedraWidget::calculateAdjacents()
{
    for (int f = 0; f < faceCount; ++f) {
        for (int r = 0; r < rowsPerFace; ++r) {
            for (int c = 0; c < colsPerFace; ++c) {
                if (faces[f][r][c].isMine) {
                    faces[f][r][c].adj = -1;
                    continue;
                }
                
                int cnt = 0;
                auto neighbors = getEdgeAdjacentCells(f, r, c);
                for (const auto &neighbor : neighbors) {
                    if (faces[f][neighbor.first][neighbor.second].isMine) {
                        ++cnt;
                    }
                }
                faces[f][r][c].adj = cnt;
            }
        }
    }
}

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

bool PolyhedraWidget::inBounds(int face, int r, int c) const
{
    return face >= 0 && face < faceCount && 
           r >= 0 && r < rowsPerFace && 
           c >= 0 && c < colsPerFace;
}

void PolyhedraWidget::onLeftClick(int r, int c)
{
    if (gameOver) return;
    if (!inBounds(currentFace, r, c)) return;

    if (firstClick) {
        placeMinesSafely(currentFace, r, c);
        calculateAdjacents();
        firstClick = false;
        timer->start();
    }

    FaceCell &cell = faces[currentFace][r][c];
    if (cell.flagged || cell.revealed) return;

    if (cell.isMine) {
        if (!explosionSound->source().isEmpty()) {
            explosionSound->play();
        }

        showExplosionEffect(r, c);

        // Reveal all mines
        for (int f = 0; f < faceCount; ++f) {
            for (int rr = 0; rr < rowsPerFace; ++rr) {
                for (int cc = 0; cc < colsPerFace; ++cc) {
                    if (faces[f][rr][cc].isMine) {
                        faces[f][rr][cc].revealed = true;
                        if (f == currentFace) updateButtonVisual(f, rr, cc);
                    }
                }
            }
        }
        
        gameOver = true;
        timer->stop();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("💥 Game Over 💥");
        msgBox.setText("<h2 style='color: #ff0000;'>💣 BOOM! 💣</h2><p>You hit a mine!</p>");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #555; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
        return;
    }

    revealCell(currentFace, r, c);
    updateButtonVisual(currentFace, r, c);
    checkWinCondition();
}

void PolyhedraWidget::onRightClick(int r, int c)
{
    if (gameOver || firstClick) return;
    if (!inBounds(currentFace, r, c)) return;
    
    FaceCell &cell = faces[currentFace][r][c];
    if (cell.revealed) return;
    
    cell.flagged = !cell.flagged;
    updateButtonVisual(currentFace, r, c);
    updateMineLabel();
}

void PolyhedraWidget::revealCell(int face, int r, int c)
{
    FaceCell &cell = faces[face][r][c];
    if (cell.revealed || cell.flagged) return;
    
    cell.revealed = true;
    revealedCells++;
    
    if (face == currentFace) {
        updateButtonVisual(face, r, c);
    } else {
        // If this face is in the adjacent previews, update it
        updateAdjacentPreview(face);
    }
    
    if (cell.adj == 0) {
        revealNeighborsIfEmpty(face, r, c);
    }
}

void PolyhedraWidget::revealNeighborsIfEmpty(int face, int r, int c)
{
    QVector<QPair<QPair<int, int>, int>> stack; // ((r, c), face)
    stack.append({{r, c}, face});
    
    QSet<int> updatedFaces; // Track which adjacent faces need preview updates
    
    while (!stack.isEmpty()) {
        auto item = stack.takeLast();
        int cf = item.second;
        int cr = item.first.first;
        int cc = item.first.second;
        
        auto neighbors = getEdgeAdjacentCells(cf, cr, cc);
        for (const auto &neighbor : neighbors) {
            int nr = neighbor.first;
            int nc = neighbor.second;
            
            if (!inBounds(cf, nr, nc)) continue;
            
            FaceCell &ncell = faces[cf][nr][nc];
            if (ncell.revealed || ncell.flagged || ncell.isMine) continue;
            
            ncell.revealed = true;
            revealedCells++;
            
            if (cf == currentFace) {
                updateButtonVisual(cf, nr, nc);
            } else {
                updatedFaces.insert(cf);
            }
            
            if (ncell.adj == 0) {
                stack.append({{nr, nc}, cf});
            }
        }
    }
    
    // Update all adjacent previews that had cells revealed
    for (int faceIdx : updatedFaces) {
        updateAdjacentPreview(faceIdx);
    }
}

void PolyhedraWidget::updateButtonVisual(int face, int r, int c)
{
    if (face != currentFace) return;
    
    FaceCell &cell = faces[face][r][c];
    FaceCellButton *btn = buttonGrid[r][c];
    if (!btn) return;

    QColor faceColor = getFaceColor(face);
    
    if (cell.revealed) {
        btn->setEnabled(false);
        if (cell.isMine) {
            btn->setText("💣");
            btn->setStyleSheet(QString(
                "QPushButton { "
                "  background-color: %1; "
                "  color: #ffffff; "
                "  border: 1px solid #000; "
                "  font-size: 18px; "
                "}"
            ).arg(faceColor.lighter(150).name()));
        } else if (cell.adj > 0) {
            btn->setText(QString::number(cell.adj));
            QString numColor;
            switch (cell.adj) {
            case 1: numColor = "#0000FF"; break;
            case 2: numColor = "#008000"; break;
            case 3: numColor = "#FF0000"; break;
            case 4: numColor = "#000080"; break;
            default: numColor = "#000000"; break;
            }
            btn->setStyleSheet(QString(
                "QPushButton { "
                "  background-color: %1; "
                "  color: %2; "
                "  border: 1px solid #000; "
                "  font-weight: bold; "
                "  font-size: 12px; "
                "}"
            ).arg(faceColor.lighter(180).name()).arg(numColor));
        } else {
            btn->setText("");
            btn->setStyleSheet(QString(
                "QPushButton { "
                "  background-color: %1; "
                "  border: 1px solid #000; "
                "}"
            ).arg(faceColor.lighter(180).name()));
        }
    } else {
        btn->setEnabled(true);
        btn->setStyleSheet(QString(
            "QPushButton { "
            "  background-color: %1; "
            "  color: #ffffff; "
            "  border: 1px solid #000; "
            "  font-weight: bold; "
            "  font-size: 12px; "
            "} "
            "QPushButton:hover { "
            "  border: 2px solid #fff; "
            "}"
        ).arg(faceColor.darker(120).name()));
        
        if (cell.flagged) {
            btn->setText("⚑");
        } else {
            btn->setText("");
        }
    }
}

void PolyhedraWidget::updateMineLabel()
{
    int flagged = 0;
    for (int f = 0; f < faceCount; ++f) {
        for (int r = 0; r < rowsPerFace; ++r) {
            for (int c = 0; c < colsPerFace; ++c) {
                if (faces[f][r][c].flagged) ++flagged;
            }
        }
    }
    int remain = mineCount - flagged;
    mineLabel->setText(QString("Mines: %1").arg(remain));
}

void PolyhedraWidget::switchToFace(int faceIdx)
{
    if (faceIdx < 0 || faceIdx >= faceCount) return;
    
    currentFace = faceIdx;
    faceLabel->setText(QString("Face: %1").arg(currentFace));
    
    clearFaceButtons();
    buildFaceButtons();
    
    for (int r = 0; r < rowsPerFace; ++r) {
        for (int c = 0; c < colsPerFace; ++c) {
            updateButtonVisual(currentFace, r, c);
        }
    }
    
    // Rebuild adjacent face previews for the new current face
    buildAdjacentPreviews();
}

void PolyhedraWidget::onTimerTick()
{
    ++elapsedSeconds;
    timeLabel->setText(QString("Time: %1").arg(elapsedSeconds));
}

void PolyhedraWidget::checkWinCondition()
{
    const int totalCells = faceCount * rowsPerFace * colsPerFace;
    if (revealedCells == totalCells - mineCount) {
        gameOver = true;
        timer->stop();
        
        if (!winSound->source().isEmpty()) {
            winSound->play();
        }
        
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🎉 Victory! 🎉");
        msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 Congratulations! 🏆</h2><p>You cleared all mines in %1 seconds!</p>").arg(elapsedSeconds));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
    }
}

QColor PolyhedraWidget::getFaceColor(int face) const
{
    if (face >= 0 && face < faceColors.size()) {
        return faceColors[face];
    }
    return QColor(100, 100, 100);
}

void PolyhedraWidget::showExplosionEffect(int r, int c)
{
    FaceCellButton *btn = buttonGrid[r][c];
    if (!btn) return;
    
    btn->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; border: 3px solid #ffff00; font-size: 20px; }");
    btn->setText("💥");
    
    QApplication::processEvents();
}

void PolyhedraWidget::showHint()
{
    if (gameOver || firstClick) return;
    
    // Collect all unrevealed, safe cells on current face
    QVector<QPair<int, int>> safeCells;
    for (int r = 0; r < rowsPerFace; ++r) {
        for (int c = 0; c < colsPerFace; ++c) {
            FaceCell &cell = faces[currentFace][r][c];
            if (!cell.revealed && !cell.isMine) {
                safeCells.append({r, c});
            }
        }
    }
    
    if (safeCells.isEmpty()) {
        QMessageBox::information(this, "Hint", "No more safe cells on this face!");
        return;
    }
    
    // Randomly select one safe cell
    int idx = QRandomGenerator::global()->bounded(safeCells.size());
    int r = safeCells[idx].first;
    int c = safeCells[idx].second;
    
    // Reveal it
    revealCell(currentFace, r, c);
    updateButtonVisual(currentFace, r, c);
    checkWinCondition();
}

void PolyhedraWidget::buildAdjacentPreviews()
{
    // Clear existing previews
    // Qt's parent-child ownership will handle cleanup, but we need to clear our references
    for (auto &preview : adjacentPreviews) {
        if (preview.frame) {
            preview.frame->deleteLater();
        }
    }
    adjacentPreviews.clear();
    
    // Get adjacent faces for current face
    const QVector<int> &adjacentFaces = faceAdjacency[currentFace];
    
    // Create preview for each adjacent face
    for (int adjFace : adjacentFaces) {
        AdjacentFacePreview preview;
        preview.faceIndex = adjFace;
        
        // Create frame for this preview
        preview.frame = new QFrame(adjacentPreviewsWidget);
        preview.frame->setStyleSheet(QString(
            "QFrame { "
            "  background-color: %1; "
            "  border: 2px solid #555; "
            "  border-radius: 5px; "
            "  padding: 5px; "
            "}"
        ).arg(getFaceColor(adjFace).darker(150).name()));
        
        QVBoxLayout *frameLayout = new QVBoxLayout(preview.frame);
        frameLayout->setSpacing(2);
        frameLayout->setContentsMargins(5, 5, 5, 5);
        
        // Label showing which face this is
        preview.label = new QLabel(QString("Face %1").arg(adjFace), preview.frame);
        preview.label->setStyleSheet("font-size: 11px; font-weight: bold; color: #fff; background-color: transparent;");
        preview.label->setAlignment(Qt::AlignCenter);
        frameLayout->addWidget(preview.label);
        
        // Create mini-grid
        preview.gridLayout = new QGridLayout();
        preview.gridLayout->setSpacing(1);
        preview.gridLayout->setContentsMargins(0, 0, 0, 0);
        
        // Determine cell size based on grid dimensions to fit in preview panel
        int cellSize = qMax(MIN_PREVIEW_CELL_SIZE, 
                           qMin(MAX_PREVIEW_CELL_SIZE, 
                                PREVIEW_PANEL_WIDTH / qMax(rowsPerFace, colsPerFace)));
        
        // Create cells for the preview
        preview.cells.resize(rowsPerFace);
        for (int r = 0; r < rowsPerFace; ++r) {
            preview.cells[r].resize(colsPerFace);
            for (int c = 0; c < colsPerFace; ++c) {
                QLabel *cell = new QLabel(preview.frame);
                cell->setFixedSize(cellSize, cellSize);
                cell->setAlignment(Qt::AlignCenter);
                cell->setStyleSheet(QString(
                    "QLabel { "
                    "  background-color: %1; "
                    "  border: 1px solid #000; "
                    "  font-size: 8px; "
                    "  font-weight: bold; "
                    "  color: #fff; "
                    "}"
                ).arg(getFaceColor(adjFace).darker(120).name()));
                
                preview.gridLayout->addWidget(cell, r, c);
                preview.cells[r][c] = cell;
            }
        }
        
        frameLayout->addLayout(preview.gridLayout);
        
        // Add to preview container layout
        previewContainerLayout->addWidget(preview.frame);
        
        adjacentPreviews.append(preview);
    }
    
    // Update all previews with current game state
    updateAllAdjacentPreviews();
}

void PolyhedraWidget::updateAdjacentPreview(int adjacentFaceIdx)
{
    // Find the preview for this face
    for (auto &preview : adjacentPreviews) {
        if (preview.faceIndex == adjacentFaceIdx) {
            // Update each cell in the preview
            for (int r = 0; r < rowsPerFace; ++r) {
                for (int c = 0; c < colsPerFace; ++c) {
                    // Assert that preview cells are properly sized
                    assert(r < preview.cells.size() && c < preview.cells[r].size());
                    
                    QLabel *cell = preview.cells[r][c];
                    const FaceCell &gameCell = faces[adjacentFaceIdx][r][c];
                    QColor faceColor = getFaceColor(adjacentFaceIdx);
                    
                    if (gameCell.revealed) {
                        if (gameCell.isMine) {
                            cell->setText("💣");
                            cell->setStyleSheet(QString(
                                "QLabel { "
                                "  background-color: %1; "
                                "  border: 1px solid #000; "
                                "  font-size: 8px; "
                                "  color: #fff; "
                                "}"
                            ).arg(faceColor.lighter(150).name()));
                        } else if (gameCell.adj > 0) {
                            cell->setText(QString::number(gameCell.adj));
                            QString numColor;
                            switch (gameCell.adj) {
                            case 1: numColor = "#0000FF"; break;
                            case 2: numColor = "#008000"; break;
                            case 3: numColor = "#FF0000"; break;
                            case 4: numColor = "#000080"; break;
                            default: numColor = "#000000"; break;
                            }
                            cell->setStyleSheet(QString(
                                "QLabel { "
                                "  background-color: %1; "
                                "  border: 1px solid #000; "
                                "  font-size: 8px; "
                                "  font-weight: bold; "
                                "  color: %2; "
                                "}"
                            ).arg(faceColor.lighter(180).name()).arg(numColor));
                        } else {
                            cell->setText("");
                            cell->setStyleSheet(QString(
                                "QLabel { "
                                "  background-color: %1; "
                                "  border: 1px solid #000; "
                                "}"
                            ).arg(faceColor.lighter(180).name()));
                        }
                    } else {
                        cell->setText("");
                        cell->setStyleSheet(QString(
                            "QLabel { "
                            "  background-color: %1; "
                            "  border: 1px solid #000; "
                            "}"
                        ).arg(faceColor.darker(120).name()));
                    }
                }
            }
            break;
        }
    }
}

void PolyhedraWidget::updateAllAdjacentPreviews()
{
    for (const auto &preview : adjacentPreviews) {
        updateAdjacentPreview(preview.faceIndex);
    }
}
