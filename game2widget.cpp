#include "game2widget.h"
#include <QApplication>
#include <algorithm>

Game2Widget::Game2Widget(DifficultyWidget::Difficulty difficulty, QWidget *parent)
    : QWidget(parent), difficulty(difficulty)
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

    setupUI();
    newGame();
}

Game2Widget::~Game2Widget()
{
}

void Game2Widget::setupUI()
{
    setWindowTitle("Multi-Face Minesweeper - Game 2");
    setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; font-family: Arial; }");

    // Top control bar
    backButton = new QPushButton("返回選單", this);
    backButton->setStyleSheet("QPushButton { background-color: #888; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #777; }");
    connect(backButton, &QPushButton::clicked, this, &Game2Widget::backToMenu);

    newGameBtn = new QPushButton("開新遊戲", this);
    newGameBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #45a049; }");
    connect(newGameBtn, &QPushButton::clicked, this, &Game2Widget::newGame);

    hintButton = new QPushButton("提示💡", this);
    hintButton->setStyleSheet("QPushButton { background-color: #FFA500; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #FF8C00; }");
    connect(hintButton, &QPushButton::clicked, this, &Game2Widget::giveHint);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(backButton);
    topLayout->addStretch();
    topLayout->addWidget(newGameBtn);
    topLayout->addWidget(hintButton);

    // Info display
    faceLabel = new QLabel("Face: 1", this);
    faceLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");
    
    mineLabel = new QLabel("Mines: 0", this);
    mineLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    
    timeLabel = new QLabel("Time: 0", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(faceLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(mineLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(timeLabel);

    // Board layout
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);

    // Thumbnail panel on the left
    thumbnailWidget = new QWidget();
    thumbnailLayout = new QVBoxLayout(thumbnailWidget);
    thumbnailLayout->setSpacing(5);
    thumbnailLayout->setContentsMargins(5, 5, 5, 5);
    
    QLabel *thumbTitle = new QLabel("面", this);
    thumbTitle->setAlignment(Qt::AlignCenter);
    thumbTitle->setStyleSheet("font-weight: bold; color: #ffffff;");
    thumbnailLayout->addWidget(thumbTitle);
    thumbnailLayout->addStretch();
    
    thumbnailScrollArea = new QScrollArea(this);
    thumbnailScrollArea->setWidget(thumbnailWidget);
    thumbnailScrollArea->setWidgetResizable(true);
    thumbnailScrollArea->setFixedWidth(100);
    thumbnailScrollArea->setStyleSheet("QScrollArea { background-color: #1a1a1a; border: 1px solid #555; }");

    // Timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Game2Widget::onTimerTick);

    // Main horizontal layout: thumbnails on left, board on right
    QHBoxLayout *gameLayout = new QHBoxLayout();
    gameLayout->addWidget(thumbnailScrollArea);
    gameLayout->addLayout(boardLayout);

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(gameLayout);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    setLayout(mainLayout);
}

void Game2Widget::newGame()
{
    // Play start sound
    if (!startSound->source().isEmpty()) {
        startSound->play();
    }

    // Reset game state
    firstClick = true;
    gameOver = false;
    revealedCells = 0;
    elapsedSeconds = 0;
    timer->stop();
    timeLabel->setText("Time: 0");
    currentFaceId = 0;

    // Initialize polyhedron based on difficulty
    initializePolyhedron();
    
    // Build UI
    clearFaceButtons();
    buildFaceButtons();
    buildThumbnails();
    
    updateMineLabel();
    faceLabel->setText(QString("Face: %1").arg(faces[currentFaceId].name));
}

void Game2Widget::initializePolyhedron()
{
    faces.clear();
    
    switch (difficulty) {
    case DifficultyWidget::Easy:
        initializeTetrahedron();
        break;
    case DifficultyWidget::Medium:
        initializeHexahedron();
        break;
    case DifficultyWidget::Hard:
        initializeDodecahedron();
        break;
    }
    
    resetModel();
}

void Game2Widget::initializeTetrahedron()
{
    // Tetrahedron: 4 triangular faces
    // Each face is 5x5 grid (triangular shape)
    faces.resize(4);
    mineCount = 6;
    
    QVector<QColor> colors = {
        QColor(255, 100, 100),  // Red
        QColor(100, 255, 100),  // Green
        QColor(100, 100, 255),  // Blue
        QColor(255, 255, 100)   // Yellow
    };
    
    // Define adjacency (each face is adjacent to 3 others)
    QVector<QVector<int>> adjacency = {
        {1, 2, 3},  // Face 0 adjacent to 1, 2, 3
        {0, 2, 3},  // Face 1 adjacent to 0, 2, 3
        {0, 1, 3},  // Face 2 adjacent to 0, 1, 3
        {0, 1, 2}   // Face 3 adjacent to 0, 1, 2
    };
    
    totalCells = 0;
    for (int i = 0; i < 4; ++i) {
        faces[i].id = i;
        faces[i].rows = 5;
        faces[i].cols = 5;
        faces[i].baseColor = colors[i];
        faces[i].name = QString("T%1").arg(i + 1);
        faces[i].adjacentFaces = adjacency[i];
        totalCells += faces[i].rows * faces[i].cols;
    }
}

void Game2Widget::initializeHexahedron()
{
    // Hexahedron (Cube): 6 square faces
    // Each face is 6x6 grid
    faces.resize(6);
    mineCount = 12;
    
    QVector<QColor> colors = {
        QColor(255, 100, 100),  // Red - Front
        QColor(100, 255, 100),  // Green - Back
        QColor(100, 100, 255),  // Blue - Left
        QColor(255, 255, 100),  // Yellow - Right
        QColor(255, 150, 200),  // Pink - Top
        QColor(150, 255, 255)   // Cyan - Bottom
    };
    
    // Define adjacency for a cube
    QVector<QVector<int>> adjacency = {
        {2, 3, 4, 5},  // Face 0 (Front) adjacent to Left, Right, Top, Bottom
        {2, 3, 4, 5},  // Face 1 (Back) adjacent to Left, Right, Top, Bottom
        {0, 1, 4, 5},  // Face 2 (Left) adjacent to Front, Back, Top, Bottom
        {0, 1, 4, 5},  // Face 3 (Right) adjacent to Front, Back, Top, Bottom
        {0, 1, 2, 3},  // Face 4 (Top) adjacent to Front, Back, Left, Right
        {0, 1, 2, 3}   // Face 5 (Bottom) adjacent to Front, Back, Left, Right
    };
    
    totalCells = 0;
    for (int i = 0; i < 6; ++i) {
        faces[i].id = i;
        faces[i].rows = 6;
        faces[i].cols = 6;
        faces[i].baseColor = colors[i];
        faces[i].name = QString("H%1").arg(i + 1);
        faces[i].adjacentFaces = adjacency[i];
        totalCells += faces[i].rows * faces[i].cols;
    }
}

void Game2Widget::initializeDodecahedron()
{
    // Dodecahedron: 12 pentagonal faces
    // Simplified as 5x5 grids
    faces.resize(12);
    mineCount = 20;
    
    QVector<QColor> colors = {
        QColor(255, 100, 100), QColor(100, 255, 100), QColor(100, 100, 255),
        QColor(255, 255, 100), QColor(255, 150, 200), QColor(150, 255, 255),
        QColor(200, 150, 100), QColor(150, 200, 150), QColor(150, 150, 200),
        QColor(200, 200, 100), QColor(200, 100, 150), QColor(100, 200, 200)
    };
    
    // Simplified adjacency for dodecahedron (each face adjacent to 3 others)
    QVector<QVector<int>> adjacency = {
        {1, 4, 5},    {0, 2, 6},    {1, 3, 7},
        {2, 4, 8},    {0, 3, 9},    {0, 6, 10},
        {1, 5, 11},   {2, 8, 11},   {3, 7, 9},
        {4, 8, 10},   {5, 9, 11},   {6, 7, 10}
    };
    
    totalCells = 0;
    for (int i = 0; i < 12; ++i) {
        faces[i].id = i;
        faces[i].rows = 5;
        faces[i].cols = 5;
        faces[i].baseColor = colors[i];
        faces[i].name = QString("D%1").arg(i + 1);
        faces[i].adjacentFaces = adjacency[i];
        totalCells += faces[i].rows * faces[i].cols;
    }
}

void Game2Widget::resetModel()
{
    // Initialize cell data for each face
    for (int f = 0; f < faces.size(); ++f) {
        faces[f].cells.clear();
        faces[f].cells.resize(faces[f].rows);
        for (int r = 0; r < faces[f].rows; ++r) {
            faces[f].cells[r].resize(faces[f].cols);
            for (int c = 0; c < faces[f].cols; ++c) {
                faces[f].cells[r][c] = FaceCell{};
            }
        }
    }
}

void Game2Widget::buildFaceButtons()
{
    // Build buttons for the current face
    Face &face = faces[currentFaceId];
    buttonGrid.clear();
    buttonGrid.resize(face.rows);
    const int btnSize = 36;

    for (int r = 0; r < face.rows; ++r) {
        buttonGrid[r].resize(face.cols);
        for (int c = 0; c < face.cols; ++c) {
            FaceCellButton *btn = new FaceCellButton(r, c, this);
            btn->setFixedSize(btnSize, btnSize);
            btn->setText("");
            btn->setFocusPolicy(Qt::NoFocus);

            QColor cellColor = face.baseColor.darker(120);
            btn->setStyleSheet(QString(
                "QPushButton { background-color: %1; color: #ffffff; border: 1px solid #000; font-weight: bold; font-size: 12px; } "
                "QPushButton:hover { border: 2px solid #fff; }"
            ).arg(cellColor.name()));

            boardLayout->addWidget(btn, r, c);
            buttonGrid[r][c] = btn;

            connect(btn, &FaceCellButton::leftClicked, this, &Game2Widget::onLeftClick);
            connect(btn, &FaceCellButton::rightClicked, this, &Game2Widget::onRightClick);
        }
    }
}

void Game2Widget::clearFaceButtons()
{
    QLayoutItem *it;
    while ((it = boardLayout->takeAt(0)) != nullptr) {
        QWidget *w = it->widget();
        if (w) {
            w->setParent(nullptr);
            delete w;
        }
        delete it;
    }
    buttonGrid.clear();
}

void Game2Widget::buildThumbnails()
{
    // Clear existing thumbnails
    for (auto btn : thumbnailButtons) {
        delete btn;
    }
    thumbnailButtons.clear();

    // Remove stretch if exists
    if (thumbnailLayout->count() > 0) {
        QLayoutItem *item = thumbnailLayout->takeAt(thumbnailLayout->count() - 1);
        delete item;
    }

    // Create thumbnail for each face
    for (int i = 0; i < faces.size(); ++i) {
        QPushButton *thumb = new QPushButton(faces[i].name, thumbnailWidget);
        thumb->setFixedSize(80, 60);
        thumb->setStyleSheet(QString(
            "QPushButton { background-color: %1; color: white; border: 2px solid #000; font-weight: bold; border-radius: 5px; } "
            "QPushButton:hover { border: 3px solid #fff; }"
        ).arg(faces[i].baseColor.name()));
        
        connect(thumb, &QPushButton::clicked, this, [this, i]() { switchToFace(i); });
        
        thumbnailLayout->insertWidget(thumbnailLayout->count(), thumb);
        thumbnailButtons.append(thumb);
    }

    thumbnailLayout->addStretch();
}

void Game2Widget::placeMinesSafely(int safeFace, int safeR, int safeC)
{
    // Collect all cells except safe cell and its edge-adjacent neighbors
    QVector<QPair<int, QPair<int, int>>> candidateCells;
    
    for (int f = 0; f < faces.size(); ++f) {
        for (int r = 0; r < faces[f].rows; ++r) {
            for (int c = 0; c < faces[f].cols; ++c) {
                bool isSafe = (f == safeFace && r == safeR && c == safeC);
                
                // Also exclude edge-adjacent cells of safe cell
                if (f == safeFace) {
                    auto adj = getEdgeAdjacentCells(safeFace, safeR, safeC);
                    for (const auto &cell : adj) {
                        if (cell.first == f && cell.second.first == r && cell.second.second == c) {
                            isSafe = true;
                            break;
                        }
                    }
                }
                
                if (!isSafe) {
                    candidateCells.append({f, {r, c}});
                }
            }
        }
    }

    // Shuffle and place mines
    std::shuffle(candidateCells.begin(), candidateCells.end(), *QRandomGenerator::global());
    for (int i = 0; i < mineCount && i < candidateCells.size(); ++i) {
        int f = candidateCells[i].first;
        int r = candidateCells[i].second.first;
        int c = candidateCells[i].second.second;
        faces[f].cells[r][c].isMine = true;
    }
}

void Game2Widget::calculateAdjacents()
{
    // Calculate adjacent mine count for each cell (edge-adjacent only)
    for (int f = 0; f < faces.size(); ++f) {
        for (int r = 0; r < faces[f].rows; ++r) {
            for (int c = 0; c < faces[f].cols; ++c) {
                if (faces[f].cells[r][c].isMine) {
                    faces[f].cells[r][c].adj = -1;
                    continue;
                }
                
                int cnt = 0;
                auto adjacent = getEdgeAdjacentCells(f, r, c);
                for (const auto &cell : adjacent) {
                    int af = cell.first;
                    int ar = cell.second.first;
                    int ac = cell.second.second;
                    if (inBounds(af, ar, ac) && faces[af].cells[ar][ac].isMine) {
                        ++cnt;
                    }
                }
                faces[f].cells[r][c].adj = cnt;
            }
        }
    }
}

QVector<QPair<int, QPair<int,int>>> Game2Widget::getEdgeAdjacentCells(int faceId, int r, int c)
{
    // Return edge-adjacent cells (up, down, left, right) within the same face
    // No vertex-only adjacency, no cross-face adjacency for now (simplified)
    QVector<QPair<int, QPair<int,int>>> result;
    
    const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; ++i) {
        int nr = r + dirs[i][0];
        int nc = c + dirs[i][1];
        if (inBounds(faceId, nr, nc)) {
            result.append({faceId, {nr, nc}});
        }
    }
    
    return result;
}

