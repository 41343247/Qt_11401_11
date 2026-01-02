#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void game1Selected();  // Multi-layer minesweeper
    void game2Selected();  // Multi-face minesweeper

private:
    void setupUI();
    QPushButton *createStyledButton(const QString &text, const QString &color);
};

#endif // MENUWIDGET_H
