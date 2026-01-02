#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "menuwidget.h"
#include "difficultywidget.h"
#include "game1widget.h"
#include "game2widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMenu();
    void showGame1();
    void showDifficultySelection();
    void startGame2(DifficultyWidget::Difficulty difficulty);

private:
    QStackedWidget *stackedWidget;
    MenuWidget *menuWidget;
    DifficultyWidget *difficultyWidget;
    Game1Widget *game1Widget;
    Game2Widget *game2Widget;
};

#endif // MAINWINDOW_H
