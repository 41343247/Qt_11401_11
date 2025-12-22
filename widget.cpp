#include "widget.h"
#include <QApplication>

// Multi-Layer Minesweeper - Enhanced Edition
// Features: Layer colors, visual feedback, modern UI

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    // Initialize sound effects with embedded data URLs (simple beep sounds)
    revealSound = new QSoundEffect(this);
    explosionSound = new QSoundEffect(this);
    
    // We'll use simple tones - Qt can generate these or use resource files
    // For now, we'll set them up and check if files exist later
    revealSound->setVolume(0.5);
    explosionSound->setVolume(0.7);
    
    buildUI();
    newGame();
}

Widget::~Widget()
{
}

void Widget::buildUI()
{
    // Set window properties
    setWindowTitle("Multi-Layer Minesweeper");
    setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; font-family: Arial; }");
    
    // Top controls
    QLabel *lblRows = new QLabel("Rows:");
    lblRows->setStyleSheet("font-weight: bold;");
    spinRows = new QSpinBox(this);
    spinRows->setRange(5, 20);
    spinRows->setValue(rows);
    spinRows->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblCols = new QLabel("Cols:");
    lblCols->setStyleSheet("font-weight: bold;");
    spinCols = new QSpinBox(this);
    spinCols->setRange(5, 20);
    spinCols->setValue(cols);
    spinCols->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblLayers = new QLabel("Layers:");
    lblLayers->setStyleSheet("font-weight: bold;");
    spinLayers = new QSpinBox(this);
    spinLayers->setRange(1, 10);
    spinLayers->setValue(layers);
    spinLayers->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblMines = new QLabel("Mines:");
    lblMines->setStyleSheet("font-weight: bold;");
    spinMines = new QSpinBox(this);
    spinMines->setRange(1, 200);
    spinMines->setValue(mineCount);
    spinMines->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    newGameBtn = new QPushButton("New Game", this);
    newGameBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #45a049; }");
    connect(newGameBtn, &QPushButton::clicked, this, &Widget::newGame);

    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    ctrlLayout->addWidget(lblRows);
    ctrlLayout->addWidget(spinRows);
    ctrlLayout->addWidget(lblCols);
    ctrlLayout->addWidget(spinCols);
    ctrlLayout->addWidget(lblLayers);
    ctrlLayout->addWidget(spinLayers);
    ctrlLayout->addWidget(lblMines);
    ctrlLayout->addWidget(spinMines);
    ctrlLayout->addWidget(newGameBtn);

    // Layer controls
    prevBtn = new QPushButton("上一層", this);
    prevBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    nextBtn = new QPushButton("下一層", this);
    nextBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    layerBox = new QComboBox(this);
    layerBox->setStyleSheet("QComboBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border: none; }");
    connect(prevBtn, &QPushButton::clicked, this, &Widget::prevLayer);
    connect(nextBtn, &QPushButton::clicked, this, &Widget::nextLayer);
    connect(layerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Widget::showLayer);

    QHBoxLayout *layerLayout = new QHBoxLayout();
    layerLayout->addWidget(prevBtn);
    layerLayout->addWidget(layerBox);
    layerLayout->addWidget(nextBtn);

    // Info
    mineLabel = new QLabel("Mines: 0", this);
    mineLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    timeLabel = new QLabel("Time: 0", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(mineLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(timeLabel);

    // Board area
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);

    // Timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Widget::onTimerTick);

    // Main layout
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(ctrlLayout);
    main->addLayout(layerLayout);
    main->addLayout(infoLayout);
    main->addLayout(boardLayout);
    main->setContentsMargins(10, 10, 10, 10);
    setLayout(main);
}

void Widget::newGame()
{
    // read UI settings
    rows = spinRows->value();
    cols = spinCols->value();
    layers = spinLayers->value();
    mineCount = spinMines->value();

    if (mineCount >= rows * cols * layers) {
        QMessageBox::warning(this, "Invalid", "Too many mines for the board size.");
        return;
    }

    // reset state
    firstClick = true;
    gameOver = false;
    revealedCells = 0;
    elapsedSeconds = 0;
    timer->stop();
    timeLabel->setText("Time: 0");

    // model reset
    resetModel();

    // rebuild layer selector
    layerBox->clear();
    for (int l = 0; l < layers; ++l) layerBox->addItem(QString("Layer %1").arg(l+1));
    currentLayer = 0;
    layerBox->setCurrentIndex(currentLayer);

    // build UI buttons
    clearBoardButtons();
    buildBoardButtons();

    updateMineLabel();
}

