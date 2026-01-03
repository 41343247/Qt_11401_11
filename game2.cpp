#include "game2.h"
#include <QApplication>
#include <algorithm>
#include <QPainter>
#include <QFrame>

Game2::Game2(QWidget *parent) : QWidget(parent)
{
    // 初始化音效
    explosionSound = new QSoundEffect(this);
    winSound = new QSoundEffect(this);
    startSound = new QSoundEffect(this);

    explosionSound->setVolume(0.7);
    winSound->setVolume(0.7);
    startSound->setVolume(0.7);

    explosionSound->setSource(QUrl("qrc:/sounds/boom.wav"));
    winSound->setSource(QUrl("qrc:/sounds/win.wav"));
    startSound->setSource(QUrl("qrc:/sounds/start.wav"));

    buildUI();
    newGame();
}

Game2::~Game2()
{
}

void Game2::buildUI()
{
    setWindowTitle("空心六面體掃雷");
    setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; font-family: Arial; }");

    // 上方控制項
    newGameBtn = new QPushButton("開新遊戲", this);
    newGameBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #45a049; }");
    connect(newGameBtn, &QPushButton::clicked, this, &Game2::newGame);

    hintBtn = new QPushButton("提示", this);
    hintBtn->setStyleSheet("QPushButton { background-color: #FFA500; color: white; border: none; padding: 5px 15px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #FF8C00; }");
    connect(hintBtn, &QPushButton::clicked, this, &Game2::onHintClicked);

    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    ctrlLayout->addWidget(newGameBtn);
    ctrlLayout->addWidget(hintBtn);
    ctrlLayout->addStretch();

    // 面控制
    prevBtn = new QPushButton("上一面", this);
    prevBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    nextBtn = new QPushButton("下一面", this);
    nextBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px 10px; font-weight: bold; border-radius: 3px; } QPushButton:hover { background-color: #666; }");
    faceLabel = new QLabel("當前面: 上", this);
    faceLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    connect(prevBtn, &QPushButton::clicked, this, &Game2::prevFace);
    connect(nextBtn, &QPushButton::clicked, this, &Game2::nextFace);

    QHBoxLayout *faceLayout = new QHBoxLayout();
    faceLayout->addWidget(prevBtn);
    faceLayout->addWidget(faceLabel);
    faceLayout->addWidget(nextBtn);

    // 資訊顯示
    mineLabel = new QLabel("地雷: 0", this);
    mineLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD700;");
    timeLabel = new QLabel("時間: 0", this);
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #87CEEB;");

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(mineLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(timeLabel);

    // 棋盤區域
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(2);

    // 相鄰面預覽區域
    QLabel *adjacentLabel = new QLabel("相鄰面狀態:", this);
    adjacentLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    
    QHBoxLayout *adjacentLayout = new QHBoxLayout();
    adjacentLayout->addWidget(adjacentLabel);
    
    // 創建4個相鄰面的縮略標籤
    for (int i = 0; i < 4; ++i) {
        QLabel *label = new QLabel(this);
        label->setFixedSize(100, 100);
        label->setFrameStyle(QFrame::Box);
        label->setStyleSheet("QLabel { border: 1px solid #555; background-color: #3c3c3c; }");
        label->setScaledContents(true);
        adjacentFaceLabels.append(label);
        adjacentLayout->addWidget(label);
    }
    adjacentLayout->addStretch();

    // 計時器
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Game2::onTimerTick);

    // 主版面配置
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(ctrlLayout);
    main->addLayout(faceLayout);
    main->addLayout(infoLayout);
    main->addLayout(boardLayout);
    main->addLayout(adjacentLayout);
    main->setContentsMargins(10, 10, 10, 10);
    setLayout(main);
}

void Game2::newGame()
{
    if (startSound->source().isEmpty()) {
        QApplication::beep();
    } else {
        startSound->play();
    }

    firstClick = true;
    gameOver = false;
    revealedCells = 0;
    elapsedSeconds = 0;
    timer->stop();
    timeLabel->setText("時間: 0");

    resetModel();
    currentFace = 0;
    faceLabel->setText("當前面: " + getFaceName(currentFace));

    clearBoardButtons();
    buildBoardButtons();
    updateAdjacentFaceLabels();
    updateMineLabel();
}

void Game2::resetModel()
{
    faces.clear();
    faces.resize(6); // 6個面
    for (int f = 0; f < 6; ++f) {
        faces[f].resize(faceSize);
        for (int r = 0; r < faceSize; ++r) {
            faces[f][r].resize(faceSize);
            for (int c = 0; c < faceSize; ++c) {
                faces[f][r][c] = Cell2{};
            }
        }
    }
}

