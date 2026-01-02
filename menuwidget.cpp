#include "menuwidget.h"
#include <QApplication>

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void MenuWidget::setupUI()
{
    // Set window properties
    setWindowTitle("Minesweeper - Game Selection");
    setStyleSheet("QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a1a2e, stop:1 #16213e); }");
    setMinimumSize(800, 600);

    // Title label
    QLabel *titleLabel = new QLabel("💣 Minesweeper Collection 💣", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "  font-size: 48px; "
        "  font-weight: bold; "
        "  color: #eee; "
        "  background: transparent; "
        "  padding: 20px; "
        "}"
    );
    titleLabel->setAlignment(Qt::AlignCenter);

    // Add shadow effect to title
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(15);
    shadowEffect->setColor(QColor(0, 0, 0, 180));
    shadowEffect->setOffset(3, 3);
    titleLabel->setGraphicsEffect(shadowEffect);

    // Subtitle
    QLabel *subtitleLabel = new QLabel("Choose Your Adventure", this);
    subtitleLabel->setStyleSheet(
        "QLabel { "
        "  font-size: 24px; "
        "  color: #bbb; "
        "  background: transparent; "
        "  padding: 10px; "
        "}"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Create game buttons
    QPushButton *game1Btn = createStyledButton("🎲 Game 1\nMulti-Layer", "#4A90E2");
    QPushButton *game2Btn = createStyledButton("🎮 Game 2\nMulti-Face", "#E24A90");

    // Connect buttons
    connect(game1Btn, &QPushButton::clicked, this, &MenuWidget::game1Selected);
    connect(game2Btn, &QPushButton::clicked, this, &MenuWidget::game2Selected);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(game1Btn);
    buttonLayout->addSpacing(50);
    buttonLayout->addWidget(game2Btn);
    buttonLayout->addStretch();

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(50);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(50, 50, 50, 50);

    setLayout(mainLayout);
}

QPushButton *MenuWidget::createStyledButton(const QString &text, const QString &color)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setMinimumSize(250, 200);
    btn->setMaximumSize(300, 250);
    btn->setCursor(Qt::PointingHandCursor);
    
    QString style = QString(
        "QPushButton { "
        "  background-color: %1; "
        "  color: white; "
        "  border: 3px solid rgba(255, 255, 255, 0.3); "
        "  border-radius: 20px; "
        "  font-size: 28px; "
        "  font-weight: bold; "
        "  padding: 20px; "
        "} "
        "QPushButton:hover { "
        "  background-color: %2; "
        "  border: 3px solid rgba(255, 255, 255, 0.8); "
        "  transform: scale(1.05); "
        "} "
        "QPushButton:pressed { "
        "  background-color: %3; "
        "  padding-top: 25px; "
        "  padding-bottom: 15px; "
        "}"
    ).arg(color)
     .arg(QColor(color).lighter(120).name())
     .arg(QColor(color).darker(120).name());
    
    btn->setStyleSheet(style);
    
    // Add shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 160));
    shadow->setOffset(5, 5);
    btn->setGraphicsEffect(shadow);
    
    return btn;
}
