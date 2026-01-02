#include "game1widget.h"
#include <QApplication>

Game1Widget::Game1Widget(QWidget *parent) : QWidget(parent)
{
    // 初始化音效，使用嵌入的資料 URL（簡單的嗶聲）
    //revealSound = new QSoundEffect(this);
    explosionSound = new QSoundEffect(this);
    winSound = new QSoundEffect(this);
    startSound = new QSoundEffect(this);

    // 設定音量
    //revealSound->setVolume(0.5);
    explosionSound->setVolume(0.7);
    winSound->setVolume(0.7);
    startSound->setVolume(0.7);

    explosionSound->setSource(QUrl("qrc:/sounds/boom.wav"));
    winSound->setSource(QUrl("qrc:/sounds/win.wav"));
    startSound->setSource(QUrl("qrc:/sounds/start.wav"));

    buildUI();
    newGame();
}

Game1Widget::~Game1Widget()
{
}

void Game1Widget::buildUI()
{
    // 設定視窗屬性
    setWindowTitle("Multi-Layer Minesweeper");
    setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; font-family: Arial; }");

    // 上方控制項
    QLabel *lblRows = new QLabel("橫排:");
    lblRows->setStyleSheet("font-weight: bold;");
    spinRows = new QSpinBox(this);
    spinRows->setRange(5, 20);
    spinRows->setValue(rows);
    spinRows->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblCols = new QLabel("直排:");
    lblCols->setStyleSheet("font-weight: bold;");
    spinCols = new QSpinBox(this);
    spinCols->setRange(5, 20);
    spinCols->setValue(cols);
    spinCols->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblLayers = new QLabel("層數:");
    lblLayers->setStyleSheet("font-weight: bold;");
    spinLayers = new QSpinBox(this);
    spinLayers->setRange(1, 10);
    spinLayers->setValue(layers);
    spinLayers->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    QLabel *lblMines = new QLabel("炸彈數:");
    lblMines->setStyleSheet("font-weight: bold;");
    spinMines = new QSpinBox(this);
    spinMines->setRange(1, 200);
    spinMines->setValue(mineCount);
    spinMines->setStyleSheet("QSpinBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; }");

    newGameBtn = new QPushButton("開新遊戲", this);
    newGameBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #45a049; }");
    connect(newGameBtn, &QPushButton::clicked, this, &Game1Widget::newGame);

    hintButton = new QPushButton("提示💡", this);
    hintButton->setStyleSheet("QPushButton { background-color: #FFA500; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #FF8C00; }");
    connect(hintButton, &QPushButton::clicked, this, &Game1Widget::giveHint);

    backButton = new QPushButton("返回選單", this);
    backButton->setStyleSheet("QPushButton { background-color: #888; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #777; }");
    connect(backButton, &QPushButton::clicked, this, &Game1Widget::backToMenu);

    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    ctrlLayout->addWidget(backButton);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(lblRows);
    ctrlLayout->addWidget(spinRows);
    ctrlLayout->addWidget(lblCols);
    ctrlLayout->addWidget(spinCols);
    ctrlLayout->addWidget(lblLayers);
    ctrlLayout->addWidget(spinLayers);
    ctrlLayout->addWidget(lblMines);
    ctrlLayout->addWidget(spinMines);
    ctrlLayout->addWidget(newGameBtn);
    ctrlLayout->addWidget(hintButton);

    // 層控制
    prevBtn = new QPushButton("上一層", this);
    prevBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    nextBtn = new QPushButton("下一層", this);
    nextBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    layerBox = new QComboBox(this);
    layerBox->setStyleSheet("QComboBox { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555; padding: 3px; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border: none; }");
    connect(prevBtn, &QPushButton::clicked, this, &Game1Widget::prevLayer);
    connect(nextBtn, &QPushButton::clicked, this, &Game1Widget::nextLayer);
    connect(layerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Game1Widget::showLayer);

    QHBoxLayout *layerLayout = new QHBoxLayout();
    layerLayout->addWidget(prevBtn);
    layerLayout->addWidget(layerBox);
    layerLayout->addWidget(nextBtn);

    // 資訊顯示
    mineLabel = new QLabel("Mines: 0", this);
    mineLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    timeLabel = new QLabel("Time: 0", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(mineLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(timeLabel);

    // 棋盤區域
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);

    // 計時器
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Widget::onTimerTick);

    // 主版面配置
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(ctrlLayout);
    main->addLayout(layerLayout);
    main->addLayout(infoLayout);
    main->addLayout(boardLayout);
    main->setContentsMargins(10, 10, 10, 10);
    setLayout(main);
}

