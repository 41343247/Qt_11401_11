#include "menuwidget.h"
#include <QFont>

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

MenuWidget::~MenuWidget()
{
}

void MenuWidget::setupUI()
{
    // Set window properties
    setWindowTitle("3D Minesweeper - Main Menu");
    setStyleSheet("QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a1a2e, stop:1 #16213e); }");
    setMinimumSize(800, 600);

    // Title label
    titleLabel = new QLabel("🎮 3D Minesweeper 🎮", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(36);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #f5f5f5; margin: 30px; text-shadow: 2px 2px 4px #000000; }");

    // Game 1 button - Multi-layer
    game1Button = new QPushButton("遊戲一\n(多層踩地雷)", this);
    game1Button->setMinimumSize(280, 200);
    game1Button->setMaximumSize(320, 220);
    QFont buttonFont;
    buttonFont.setPointSize(20);
    buttonFont.setBold(true);
    game1Button->setFont(buttonFont);
    game1Button->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #667eea, stop:1 #4a5fd8);"
        "   color: white;"
        "   border: 3px solid #8b9cff;"
        "   border-radius: 20px;"
        "   padding: 20px;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7b8ffa, stop:1 #5a6fe8);"
        "   border: 3px solid #a0b0ff;"
        "   transform: scale(1.05);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5a6fe8, stop:1 #3a4fc8);"
        "}"
    );

    // Game 2 button - Multi-face
    game2Button = new QPushButton("遊戲二\n(多面踩地雷)", this);
    game2Button->setMinimumSize(280, 200);
    game2Button->setMaximumSize(320, 220);
    game2Button->setFont(buttonFont);
    game2Button->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f093fb, stop:1 #f5576c);"
        "   color: white;"
        "   border: 3px solid #ff8ba8;"
        "   border-radius: 20px;"
        "   padding: 20px;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffa3ff, stop:1 #ff677c);"
        "   border: 3px solid #ffacb8;"
        "   transform: scale(1.05);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e083eb, stop:1 #e5475c);"
        "}"
    );

    // Connect signals
    connect(game1Button, &QPushButton::clicked, this, &MenuWidget::game1Selected);
    connect(game2Button, &QPushButton::clicked, this, &MenuWidget::game2Selected);

    // Layout - buttons side by side
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(game1Button);
    buttonLayout->addSpacing(60);
    buttonLayout->addWidget(game2Button);
    buttonLayout->addStretch();

    // Subtitle
    QLabel *subtitleLabel = new QLabel("選擇你的遊戲模式", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont;
    subtitleFont.setPointSize(16);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("QLabel { color: #b0b0b0; margin: 10px; }");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(40, 40, 40, 40);

    setLayout(mainLayout);
}
