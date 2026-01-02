#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

signals:
    void game1Selected();
    void game2Selected();

private:
    void setupUI();
    QPushButton *game1Button;
    QPushButton *game2Button;
    QLabel *titleLabel;
};

#endif // MENUWIDGET_H
