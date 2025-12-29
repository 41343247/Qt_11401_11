#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QPropertyAnimation>

// 每個格子的資料結構
struct Cell {
    bool isMine = false;
    bool revealed = false;
    bool flagged = false;
    int adj = 0; // 鄰近地雷數
};

// 自訂按鈕，用以辨識座標並捕捉左右鍵
class CellButton : public QPushButton {
    Q_OBJECT
public:
    CellButton(int r, int c, QWidget *parent = nullptr)
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
        // do not call base to avoid default clicked() emission
    }
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newGame();                       // 重新開始
    void onLeftClick(int r, int c);       // 左鍵揭露（當前層）
    void onRightClick(int r, int c);      // 右鍵插旗（當前層）
    void showLayer(int idx);              // 顯示指定層
    void prevLayer();                     // 上一層
    void nextLayer();                     // 下一層
    void onTimerTick();                   // 計時器
private:
    // UI
    QGridLayout *boardLayout;
    QVector<QVector<CellButton*>> buttonGrid; // 表示當前層的按鈕引用
    QComboBox *layerBox;
    QPushButton *prevBtn;
    QPushButton *nextBtn;
    QLabel *mineLabel;
    QLabel *timeLabel;
    QPushButton *newGameBtn;
    QSpinBox *spinRows;
    QSpinBox *spinCols;
    QSpinBox *spinLayers;
    QSpinBox *spinMines;

    QTimer *timer;
    int elapsedSeconds = 0;

    // Sound effects
    QSoundEffect *revealSound;
    QSoundEffect *explosionSound;
    QSoundEffect *winSound;

    // game model
    QVector<QVector<QVector<Cell>>> board; // [layer][row][col]
    int rows = 8;
    int cols = 8;
    int layers = 3;
    int mineCount = 10;
    int currentLayer = 0;
    bool firstClick = true;
    bool gameOver = false;
    int revealedCells = 0;

    // helpers
    void buildUI();
    void buildBoardButtons(); // 建立按鈕網格（UI）
    void clearBoardButtons();
    void resetModel();
    void placeMinesSafely(int safeLayer, int safeR, int safeC);
    void calculateAdjacents();
    void revealCell(int layer, int r, int c);
    void revealNeighborsIfEmpty(int layer, int r, int c);
    bool inBounds(int l, int r, int c) const;
    void updateButtonVisual(int layer, int r, int c);
    void updateMineLabel();
    void checkWinCondition();
    QColor getLayerColor(int layer, bool revealed) const;
    void showExplosionEffect(int r, int c);
};

#endif // WIDGET_H