bool Game2Widget::inBounds(int faceId, int r, int c) const
{
    if (faceId < 0 || faceId >= faces.size()) return false;
    return r >= 0 && r < faces[faceId].rows && c >= 0 && c < faces[faceId].cols;
}

void Game2Widget::onLeftClick(int r, int c)
{
    if (gameOver) return;
    if (!inBounds(currentFaceId, r, c)) return;

    if (firstClick) {
        placeMinesSafely(currentFaceId, r, c);
        calculateAdjacents();
        firstClick = false;
        timer->start();
    }

    FaceCell &cell = faces[currentFaceId].cells[r][c];
    if (cell.flagged || cell.revealed) return;

    if (cell.isMine) {
        // Game over
        if (!explosionSound->source().isEmpty()) {
            explosionSound->play();
        }

        showExplosionEffect(r, c);

        // Reveal all mines
        for (int f = 0; f < faces.size(); ++f) {
            for (int rr = 0; rr < faces[f].rows; ++rr) {
                for (int cc = 0; cc < faces[f].cols; ++cc) {
                    if (faces[f].cells[rr][cc].isMine) {
                        faces[f].cells[rr][cc].revealed = true;
                        if (f == currentFaceId) updateButtonVisual(f, rr, cc);
                    }
                }
            }
        }
        gameOver = true;
        timer->stop();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("💥 Game Over 💥");
        msgBox.setText("<h2 style='color: #ff0000;'>💣 BOOM! 💣</h2><p>你點擊到了炸彈!</p>");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #555; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
        return;
    }

    revealCell(currentFaceId, r, c);
    updateButtonVisual(currentFaceId, r, c);
    checkWinCondition();
}

