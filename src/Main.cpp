#include "../include/IntroWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IntroWindow w;
    w.show();

    return a.exec();
}
