#ifndef DIFFICULTYWIDGET_H
#define DIFFICULTYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

class DifficultyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DifficultyWidget(QWidget *parent = nullptr);

signals:
    void easySelected();      // Tetrahedron (4 faces)
    void mediumSelected();    // Cube (6 faces)
    void hardSelected();      // Dodecahedron (12 faces)
    void backToMenu();        // Back to main menu

private:
    void setupUI();
    QPushButton *createDifficultyButton(const QString &text, const QString &description, const QString &color);
};

#endif // DIFFICULTYWIDGET_H