void Game2Widget::onRightClick(int r, int c)
{
    if (gameOver || firstClick) return;
    if (!inBounds(currentFaceId, r, c)) return;
    
    FaceCell &cell = faces[currentFaceId].cells[r][c];
    if (cell.revealed) return;
    
    cell.flagged = !cell.flagged;
    updateButtonVisual(currentFaceId, r, c);
    updateMineLabel();
    checkWinCondition();
}

void Game2Widget::revealCell(int faceId, int r, int c)
{
    FaceCell &cell = faces[faceId].cells[r][c];
    if (cell.revealed || cell.flagged) return;
    
    cell.revealed = true;
    revealedCells++;
    
    if (faceId == currentFaceId) {
        updateButtonVisual(faceId, r, c);
    }

    if (cell.adj == 0) {
        revealNeighborsIfEmpty(faceId, r, c);
    }
}

void Game2Widget::revealNeighborsIfEmpty(int faceId, int r, int c)
{
    QVector<QPair<int, QPair<int,int>>> stack;
    stack.append({faceId, {r, c}});

    while (!stack.isEmpty()) {
        auto item = stack.takeLast();
        int cf = item.first;
        int cr = item.second.first;
        int cc = item.second.second;

        auto adjacent = getEdgeAdjacentCells(cf, cr, cc);
        for (const auto &cell : adjacent) {
            int nf = cell.first;
            int nr = cell.second.first;
            int nc = cell.second.second;
            
            if (!inBounds(nf, nr, nc)) continue;
            
            FaceCell &ncell = faces[nf].cells[nr][nc];
            if (ncell.revealed || ncell.flagged || ncell.isMine) continue;
            
            ncell.revealed = true;
            revealedCells++;
            
            if (nf == currentFaceId) {
                updateButtonVisual(nf, nr, nc);
            }
            
            if (ncell.adj == 0) {
                stack.append({nf, {nr, nc}});
            }
        }
    }
}

