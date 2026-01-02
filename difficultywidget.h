#ifndef DIFFICULTYWIDGET_H
#define DIFFICULTYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class DifficultyWidget : public QWidget
{
    Q_OBJECT

public:
    enum Difficulty {
        Easy = 0,      // Tetrahedron (4 faces)
        Medium = 1,    // Hexahedron (6 faces)
        Hard = 2       // Dodecahedron (12 faces)
    };

    explicit DifficultyWidget(QWidget *parent = nullptr);
    ~DifficultyWidget();

signals:
    void difficultySelected(Difficulty difficulty);
    void backToMenu();

private:
    void setupUI();
    QPushButton *easyButton;
    QPushButton *mediumButton;
    QPushButton *hardButton;
    QPushButton *backButton;
    QLabel *titleLabel;
};

#endif // DIFFICULTYWIDGET_H
