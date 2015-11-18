#include <QApplication>
#include "include/IntroWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IntroWindow w;
    w.show();

    return a.exec();
}