void Widget::resetModel()
{
    board.clear();
    board.resize(layers);
    for (int l = 0; l < layers; ++l) {
        board[l].resize(rows);
        for (int r = 0; r < rows; ++r) {
            board[l][r].resize(cols);
            for (int c = 0; c < cols; ++c) {
                board[l][r][c] = Cell{};
            }
        }
    }
}

void Widget::buildBoardButtons()
{
    // create button grid for current layer (buttons are recreated but share signal to model)
    buttonGrid.clear();
    buttonGrid.resize(rows);
    const int btnSize = 36;

    for (int r = 0; r < rows; ++r) {
        buttonGrid[r].resize(cols);
        for (int c = 0; c < cols; ++c) {
            CellButton *btn = new CellButton(r, c, this);
            btn->setFixedSize(btnSize, btnSize);
            btn->setText(""); // 初始不顯示任何文字
            btn->setFocusPolicy(Qt::NoFocus);
            
            // Apply layer-specific color
            QColor layerColor = getLayerColor(currentLayer, false);
            btn->setStyleSheet(QString("QPushButton { background-color: %1; color: #ffffff; border: 1px solid #000; font-weight: bold; font-size: 12px; } QPushButton:hover { border: 2px solid #fff; }").arg(layerColor.name()));
            
            boardLayout->addWidget(btn, r, c);
            buttonGrid[r][c] = btn;

            connect(btn, &CellButton::leftClicked, this, &Widget::onLeftClick);
            connect(btn, &CellButton::rightClicked, this, &Widget::onRightClick);
        }
    }
}

void Widget::clearBoardButtons()
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
}

void Widget::placeMinesSafely(int safeLayer, int safeR, int safeC)
{
    // 隨機放置 mines，但避開 safe cell (safeLayer,safeR,safeC) 以及其鄰近格（保守處理）
    QVector<int> idxs;
    const int total = layers * rows * cols;
    idxs.reserve(total);
    for (int l = 0; l < layers; ++l)
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c) {
                // skip the safe cell and its neighbors
                bool isSafe = false;
                for (int dl = -1; dl <= 1 && !isSafe; ++dl)
                    for (int dr = -1; dr <= 1 && !isSafe; ++dr)
                        for (int dc = -1; dc <= 1 && !isSafe; ++dc) {
                            int nl = safeLayer + dl, nr = safeR + dr, nc = safeC + dc;
                            if (nl == l && nr == r && nc == c) { isSafe = true; break; }
                        }
                if (!isSafe) idxs.append(l * rows * cols + r * cols + c);
            }

    // shuffle and pick mineCount
    std::shuffle(idxs.begin(), idxs.end(), *QRandomGenerator::global());
    for (int i = 0; i < mineCount && i < idxs.size(); ++i) {
        int val = idxs[i];
        int l = val / (rows * cols);
        int rem = val % (rows * cols);
        int r = rem / cols;
        int c = rem % cols;
        board[l][r][c].isMine = true;
    }
}

void Widget::calculateAdjacents()
{
    for (int l = 0; l < layers; ++l) {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[l][r][c].isMine) { board[l][r][c].adj = -1; continue; }
                int cnt = 0;
                for (int dl = -1; dl <= 1; ++dl)
                    for (int dr = -1; dr <= 1; ++dr)
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dl == 0 && dr == 0 && dc == 0) continue;
                            int nl = l + dl, nr = r + dr, nc = c + dc;
                            if (inBounds(nl, nr, nc) && board[nl][nr][nc].isMine) ++cnt;
                        }
                board[l][r][c].adj = cnt;
            }
        }
    }
}

bool Widget::inBounds(int l, int r, int c) const {
    return l >= 0 && l < layers && r >= 0 && r < rows && c >= 0 && c < cols;
}