void Game1Widget::newGame()
{
    // 播放遊戲開始音效
    if (startSound->source().isEmpty()) {
        QApplication::beep();
    } else {
        startSound->play();
    }
    // 讀取 UI 設定
    rows = spinRows->value();
    cols = spinCols->value();
    layers = spinLayers->value();
    mineCount = spinMines->value();

    if (mineCount >= rows * cols * layers) {
        QMessageBox::warning(this, "Invalid", "Too many mines for the board size.");
        return;
    }

    // 重設遊戲狀態
    firstClick = true;
    gameOver = false;
    revealedCells = 0;
    elapsedSeconds = 0;
    timer->stop();
    timeLabel->setText("Time: 0");

    // 重建層選擇器
    resetModel();

    // 建立棋盤按鈕
    layerBox->clear();
    for (int l = 0; l < layers; ++l) layerBox->addItem(QString("Layer %1").arg(l+1));
    currentLayer = 0;
    layerBox->setCurrentIndex(currentLayer);

    // 建立 UI 按鈕
    clearBoardButtons();
    buildBoardButtons();

    updateMineLabel();
}

// 重設遊戲模型（建立多層棋盤）
void Game1Widget::resetModel()
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

void Game1Widget::buildBoardButtons()
{
    // 建立當前層的按鈕格子（重新建立但共用訊號）
    buttonGrid.clear();
    buttonGrid.resize(rows);
    const int btnSize = 36;

    // 套用層顏色
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

            connect(btn, &CellButton::leftClicked, this, &Game1Widget::onLeftClick);
            connect(btn, &CellButton::rightClicked, this, &Game1Widget::onRightClick);
        }
    }
}

// 清除棋盤上的按鈕
void Game1Widget::clearBoardButtons()
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

void Game1Widget::placeMinesSafely(int safeLayer, int safeR, int safeC)
{
    // 隨機放置地雷，但避開安全格（第一次點擊的格子及其鄰近格）
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

// 計算每個格子的鄰近地雷數
void Game1Widget::calculateAdjacents()
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

// 檢查是否在棋盤範圍內
bool Game1Widget::inBounds(int l, int r, int c) const {
    return l >= 0 && l < layers && r >= 0 && r < rows && c >= 0 && c < cols;
}

// 左鍵點擊事件
void Game1Widget::onLeftClick(int r, int c)
{
    if (gameOver) return;
    if (!inBounds(currentLayer, r, c)) return;

    if (firstClick) {
        // 第一次點擊：放置地雷並計算鄰近數
        placeMinesSafely(currentLayer, r, c);
        calculateAdjacents();
        firstClick = false;
        timer->start();
    }

    Cell &cell = board[currentLayer][r][c];
    if (cell.flagged || cell.revealed) return;

    // 第一次點擊：放置地雷並計算鄰近數
    if (cell.isMine) {
        // 播放爆炸音效
        if (explosionSound->source().isEmpty()) {
            QApplication::beep();
        } else {
            explosionSound->play();
        }

        // 顯示爆炸
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

        // 顯示遊戲結束訊息
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("💥 Game Over 💥");
        msgBox.setText("<h2 style='color: #ff0000;'>💣 BOOM! 💣</h2><p>你點擊到了炸彈!</p>");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #555; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
        return;
    }

    // 揭露格子
    revealCell(currentLayer, r, c);
    updateButtonVisual(currentLayer, r, c);
    checkWinCondition();
}

// 右鍵點擊事件
void Game1Widget::onRightClick(int r, int c)
{
    if (gameOver || firstClick) return;
    if (!inBounds(currentLayer, r, c)) return;
    Cell &cell = board[currentLayer][r][c];
    // 插旗或取消旗標
    if (cell.revealed) return;
    cell.flagged = !cell.flagged;
    updateButtonVisual(currentLayer, r, c);
    updateMineLabel();
    checkWinCondition();
}

// 揭露格子
void Game1Widget::revealCell(int layer, int r, int c)
{
    Cell &cell = board[layer][r][c];
    if (cell.revealed || cell.flagged) return;
    // 若鄰近地雷數為 0，則展開鄰近格
    cell.revealed = true;
    revealedCells++;
    // 更新本層的按鈕
    if (layer == currentLayer) updateButtonVisual(layer, r, c);

    if (cell.adj == 0) {
        revealNeighborsIfEmpty(layer, r, c);
    }
}

// 當格子為空時，展開鄰近格
void Game1Widget::revealNeighborsIfEmpty(int layer, int r, int c)
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

// 更新按鈕的顯示（依據是否揭露、是否插旗、是否地雷）
void Game1Widget::updateButtonVisual(int layer, int r, int c)
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
            // 每個數字不同顏色
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

// 更新剩餘地雷數顯示
void Game1Widget::updateMineLabel()
{
    // 剩餘數量 = 總數 - 已插旗
    int flagged = 0;
    for (int l = 0; l < layers; ++l)
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (board[l][r][c].flagged) ++flagged;
    int remain = mineCount - flagged;
    mineLabel->setText(QString("Mines: %1").arg(remain));
}

// 顯示指定層，重建按鈕並更新顯示
void Game1Widget::showLayer(int idx)
{
    if (idx < 0 || idx >= layers) return;
    currentLayer = idx;

    clearBoardButtons();
    buildBoardButtons();

    // 更新本層所有按鈕狀態
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            updateButtonVisual(currentLayer, r, c);
}

// 切換到上一層
void Game1Widget::prevLayer()
{
    int idx = layerBox->currentIndex();
    if (idx > 0) layerBox->setCurrentIndex(idx - 1);
}

// 切換到下一層
void Game1Widget::nextLayer()
{
    int idx = layerBox->currentIndex();
    if (idx + 1 < layerBox->count()) layerBox->setCurrentIndex(idx + 1);
}

// 每秒更新計時器顯示
void Game1Widget::onTimerTick()
{
    ++elapsedSeconds;
    timeLabel->setText(QString("Time: %1").arg(elapsedSeconds));
}

// 檢查是否勝利（揭露所有非地雷格或正確插旗所有地雷）
void Game1Widget::checkWinCondition()
{
    const int totalCells = rows * cols * layers;
    if (revealedCells == totalCells - mineCount) {
        gameOver = true;
        timer->stop();
        // 播放過關音效
        if (winSound->source().isEmpty()) {
            QApplication::beep();
        } else {
            winSound->play();
        }
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🎉 通關! 🎉");
        msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 恭喜! 🏆</h2><p>您在 %1 秒內成功清空炸彈.</p>").arg(elapsedSeconds));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
    } else {
        // 檢查是否全標記，並且標記的都是地雷
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
            // 播放過關音效
            if (winSound->source().isEmpty()) {
                QApplication::beep();
            } else {
                winSound->play();
            }
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("🎉 通關! 🎉");
            msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 已標示所有地雷! 🏆</h2><p>時間: %1 秒.</p>").arg(elapsedSeconds));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
            msgBox.exec();
        }
    }
}

