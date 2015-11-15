#include <QApplication>
#include "include/IntroWindow.hpp"
#include "quizmanexamtest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test1();
    IntroWindow w;
    w.show();

    return a.exec();
}
