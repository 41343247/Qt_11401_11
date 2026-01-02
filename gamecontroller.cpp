#include "gamecontroller.h"
#include <QVBoxLayout>

GameController::GameController(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Minesweeper Collection");
    resize(1000, 700);

    stackedWidget = new QStackedWidget(this);

    // Create menu
    menuWidget = new MenuWidget(this);
    connect(menuWidget, &MenuWidget::game1Selected, this, &GameController::showGame1);
    connect(menuWidget, &MenuWidget::game2Selected, this, &GameController::showGame2Difficulty);
    stackedWidget->addWidget(menuWidget);

    // Create difficulty selection
    difficultyWidget = new DifficultyWidget(this);
    connect(difficultyWidget, &DifficultyWidget::easySelected, this, &GameController::showGame2Easy);
    connect(difficultyWidget, &DifficultyWidget::mediumSelected, this, &GameController::showGame2Medium);
    connect(difficultyWidget, &DifficultyWidget::hardSelected, this, &GameController::showGame2Hard);
    connect(difficultyWidget, &DifficultyWidget::backToMenu, this, &GameController::showMenu);
    stackedWidget->addWidget(difficultyWidget);

    // Create Game 1 (multi-layer)
    game1Widget = new Widget(this);
    connect(game1Widget, &Widget::backToMenu, this, &GameController::showMenu);
    stackedWidget->addWidget(game1Widget);

    // Game 2 widgets will be created on demand
    game2EasyWidget = nullptr;
    game2MediumWidget = nullptr;
    game2HardWidget = nullptr;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    // Start with menu
    stackedWidget->setCurrentWidget(menuWidget);
}

void GameController::showMenu()
{
    stackedWidget->setCurrentWidget(menuWidget);
}

void GameController::showGame1()
{
    stackedWidget->setCurrentWidget(game1Widget);
}

void GameController::showGame2Difficulty()
{
    stackedWidget->setCurrentWidget(difficultyWidget);
}

void GameController::showGame2Easy()
{
    if (!game2EasyWidget) {
        game2EasyWidget = new PolyhedraWidget(PolyhedraWidget::TETRAHEDRON, this);
        connect(game2EasyWidget, &PolyhedraWidget::backToMenu, this, &GameController::showGame2Difficulty);
        stackedWidget->addWidget(game2EasyWidget);
    }
    stackedWidget->setCurrentWidget(game2EasyWidget);
}

void GameController::showGame2Medium()
{
    if (!game2MediumWidget) {
        game2MediumWidget = new PolyhedraWidget(PolyhedraWidget::CUBE, this);
        connect(game2MediumWidget, &PolyhedraWidget::backToMenu, this, &GameController::showGame2Difficulty);
        stackedWidget->addWidget(game2MediumWidget);
    }
    stackedWidget->setCurrentWidget(game2MediumWidget);
}

void GameController::showGame2Hard()
{
    if (!game2HardWidget) {
        game2HardWidget = new PolyhedraWidget(PolyhedraWidget::DODECAHEDRON, this);
        connect(game2HardWidget, &PolyhedraWidget::backToMenu, this, &GameController::showGame2Difficulty);
        stackedWidget->addWidget(game2HardWidget);
    }
    stackedWidget->setCurrentWidget(game2HardWidget);
}
