#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D Minesweeper Collection");
    resize(1000, 700);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create menu widget
    menuWidget = new MenuWidget(this);
    stackedWidget->addWidget(menuWidget);

    // Connect menu signals
    connect(menuWidget, &MenuWidget::game1Selected, this, &MainWindow::showGame1);
    connect(menuWidget, &MenuWidget::game2Selected, this, &MainWindow::showDifficultySelection);

    // Create difficulty widget
    difficultyWidget = new DifficultyWidget(this);
    stackedWidget->addWidget(difficultyWidget);

    // Connect difficulty signals
    connect(difficultyWidget, &DifficultyWidget::difficultySelected, this, &MainWindow::startGame2);
    connect(difficultyWidget, &DifficultyWidget::backToMenu, this, &MainWindow::showMenu);

    // Game widgets will be created on demand
    game1Widget = nullptr;
    game2Widget = nullptr;

    // Show menu
    stackedWidget->setCurrentWidget(menuWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showMenu()
{
    stackedWidget->setCurrentWidget(menuWidget);
}

void MainWindow::showGame1()
{
    // Create or reuse Game1Widget
    if (!game1Widget) {
        game1Widget = new Game1Widget(this);
        stackedWidget->addWidget(game1Widget);
        connect(game1Widget, &Game1Widget::backToMenu, this, &MainWindow::showMenu);
    }
    
    stackedWidget->setCurrentWidget(game1Widget);
}

void MainWindow::showDifficultySelection()
{
    stackedWidget->setCurrentWidget(difficultyWidget);
}

void MainWindow::startGame2(DifficultyWidget::Difficulty difficulty)
{
    // Clean up old game2 widget if exists
    if (game2Widget) {
        stackedWidget->removeWidget(game2Widget);
        delete game2Widget;
        game2Widget = nullptr;
    }

    // Create new Game2Widget with selected difficulty
    game2Widget = new Game2Widget(difficulty, this);
    stackedWidget->addWidget(game2Widget);
    connect(game2Widget, &Game2Widget::backToMenu, this, &MainWindow::showMenu);

    stackedWidget->setCurrentWidget(game2Widget);
}
