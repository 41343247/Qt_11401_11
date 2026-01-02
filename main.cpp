#include "gamecontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameController gc;
    gc.show();
    return a.exec();
}
