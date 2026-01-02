#ifndef POLYHEDRAWIDGET_H
#define POLYHEDRAWIDGET_H

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
#include <QScrollArea>
#include <QFrame>

// Cell structure for polyhedra faces
struct FaceCell {
    bool isMine = false;
    bool revealed = false;
    bool flagged = false;
    int adj = 0; // Count of adjacent mines (using edge-adjacency only, not vertex-adjacency)
};

// Face button for grid cells
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

// Face thumbnail button
class FaceThumbnail : public QPushButton {
    Q_OBJECT
public:
    FaceThumbnail(int faceId, const QString &label, const QColor &color, QWidget *parent = nullptr)
        : QPushButton(label, parent), faceIndex(faceId) {
        setFixedSize(80, 80);
        setStyleSheet(QString(
            "QPushButton { "
            "  background-color: %1; "
            "  color: white; "
            "  border: 2px solid #555; "
            "  border-radius: 5px; "
            "  font-weight: bold; "
            "  font-size: 14px; "
            "} "
            "QPushButton:hover { "
            "  border: 3px solid #fff; "
            "}"
        ).arg(color.name()));
    }

    int faceIndex;
};

class PolyhedraWidget : public QWidget
{
    Q_OBJECT

public:
    enum PolyhedraType {
        TETRAHEDRON,   // 4 triangular faces
        CUBE,          // 6 square faces
        DODECAHEDRON   // 12 pentagonal faces
    };

    explicit PolyhedraWidget(PolyhedraType type, QWidget *parent = nullptr);
    ~PolyhedraWidget();

signals:
    void backToMenu();

private slots:
    void newGame();
    void onLeftClick(int r, int c);
    void onRightClick(int r, int c);
    void switchToFace(int faceIdx);
    void onTimerTick();
    void showHint();

private:
    // UI
    QGridLayout *mainFaceLayout;
    QVBoxLayout *thumbnailLayout;
    QVector<QVector<FaceCellButton*>> buttonGrid;
    QVector<FaceThumbnail*> thumbnails;
    QLabel *mineLabel;
    QLabel *timeLabel;
    QLabel *faceLabel;
    QPushButton *newGameBtn;
    QPushButton *hintBtn;
    QPushButton *backBtn;

    QTimer *timer;
    int elapsedSeconds = 0;

    // Sound effects
    QSoundEffect *explosionSound;
    QSoundEffect *winSound;
    QSoundEffect *startSound;

    // Game model
    PolyhedraType polyType;
    int faceCount;
    int rowsPerFace;
    int colsPerFace;
    int mineCount;
    int currentFace = 0;
    bool firstClick = true;
    bool gameOver = false;
    int revealedCells = 0;

    // Face data: [faceIndex][row][col]
    QVector<QVector<QVector<FaceCell>>> faces;
    
    // Adjacency map: faceIndex -> list of (adjacent face, edge info)
    QVector<QVector<int>> faceAdjacency;
    
    // Face colors
    QVector<QColor> faceColors;

    // Helpers
    void buildUI();
    void setupPolyhedraStructure();
    void buildFaceButtons();
    void clearFaceButtons();
    void resetModel();
    void placeMinesSafely(int safeFace, int safeR, int safeC);
    void calculateAdjacents();
    void revealCell(int face, int r, int c);
    void revealNeighborsIfEmpty(int face, int r, int c);
    bool inBounds(int face, int r, int c) const;
    void updateButtonVisual(int face, int r, int c);
    void updateMineLabel();
    void checkWinCondition();
    QColor getFaceColor(int face) const;
    void showExplosionEffect(int r, int c);
    
    // Edge-adjacency helper
    QVector<QPair<int, int>> getEdgeAdjacentCells(int face, int r, int c) const;
};

#endif // POLYHEDRAWIDGET_H
