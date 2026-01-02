#include "difficultywidget.h"
#include <QFont>

DifficultyWidget::DifficultyWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

DifficultyWidget::~DifficultyWidget()
{
}

void DifficultyWidget::setupUI()
{
    // Set window properties
    setWindowTitle("選擇難度 - 遊戲二");
    setStyleSheet("QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2c1a3e, stop:1 #3e1628); }");
    setMinimumSize(700, 550);

    // Title label
    titleLabel = new QLabel("選擇難度", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color: #f5f5f5; margin: 20px; text-shadow: 2px 2px 4px #000000; }");

    QFont buttonFont;
    buttonFont.setPointSize(18);
    buttonFont.setBold(true);

    // Easy button - Tetrahedron
    easyButton = new QPushButton("簡單\n正四面體 (4面)", this);
    easyButton->setMinimumSize(200, 120);
    easyButton->setFont(buttonFont);
    easyButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(144, 238, 144, 0.3);"
        "   color: #90EE90;"
        "   border: 2px solid #90EE90;"
        "   border-radius: 15px;"
        "   padding: 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(144, 238, 144, 0.5);"
        "   border: 3px solid #90EE90;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(144, 238, 144, 0.7);"
        "}"
    );

    // Medium button - Hexahedron
    mediumButton = new QPushButton("中等\n正六面體 (6面)", this);
    mediumButton->setMinimumSize(200, 120);
    mediumButton->setFont(buttonFont);
    mediumButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 215, 0, 0.3);"
        "   color: #FFD700;"
        "   border: 2px solid #FFD700;"
        "   border-radius: 15px;"
        "   padding: 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 215, 0, 0.5);"
        "   border: 3px solid #FFD700;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255, 215, 0, 0.7);"
        "}"
    );

    // Hard button - Dodecahedron
    hardButton = new QPushButton("困難\n正十二面體 (12面)", this);
    hardButton->setMinimumSize(200, 120);
    hardButton->setFont(buttonFont);
    hardButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 99, 71, 0.3);"
        "   color: #FF6347;"
        "   border: 2px solid #FF6347;"
        "   border-radius: 15px;"
        "   padding: 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 99, 71, 0.5);"
        "   border: 3px solid #FF6347;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255, 99, 71, 0.7);"
        "}"
    );

    // Back button
    backButton = new QPushButton("返回主選單", this);
    backButton->setMinimumSize(150, 40);
    QFont backFont;
    backFont.setPointSize(12);
    backButton->setFont(backFont);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(128, 128, 128, 0.3);"
        "   color: #cccccc;"
        "   border: 1px solid #888888;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(128, 128, 128, 0.5);"
        "}"
    );

    // Connect signals
    connect(easyButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(Easy); });
    connect(mediumButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(Medium); });
    connect(hardButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(Hard); });
    connect(backButton, &QPushButton::clicked, this, &DifficultyWidget::backToMenu);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(easyButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(mediumButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(hardButton);
    buttonLayout->addStretch();

    // Subtitle
    QLabel *subtitleLabel = new QLabel("選擇立體形狀的複雜度", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont;
    subtitleFont.setPointSize(14);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("QLabel { color: #b0b0b0; margin: 10px; }");

    // Back button layout
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addWidget(backButton);
    backLayout->addStretch();

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(backLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(30, 30, 30, 30);

    setLayout(mainLayout);
}