void Game2::buildBoardButtons()
{
    buttonGrid.clear();
    buttonGrid.resize(faceSize);
    const int btnSize = 36;

    for (int r = 0; r < faceSize; ++r) {
        buttonGrid[r].resize(faceSize);
        for (int c = 0; c < faceSize; ++c) {
            CellButton2 *btn = new CellButton2(r, c, this);
            btn->setFixedSize(btnSize, btnSize);
            btn->setText("");
            btn->setFocusPolicy(Qt::NoFocus);

            QColor faceColor = getFaceColor(currentFace, false);
            btn->setStyleSheet(QString("QPushButton { background-color: %1; color: #ffffff; border: 1px solid #000; font-weight: bold; font-size: 12px; } QPushButton:hover { border: 2px solid #fff; }").arg(faceColor.name()));

            boardLayout->addWidget(btn, r, c);
            buttonGrid[r][c] = btn;

            connect(btn, &CellButton2::leftClicked, this, &Game2::onLeftClick);
            connect(btn, &CellButton2::rightClicked, this, &Game2::onRightClick);
        }
    }
}

void Game2::clearBoardButtons()
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

void Game2::placeMinesSafely(int safeFace, int safeR, int safeC)
{
    QVector<int> idxs;
    const int total = 6 * faceSize * faceSize;
    idxs.reserve(total);
    
    for (int f = 0; f < 6; ++f) {
        for (int r = 0; r < faceSize; ++r) {
            for (int c = 0; c < faceSize; ++c) {
                // 避開安全格及其鄰近格
                bool isSafe = false;
                for (int dr = -1; dr <= 1 && !isSafe; ++dr) {
                    for (int dc = -1; dc <= 1 && !isSafe; ++dc) {
                        int nr = safeR + dr, nc = safeC + dc;
                        if (f == safeFace && nr == r && nc == c) {
                            isSafe = true;
                            break;
                        }
                    }
                }
                if (!isSafe) idxs.append(f * faceSize * faceSize + r * faceSize + c);
            }
        }
    }

    std::shuffle(idxs.begin(), idxs.end(), *QRandomGenerator::global());
    for (int i = 0; i < mineCount && i < idxs.size(); ++i) {
        int val = idxs[i];
        int f = val / (faceSize * faceSize);
        int rem = val % (faceSize * faceSize);
        int r = rem / faceSize;
        int c = rem % faceSize;
        faces[f][r][c].isMine = true;
    }
}

void Game2::calculateAdjacents()
{
    // 對每個面的每個格子計算鄰近地雷數
    // 注意：邊界格子可能連接到其他面（暫時簡化，只計算同一面內的）
    for (int f = 0; f < 6; ++f) {
        for (int r = 0; r < faceSize; ++r) {
            for (int c = 0; c < faceSize; ++c) {
                if (faces[f][r][c].isMine) {
                    faces[f][r][c].adj = -1;
                    continue;
                }
                int cnt = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (inBounds(f, nr, nc) && faces[f][nr][nc].isMine) ++cnt;
                    }
                }
                faces[f][r][c].adj = cnt;
            }
        }
    }
}

bool Game2::inBounds(int face, int r, int c) const
{
    return face >= 0 && face < 6 && r >= 0 && r < faceSize && c >= 0 && c < faceSize;
}

