#ifndef GAME2WIDGET_H
#define GAME2WIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QScrollArea>
#include <QFrame>
#include "difficultywidget.h"

// Cell structure for Game 2
struct FaceCell {
    bool isMine = false;
    bool revealed = false;
    bool flagged = false;
    int adj = 0; // Adjacent mine count (edge-adjacent only)
};

// Custom button for face cells
class FaceCellButton : public QPushButton {
    Q_OBJECT
public:
    FaceCellButton(int r, int c, QWidget *parent = nullptr)
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

// Face structure
struct Face {
    int id;
    int rows;
    int cols;
    QVector<QVector<FaceCell>> cells;
    QVector<int> adjacentFaces; // List of adjacent face IDs
    QColor baseColor;
    QString name;
};

class Game2Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Game2Widget(DifficultyWidget::Difficulty difficulty, QWidget *parent = nullptr);
    ~Game2Widget();

signals:
    void backToMenu();

private slots:
    void newGame();
    void onLeftClick(int r, int c);
    void onRightClick(int r, int c);
    void onTimerTick();
    void switchToFace(int faceId);
    void giveHint();

private:
    void setupUI();
    void initializePolyhedron();
    void initializeTetrahedron();
    void initializeHexahedron();
    void initializeDodecahedron();
    
    void buildFaceButtons();
    void clearFaceButtons();
    void buildThumbnails();
    
    void resetModel();
    void placeMinesSafely(int safeFace, int safeR, int safeC);
    void calculateAdjacents();
    void revealCell(int faceId, int r, int c);
    void revealNeighborsIfEmpty(int faceId, int r, int c);
    void updateButtonVisual(int faceId, int r, int c);
    void updateMineLabel();
    void checkWinCondition();
    void showExplosionEffect(int r, int c);
    
    bool inBounds(int faceId, int r, int c) const;
    QVector<QPair<int, QPair<int,int>>> getEdgeAdjacentCells(int faceId, int r, int c);

    // UI components
    QGridLayout *boardLayout;
    QVBoxLayout *thumbnailLayout;
    QVector<QVector<FaceCellButton*>> buttonGrid;
    QLabel *mineLabel;
    QLabel *timeLabel;
    QLabel *faceLabel;
    QPushButton *newGameBtn;
    QPushButton *hintButton;
    QPushButton *backButton;
    QScrollArea *thumbnailScrollArea;
    QWidget *thumbnailWidget;
    QVector<QPushButton*> thumbnailButtons;

    QTimer *timer;
    int elapsedSeconds = 0;

    // Sound effects
    QSoundEffect *explosionSound;
    QSoundEffect *winSound;
    QSoundEffect *startSound;

    // Game model
    QVector<Face> faces;
    int currentFaceId = 0;
    int mineCount = 10;
    bool firstClick = true;
    bool gameOver = false;
    int revealedCells = 0;
    int totalCells = 0;
    
    DifficultyWidget::Difficulty difficulty;
};

#endif // GAME2WIDGET_H
