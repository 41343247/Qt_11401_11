#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QWidget>
#include <QStackedWidget>
#include "menuwidget.h"
#include "difficultywidget.h"
#include "widget.h"
#include "polyhedrawidget.h"

class GameController : public QWidget
{
    Q_OBJECT

public:
    explicit GameController(QWidget *parent = nullptr);

private slots:
    void showMenu();
    void showGame1();
    void showGame2Difficulty();
    void showGame2Easy();
    void showGame2Medium();
    void showGame2Hard();

private:
    QStackedWidget *stackedWidget;
    MenuWidget *menuWidget;
    DifficultyWidget *difficultyWidget;
    Widget *game1Widget;
    PolyhedraWidget *game2EasyWidget;
    PolyhedraWidget *game2MediumWidget;
    PolyhedraWidget *game2HardWidget;
};

#endif // GAMECONTROLLER_H