void Widget::onLeftClick(int r, int c)
{
    if (gameOver) return;
    if (!inBounds(currentLayer, r, c)) return;

    if (firstClick) {
        // 第一次點擊：先放地雷（避開該格周圍），再計算 adj
        placeMinesSafely(currentLayer, r, c);
        calculateAdjacents();
        firstClick = false;
        timer->start();
    }

    Cell &cell = board[currentLayer][r][c];
    if (cell.flagged || cell.revealed) return;

    // 若踩到地雷 -> game over
    if (cell.isMine) {
        // Play explosion sound
        if (explosionSound->source().isEmpty()) {
            // Generate a simple beep sound using system beep
            QApplication::beep();
        } else {
            explosionSound->play();
        }
        
        // Show explosion effect
        showExplosionEffect(r, c);
        
        // 顯示所有地雷
        for (int l = 0; l < layers; ++l)
            for (int rr = 0; rr < rows; ++rr)
                for (int cc = 0; cc < cols; ++cc) {
                    if (board[l][rr][cc].isMine) {
                        board[l][rr][cc].revealed = true;
                        if (l == currentLayer) updateButtonVisual(l, rr, cc);
                    }
                }
        gameOver = true;
        timer->stop();
        
        // Show game over message with explosion icon
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("💥 Game Over 💥");
        msgBox.setText("<h2 style='color: #ff0000;'>💣 BOOM! 💣</h2><p>You clicked a mine! Game Over.</p>");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #555; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
        return;
    }

    // Play reveal sound - use a simple notification
    // Since we don't have sound files, we use visual feedback instead
    // A proper implementation would load .wav files using QSoundEffect::setSource()
    
    // 揭露格子
    revealCell(currentLayer, r, c);
    updateButtonVisual(currentLayer, r, c);
    checkWinCondition();
}

void Widget::onRightClick(int r, int c)
{
    if (gameOver || firstClick) return;
    if (!inBounds(currentLayer, r, c)) return;
    Cell &cell = board[currentLayer][r][c];
    if (cell.revealed) return;
    cell.flagged = !cell.flagged;
    updateButtonVisual(currentLayer, r, c);
    updateMineLabel();
    checkWinCondition();
}

void Widget::revealCell(int layer, int r, int c)
{
    Cell &cell = board[layer][r][c];
    if (cell.revealed || cell.flagged) return;
    cell.revealed = true;
    revealedCells++;
    // update current layer button visual
    if (layer == currentLayer) updateButtonVisual(layer, r, c);

    if (cell.adj == 0) {
        revealNeighborsIfEmpty(layer, r, c);
    }
}

void Widget::revealNeighborsIfEmpty(int layer, int r, int c)
{
    // BFS stack
    QVector<QPair<QPair<int,int>,int>> stack; // ((r,c), layer)
    stack.append({{r,c}, layer});

    while (!stack.isEmpty()) {
        auto item = stack.takeLast();
        int cl = item.second;
        int cr = item.first.first;
        int cc = item.first.second;

        for (int dl = -1; dl <= 1; ++dl)
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    int nl = cl + dl, nr = cr + dr, nc = cc + dc;
                    if (!inBounds(nl, nr, nc)) continue;
                    Cell &ncell = board[nl][nr][nc];
                    if (ncell.revealed || ncell.flagged) continue;
                    if (ncell.isMine) continue;
                    ncell.revealed = true;
                    revealedCells++;
                    if (nl == currentLayer) updateButtonVisual(nl, nr, nc);
                    if (ncell.adj == 0) {
                        stack.append({{nr,nc}, nl});
                    }
                }
    }
}