void Game2Widget::updateButtonVisual(int faceId, int r, int c)
{
    if (faceId != currentFaceId) return;
    if (r < 0 || r >= buttonGrid.size()) return;
    if (c < 0 || c >= buttonGrid[r].size()) return;
    
    FaceCell &cell = faces[faceId].cells[r][c];
    FaceCellButton *btn = buttonGrid[r][c];
    if (!btn) return;

    QColor faceColor = faces[faceId].baseColor;
    QColor cellColor = cell.revealed ? faceColor.lighter(180) : faceColor.darker(120);
    
    QString baseStyle = QString(
        "QPushButton { background-color: %1; color: %2; border: 1px solid #000; font-weight: bold; font-size: 12px; }"
    ).arg(cellColor.name()).arg(cell.revealed ? "#000000" : "#ffffff");

    if (cell.revealed) {
        btn->setEnabled(false);
        if (cell.isMine) {
            btn->setText("💣");
            btn->setStyleSheet(baseStyle + " QPushButton { font-size: 18px; }");
        } else if (cell.adj > 0) {
            btn->setText(QString::number(cell.adj));
            QString numColor;
            switch (cell.adj) {
            case 1: numColor = "#0000FF"; break;
            case 2: numColor = "#008000"; break;
            case 3: numColor = "#FF0000"; break;
            case 4: numColor = "#000080"; break;
            case 5: numColor = "#800000"; break;
            case 6: numColor = "#008080"; break;
            case 7: numColor = "#000000"; break;
            case 8: numColor = "#808080"; break;
            default: numColor = "#000000"; break;
            }
            btn->setStyleSheet(baseStyle.replace("#000000", numColor));
        } else {
            btn->setText("");
            btn->setStyleSheet(baseStyle);
        }
    } else {
        btn->setEnabled(true);
        btn->setStyleSheet(baseStyle + " QPushButton:hover { border: 2px solid #fff; }");
        if (cell.flagged) btn->setText("⚑");
        else btn->setText("");
    }
}

