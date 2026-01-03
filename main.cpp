#include "widget.h"
#include "game2.h"

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 創建選擇對話框
    QDialog selector;
    selector.setWindowTitle("選擇遊戲模式");
    selector.setStyleSheet("QDialog { background-color: #2b2b2b; } QLabel { color: #ffffff; font-size: 16px; font-weight: bold; } QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; font-weight: bold; border-radius: 5px; font-size: 14px; } QPushButton:hover { background-color: #45a049; }");
    
    QVBoxLayout *layout = new QVBoxLayout(&selector);
    
    QLabel *titleLabel = new QLabel("請選擇遊戲模式:", &selector);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    QPushButton *game1Btn = new QPushButton("遊戲模式 1: 多層掃雷", &selector);
    QPushButton *game2Btn = new QPushButton("遊戲模式 2: 空心六面體掃雷", &selector);
    
    layout->addWidget(game1Btn);
    layout->addWidget(game2Btn);
    
    QWidget *selectedGame = nullptr;
    
    QObject::connect(game1Btn, &QPushButton::clicked, [&]() {
        selector.close();
        selectedGame = new Widget();
        selectedGame->show();
    });
    
    QObject::connect(game2Btn, &QPushButton::clicked, [&]() {
        selector.close();
        selectedGame = new Game2();
        selectedGame->show();
    });
    
    selector.exec();
    
    return a.exec();
}