void Widget::updateButtonVisual(int layer, int r, int c)
{
    // Only update the displayed layer's buttons
    if (layer != currentLayer) return;
    Cell &cell = board[layer][r][c];
    CellButton *btn = buttonGrid[r][c];
    if (!btn) return;

    QColor layerColor = getLayerColor(layer, cell.revealed);
    QString baseStyle = QString("QPushButton { background-color: %1; color: %2; border: 1px solid #000; font-weight: bold; font-size: 12px; }")
                        .arg(layerColor.name())
                        .arg(cell.revealed ? "#000000" : "#ffffff");

    if (cell.revealed) {
        btn->setEnabled(false);
        if (cell.isMine) {
            btn->setText("💣");
            btn->setStyleSheet(baseStyle + " QPushButton { font-size: 18px; }");
        } else if (cell.adj > 0) {
            btn->setText(QString::number(cell.adj));
            // Different colors for different numbers
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

void Widget::updateMineLabel()
{
    // count remaining = mineCount - flagged (global)
    int flagged = 0;
    for (int l = 0; l < layers; ++l)
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (board[l][r][c].flagged) ++flagged;
    int remain = mineCount - flagged;
    mineLabel->setText(QString("Mines: %1").arg(remain));
}

void Widget::showLayer(int idx)
{
    if (idx < 0 || idx >= layers) return;
    currentLayer = idx;

    // rebuild visible buttons: easiest is to rebuild the grid widgets
    // but we choose to update texts of existing buttons
    // For simplicity we rebuild (clear + build) so visuals match model.
    clearBoardButtons();
    buildBoardButtons();

    // update all visible cells in this layer
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            updateButtonVisual(currentLayer, r, c);
}

void Widget::prevLayer()
{
    int idx = layerBox->currentIndex();
    if (idx > 0) layerBox->setCurrentIndex(idx - 1);
}

void Widget::nextLayer()
{
    int idx = layerBox->currentIndex();
    if (idx + 1 < layerBox->count()) layerBox->setCurrentIndex(idx + 1);
}

void Widget::onTimerTick()
{
    ++elapsedSeconds;
    timeLabel->setText(QString("Time: %1").arg(elapsedSeconds));
}

void Widget::checkWinCondition()
{
    const int totalCells = rows * cols * layers;
    if (revealedCells == totalCells - mineCount) {
        gameOver = true;
        timer->stop();
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🎉 Victory! 🎉");
        msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 Congratulations! 🏆</h2><p>You cleared the field in %1 seconds.</p>").arg(elapsedSeconds));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
    } else {
        // also optional: if all mines flagged correctly -> win
        // check if number of flagged == mineCount and all flagged are mines
        int flagged = 0;
        int correctFlags = 0;
        for (int l = 0; l < layers; ++l)
            for (int r = 0; r < rows; ++r)
                for (int c = 0; c < cols; ++c)
                    if (board[l][r][c].flagged) {
                        ++flagged;
                        if (board[l][r][c].isMine) ++correctFlags;
                    }
        if (flagged == mineCount && correctFlags == mineCount) {
            gameOver = true;
            timer->stop();
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("🎉 Victory! 🎉");
            msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 All mines flagged correctly! 🏆</h2><p>Time: %1 seconds.</p>").arg(elapsedSeconds));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
            msgBox.exec();
        }
    }
}

QColor Widget::getLayerColor(int layer, bool revealed) const
{
    // Generate different colors for each layer
    // Darker for unrevealed, lighter for revealed
    const QVector<QColor> layerBaseColors = {
        QColor(70, 130, 180),   // Steel Blue - Layer 0
        QColor(160, 82, 45),    // Sienna - Layer 1
        QColor(85, 107, 47),    // Dark Olive Green - Layer 2
        QColor(139, 69, 19),    // Saddle Brown - Layer 3
        QColor(72, 61, 139),    // Dark Slate Blue - Layer 4
        QColor(47, 79, 79),     // Dark Slate Gray - Layer 5
        QColor(128, 0, 0),      // Maroon - Layer 6
        QColor(0, 100, 0),      // Dark Green - Layer 7
        QColor(75, 0, 130),     // Indigo - Layer 8
        QColor(105, 105, 105)   // Dim Gray - Layer 9
    };
    
    int colorIndex = layer % layerBaseColors.size();
    QColor baseColor = layerBaseColors[colorIndex];
    
    if (revealed) {
        // Lighter version for revealed cells
        return baseColor.lighter(180);
    } else {
        // Darker version for unrevealed cells
        return baseColor.darker(120);
    }
}

void Widget::showExplosionEffect(int r, int c)
{
    // Visual feedback: make the button flash with explosion animation
    CellButton *btn = buttonGrid[r][c];
    if (!btn) return;
    
    // Create a simple animation effect by changing background color
    btn->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; border: 3px solid #ffff00; font-size: 20px; }");
    btn->setText("💥");
    
    // Force UI update
    QApplication::processEvents();
}
