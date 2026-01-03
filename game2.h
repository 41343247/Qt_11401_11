#ifndef GAME2_H
#define GAME2_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QSoundEffect>

// 格子資料結構 (和 game1 相同)
struct Cell2 {
    bool isMine = false;
    bool revealed = false;
    bool flagged = false;
    int adj = 0; // 鄰近地雷數
};

// 自訂按鈕，用以辨識座標並捕捉左右鍵
class CellButton2 : public QPushButton {
    Q_OBJECT
public:
    CellButton2(int r, int c, QWidget *parent = nullptr)
        : QPushButton(parent), row(r), col(c) {}

    int row;
    int col;

signals:
    void leftClicked(int r, int c);
    void rightClicked(int r, int c);

protected:
    void mousePressEvent(QMouseEvent *ev) override {
        if (ev->button() == Qt::LeftButton) emit leftClicked(row, col);
        else if (ev->button() == Qt::RightButton) emit rightClicked(row, col);
    }
};

// Game2: 空心六面體掃雷遊戲
// 六個面: 上(0), 下(1), 前(2), 後(3), 左(4), 右(5)
class Game2 : public QWidget
{
    Q_OBJECT

public:
    explicit Game2(QWidget *parent = nullptr);
    ~Game2();

private slots:
    void newGame();
    void onLeftClick(int r, int c);
    void onRightClick(int r, int c);
    void showFace(int idx);
    void prevFace();
    void nextFace();
    void onTimerTick();
    void onHintClicked(); // 提示按鈕

private:
    // UI 元件
    QGridLayout *boardLayout;
    QVector<QVector<CellButton2*>> buttonGrid; // 當前面的按鈕
    QLabel *faceLabel; // 顯示當前面名稱
    QPushButton *prevBtn;
    QPushButton *nextBtn;
    QLabel *mineLabel;
    QLabel *timeLabel;
    QPushButton *newGameBtn;
    QPushButton *hintBtn; // 提示按鈕
    
    // 小型鄰面預覽
    QVector<QLabel*> adjacentFaceLabels; // 顯示相鄰面的縮略圖

    QTimer *timer;
    int elapsedSeconds = 0;

    // 音效
    QSoundEffect *explosionSound;
    QSoundEffect *winSound;
    QSoundEffect *startSound;

    // 遊戲模型
    // 6個面，每個面是一個二維網格
    QVector<QVector<QVector<Cell2>>> faces; // [face][row][col]
    int faceSize = 8; // 每個面的大小 (8x8)
    int mineCount = 20; // 總地雷數
    int currentFace = 0; // 當前顯示的面 (0-5)
    bool firstClick = true;
    bool gameOver = false;
    int revealedCells = 0;

    // 輔助函數
    void buildUI();
    void buildBoardButtons();
    void clearBoardButtons();
    void resetModel();
    void placeMinesSafely(int safeFace, int safeR, int safeC);
    void calculateAdjacents();
    void revealCell(int face, int r, int c);
    void revealNeighborsIfEmpty(int face, int r, int c);
    bool inBounds(int face, int r, int c) const;
    void updateButtonVisual(int face, int r, int c);
    void updateMineLabel();
    void checkWinCondition();
    QColor getFaceColor(int face, bool revealed) const;
    void showExplosionEffect(int r, int c);
    void updateAdjacentFaceLabels(); // 更新鄰面縮略圖
    QPixmap renderFaceThumbnail(int face); // 渲染面的縮略圖
    QString getFaceName(int face) const; // 獲取面的名稱
    void performHint(); // 執行提示動作
};

#endif // GAME2_H