void Game2::onLeftClick(int r, int c)
{
    if (gameOver) return;
    if (!inBounds(currentFace, r, c)) return;

    if (firstClick) {
        placeMinesSafely(currentFace, r, c);
        calculateAdjacents();
        firstClick = false;
        timer->start();
    }

    Cell2 &cell = faces[currentFace][r][c];
    if (cell.flagged || cell.revealed) return;

    if (cell.isMine) {
        if (explosionSound->source().isEmpty()) {
            QApplication::beep();
        } else {
            explosionSound->play();
        }

        showExplosionEffect(r, c);

        // 顯示所有地雷
        for (int f = 0; f < 6; ++f) {
            for (int rr = 0; rr < faceSize; ++rr) {
                for (int cc = 0; cc < faceSize; ++cc) {
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
        msgBox.setWindowTitle("💥 遊戲結束 💥");
        msgBox.setText("<h2 style='color: #ff0000;'>💣 爆炸! 💣</h2><p>你點擊到了地雷!</p>");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #555; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
        return;
    }

    revealCell(currentFace, r, c);
    updateButtonVisual(currentFace, r, c);
    updateAdjacentFaceLabels();
    checkWinCondition();
}

void Game2::onRightClick(int r, int c)
{
    if (gameOver || firstClick) return;
    if (!inBounds(currentFace, r, c)) return;
    
    Cell2 &cell = faces[currentFace][r][c];
    if (cell.revealed) return;
    
    cell.flagged = !cell.flagged;
    updateButtonVisual(currentFace, r, c);
    updateMineLabel();
    checkWinCondition();
}

void Game2::revealCell(int face, int r, int c)
{
    Cell2 &cell = faces[face][r][c];
    if (cell.revealed || cell.flagged) return;
    
    cell.revealed = true;
    revealedCells++;
    
    if (face == currentFace) updateButtonVisual(face, r, c);

    if (cell.adj == 0) {
        revealNeighborsIfEmpty(face, r, c);
    }
}

void Game2::revealNeighborsIfEmpty(int face, int r, int c)
{
    QVector<QPair<QPair<int,int>,int>> stack;
    stack.append({{r,c}, face});

    while (!stack.isEmpty()) {
        auto item = stack.takeLast();
        int cf = item.second;
        int cr = item.first.first;
        int cc = item.first.second;

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = cr + dr, nc = cc + dc;
                if (!inBounds(cf, nr, nc)) continue;
                
                Cell2 &ncell = faces[cf][nr][nc];
                if (ncell.revealed || ncell.flagged) continue;
                if (ncell.isMine) continue;
                
                ncell.revealed = true;
                revealedCells++;
                if (cf == currentFace) updateButtonVisual(cf, nr, nc);
                
                if (ncell.adj == 0) {
                    stack.append({{nr,nc}, cf});
                }
            }
        }
    }
}

void Game2::updateButtonVisual(int face, int r, int c)
{
    if (face != currentFace) return;
    
    Cell2 &cell = faces[face][r][c];
    CellButton2 *btn = buttonGrid[r][c];
    if (!btn) return;

    QColor faceColor = getFaceColor(face, cell.revealed);
    QString baseStyle = QString("QPushButton { background-color: %1; color: %2; border: 1px solid #000; font-weight: bold; font-size: 12px; }")
                            .arg(faceColor.name())
                            .arg(cell.revealed ? "#000000" : "#ffffff");

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

void Game2::updateMineLabel()
{
    int flagged = 0;
    for (int f = 0; f < 6; ++f) {
        for (int r = 0; r < faceSize; ++r) {
            for (int c = 0; c < faceSize; ++c) {
                if (faces[f][r][c].flagged) ++flagged;
            }
        }
    }
    int remain = mineCount - flagged;
    mineLabel->setText(QString("地雷: %1").arg(remain));
}

void Game2::showFace(int idx)
{
    if (idx < 0 || idx >= 6) return;
    currentFace = idx;
    faceLabel->setText("當前面: " + getFaceName(currentFace));

    clearBoardButtons();
    buildBoardButtons();

    for (int r = 0; r < faceSize; ++r) {
        for (int c = 0; c < faceSize; ++c) {
            updateButtonVisual(currentFace, r, c);
        }
    }
    
    updateAdjacentFaceLabels();
}

void Game2::prevFace()
{
    int idx = currentFace;
    if (idx > 0) showFace(idx - 1);
}

void Game2::nextFace()
{
    int idx = currentFace;
    if (idx + 1 < 6) showFace(idx + 1);
}

void Game2::onTimerTick()
{
    ++elapsedSeconds;
    timeLabel->setText(QString("時間: %1").arg(elapsedSeconds));
}

void Game2::checkWinCondition()
{
    const int totalCells = 6 * faceSize * faceSize;
    if (revealedCells == totalCells - mineCount) {
        gameOver = true;
        timer->stop();
        
        if (winSound->source().isEmpty()) {
            QApplication::beep();
        } else {
            winSound->play();
        }
        
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🎉 通關! 🎉");
        msgBox.setText(QString("<h2 style='color: #00ff00;'>🏆 恭喜! 🏆</h2><p>您在 %1 秒內成功清除所有地雷.</p>").arg(elapsedSeconds));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: #ffffff; } QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }");
        msgBox.exec();
    }
}

QColor Game2::getFaceColor(int face, bool revealed) const
{
    const QVector<QColor> faceBaseColors = {
        QColor(255, 99, 71),    // 上 - 番茄紅
        QColor(135, 206, 235),  // 下 - 天空藍
        QColor(60, 179, 113),   // 前 - 海綠色
        QColor(255, 215, 0),    // 後 - 金色
        QColor(147, 112, 219),  // 左 - 中紫色
        QColor(255, 165, 0)     // 右 - 橙色
    };

    QColor baseColor = faceBaseColors[face];
    
    if (revealed) {
        return baseColor.lighter(180);
    } else {
        return baseColor.darker(120);
    }
}

void Game2::showExplosionEffect(int r, int c)
{
    CellButton2 *btn = buttonGrid[r][c];
    if (!btn) return;

    btn->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; border: 3px solid #ffff00; font-size: 20px; }");
    btn->setText("💥");

    QApplication::processEvents();
}

QString Game2::getFaceName(int face) const
{
    const QVector<QString> faceNames = {"上", "下", "前", "後", "左", "右"};
    return faceNames[face];
}

void Game2::updateAdjacentFaceLabels()
{
    // 根據當前面顯示相鄰的4個面
    // 簡化版：顯示除當前面和對面之外的4個面
    QVector<int> adjacentFaces;
    
    // 面的對應關係: 上(0)↔下(1), 前(2)↔後(3), 左(4)↔右(5)
    int oppositeFace = -1;
    if (currentFace == 0) oppositeFace = 1;
    else if (currentFace == 1) oppositeFace = 0;
    else if (currentFace == 2) oppositeFace = 3;
    else if (currentFace == 3) oppositeFace = 2;
    else if (currentFace == 4) oppositeFace = 5;
    else if (currentFace == 5) oppositeFace = 4;
    
    for (int i = 0; i < 6; ++i) {
        if (i != currentFace && i != oppositeFace) {
            adjacentFaces.append(i);
        }
    }
    
    // 更新4個相鄰面的縮略圖
    for (int i = 0; i < qMin(4, adjacentFaces.size()); ++i) {
        QPixmap thumbnail = renderFaceThumbnail(adjacentFaces[i]);
        adjacentFaceLabels[i]->setPixmap(thumbnail);
        adjacentFaceLabels[i]->setToolTip(getFaceName(adjacentFaces[i]));
    }
}

QPixmap Game2::renderFaceThumbnail(int face)
{
    // 渲染面的縮略圖
    const int thumbSize = 100;
    const int cellSize = thumbSize / faceSize;
    
    QPixmap pixmap(thumbSize, thumbSize);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    for (int r = 0; r < faceSize; ++r) {
        for (int c = 0; c < faceSize; ++c) {
            Cell2 &cell = faces[face][r][c];
            QColor color = getFaceColor(face, cell.revealed);
            
            painter.fillRect(c * cellSize, r * cellSize, cellSize, cellSize, color);
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(c * cellSize, r * cellSize, cellSize, cellSize);
            
            if (cell.revealed) {
                painter.setPen(Qt::black);
                if (cell.isMine) {
                    painter.drawText(c * cellSize, r * cellSize, cellSize, cellSize, Qt::AlignCenter, "💣");
                } else if (cell.adj > 0) {
                    painter.drawText(c * cellSize, r * cellSize, cellSize, cellSize, Qt::AlignCenter, QString::number(cell.adj));
                }
            } else if (cell.flagged) {
                painter.setPen(Qt::red);
                painter.drawText(c * cellSize, r * cellSize, cellSize, cellSize, Qt::AlignCenter, "⚑");
            }
        }
    }
    
    // 添加面名稱標籤
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(2, 15, getFaceName(face));
    
    return pixmap;
}

void Game2::onHintClicked()
{
    if (gameOver || firstClick) {
        QMessageBox::information(this, "提示", "請先開始遊戲!");
        return;
    }
    
    performHint();
}

void Game2::performHint()
{
    // 找出當前面所有未揭露且非地雷的格子
    QVector<QPair<int,int>> safeCells;
    
    for (int r = 0; r < faceSize; ++r) {
        for (int c = 0; c < faceSize; ++c) {
            Cell2 &cell = faces[currentFace][r][c];
            if (!cell.revealed && !cell.flagged && !cell.isMine) {
                safeCells.append({r, c});
            }
        }
    }
    
    if (safeCells.isEmpty()) {
        QMessageBox::information(this, "提示", "當前面已無安全格可揭露!");
        return;
    }
    
    // 隨機選擇一個安全格
    int idx = QRandomGenerator::global()->bounded(safeCells.size());
    int r = safeCells[idx].first;
    int c = safeCells[idx].second;
    
    // 模擬點擊
    revealCell(currentFace, r, c);
    updateButtonVisual(currentFace, r, c);
    updateAdjacentFaceLabels();
    checkWinCondition();
    
    // 視覺回饋：短暫高亮該格子
    CellButton2 *btn = buttonGrid[r][c];
    if (btn) {
        btn->setStyleSheet(btn->styleSheet() + " QPushButton { border: 3px solid #00ff00; }");
        QTimer::singleShot(500, [btn, this, r, c]() {
            updateButtonVisual(currentFace, r, c);
        });
    }
}