void Game2Widget::updateMineLabel()
{
    int flagged = 0;
    for (int f = 0; f < faces.size(); ++f) {
        for (int r = 0; r < faces[f].rows; ++r) {
            for (int c = 0; c < faces[f].cols; ++c) {
                if (faces[f].cells[r][c].flagged) ++flagged;
            }
        }
    }
    int remain = mineCount - flagged;
    mineLabel->setText(QString("Mines: %1").arg(remain));
}

void Game2Widget::switchToFace(int faceId)
{
    if (faceId < 0 || faceId >= faces.size()) return;
    if (faceId == currentFaceId) return;

    currentFaceId = faceId;
    faceLabel->setText(QString("Face: %1").arg(faces[currentFaceId].name));

    clearFaceButtons();
    buildFaceButtons();

    // Update all buttons
    for (int r = 0; r < faces[currentFaceId].rows; ++r) {
        for (int c = 0; c < faces[currentFaceId].cols; ++c) {
            updateButtonVisual(currentFaceId, r, c);
        }
    }
}

void Game2Widget::checkWinCondition()
{
    if (revealedCells == totalCells - mineCount) {
        gameOver = true;
        timer->stop();
        
        if (!winSound->source().isEmpty()) {
            winSound->play();
        }
        
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🎉 通關! 🎉");
        msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 恭喜! 🏆</h2><p>您在 %1 秒內成功清空炸彈.</p>").arg(elapsedSeconds));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
    }
}

