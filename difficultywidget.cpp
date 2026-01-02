#include "difficultywidget.h"
#include <QApplication>

DifficultyWidget::DifficultyWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void DifficultyWidget::setupUI()
{
    // Set window properties
    setWindowTitle("Game 2 - Select Difficulty");
    setStyleSheet("QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2c003e, stop:1 #16213e); }");
    setMinimumSize(900, 600);

    // Title
    QLabel *titleLabel = new QLabel("🎮 Game 2: Multi-Face Minesweeper", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "  font-size: 40px; "
        "  font-weight: bold; "
        "  color: #fff; "
        "  background: transparent; "
        "  padding: 20px; "
        "}"
    );
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    QLabel *subtitleLabel = new QLabel("Select Your Challenge Level", this);
    subtitleLabel->setStyleSheet(
        "QLabel { "
        "  font-size: 20px; "
        "  color: #ddd; "
        "  background: transparent; "
        "  padding: 10px; "
        "}"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Create difficulty buttons
    QPushButton *easyBtn = createDifficultyButton("🔺 Easy", "Tetrahedron\n(4 Triangular Faces)", "#4CAF50");
    QPushButton *mediumBtn = createDifficultyButton("🔷 Medium", "Cube\n(6 Square Faces)", "#FF9800");
    QPushButton *hardBtn = createDifficultyButton("⬟ Hard", "Dodecahedron\n(12 Pentagonal Faces)", "#F44336");

    // Connect buttons
    connect(easyBtn, &QPushButton::clicked, this, &DifficultyWidget::easySelected);
    connect(mediumBtn, &QPushButton::clicked, this, &DifficultyWidget::mediumSelected);
    connect(hardBtn, &QPushButton::clicked, this, &DifficultyWidget::hardSelected);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(easyBtn);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(mediumBtn);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(hardBtn);
    buttonLayout->addStretch();

    // Back button
    QPushButton *backBtn = new QPushButton("← Back to Menu", this);
    backBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #555; "
        "  color: white; "
        "  border: 2px solid #777; "
        "  border-radius: 8px; "
        "  font-size: 16px; "
        "  padding: 10px 20px; "
        "} "
        "QPushButton:hover { "
        "  background-color: #666; "
        "  border: 2px solid #999; "
        "}"
    );
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, &DifficultyWidget::backToMenu);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(backBtn, 0, Qt::AlignLeft);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(30, 30, 30, 30);

    setLayout(mainLayout);
}

QPushButton *DifficultyWidget::createDifficultyButton(const QString &text, const QString &description, const QString &color)
{
    QPushButton *btn = new QPushButton(text + "\n\n" + description, this);
    btn->setMinimumSize(220, 180);
    btn->setMaximumSize(260, 220);
    btn->setCursor(Qt::PointingHandCursor);
    
    QString style = QString(
        "QPushButton { "
        "  background-color: %1; "
        "  color: white; "
        "  border: 3px solid rgba(255, 255, 255, 0.4); "
        "  border-radius: 15px; "
        "  font-size: 18px; "
        "  font-weight: bold; "
        "  padding: 15px; "
        "} "
        "QPushButton:hover { "
        "  background-color: %2; "
        "  border: 3px solid rgba(255, 255, 255, 0.9); "
        "} "
        "QPushButton:pressed { "
        "  background-color: %3; "
        "  padding-top: 20px; "
        "  padding-bottom: 10px; "
        "}"
    ).arg(color)
     .arg(QColor(color).lighter(115).name())
     .arg(QColor(color).darker(115).name());
    
    btn->setStyleSheet(style);
    
    // Add shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 140));
    shadow->setOffset(4, 4);
    btn->setGraphicsEffect(shadow);
    
    return btn;
}