// 產生不同層的顏色
QColor Game1Widget::getLayerColor(int layer, bool revealed) const
{
    const QVector<QColor> layerBaseColors = {
        QColor(70, 130, 180),   // Layer 0
        QColor(160, 82, 45),    // Layer 1
        QColor(85, 107, 47),    // Layer 2
        QColor(139, 69, 19),    // Layer 3
        QColor(72, 61, 139),    // Layer 4
        QColor(47, 79, 79),     // Layer 5
        QColor(128, 0, 0),      // Layer 6
        QColor(0, 100, 0),      // Layer 7
        QColor(75, 0, 130),     // Layer 8
        QColor(105, 105, 105)   // Layer 9
    };

    int colorIndex = layer % layerBaseColors.size();
    QColor baseColor = layerBaseColors[colorIndex];

    // 已揭露：較亮；未揭露：較暗
    if (revealed) {
        return baseColor.lighter(180);
    } else {
        return baseColor.darker(120);
    }
}

// 視覺回饋：讓按鈕閃爍並顯示爆炸動畫
void Game1Widget::showExplosionEffect(int r, int c)
{
    CellButton *btn = buttonGrid[r][c];
    if (!btn) return;

    // 改變按鈕圖示和顏色，使他有爆炸感
    btn->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; border: 3px solid #ffff00; font-size: 20px; }");
    btn->setText("💥");

    QApplication::processEvents();
}

// 提示功能：揭露當前層的一個隨機安全格
void Game1Widget::giveHint()
{
    if (gameOver || firstClick) return;

    // 收集當前層所有未揭露且安全的格子
    QVector<QPair<int, int>> safeCells;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell &cell = board[currentLayer][r][c];
            if (!cell.revealed && !cell.flagged && !cell.isMine) {
                safeCells.append({r, c});
            }
        }
    }

    if (safeCells.isEmpty()) {
        QMessageBox::information(this, "提示", "當前層沒有可以揭露的安全格了!");
        return;
    }

    // 隨機選擇一個安全格揭露
    int idx = QRandomGenerator::global()->bounded(safeCells.size());
    int r = safeCells[idx].first;
    int c = safeCells[idx].second;

    // 揭露這個格子
    revealCell(currentLayer, r, c);
    updateButtonVisual(currentLayer, r, c);
    checkWinCondition();

    // 播放音效
    if (startSound && !startSound->source().isEmpty()) {
        startSound->play();
    }
}