void Game2Widget::showExplosionEffect(int r, int c)
{
    if (r < 0 || r >= buttonGrid.size()) return;
    if (c < 0 || c >= buttonGrid[r].size()) return;
    
    FaceCellButton *btn = buttonGrid[r][c];
    if (!btn) return;

    btn->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; border: 3px solid #ffff00; font-size: 20px; }");
    btn->setText("💥");

    QApplication::processEvents();
}

void Game2Widget::giveHint()
{
    if (gameOver || firstClick) return;

    // Collect all unrevealed, unflagged, safe cells on current face
    QVector<QPair<int, int>> safeCells;
    for (int r = 0; r < faces[currentFaceId].rows; ++r) {
        for (int c = 0; c < faces[currentFaceId].cols; ++c) {
            FaceCell &cell = faces[currentFaceId].cells[r][c];
            if (!cell.revealed && !cell.flagged && !cell.isMine) {
                safeCells.append({r, c});
            }
        }
    }

    if (safeCells.isEmpty()) {
        QMessageBox::information(this, "提示", "當前面沒有可以揭露的安全格了!");
        return;
    }

    // Randomly reveal one
    int idx = QRandomGenerator::global()->bounded(safeCells.size());
    int r = safeCells[idx].first;
    int c = safeCells[idx].second;

    revealCell(currentFaceId, r, c);
    updateButtonVisual(currentFaceId, r, c);
    checkWinCondition();

    if (startSound && !startSound->source().isEmpty()) {
        startSound->play();
    }
}

void Game2Widget::onTimerTick()
{
    ++elapsedSeconds;
    timeLabel->setText(QString("Time: %1").arg(elapsedSeconds));
}
