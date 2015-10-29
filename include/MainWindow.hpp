#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "Account.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, UserAccount account = UserAccount());
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UserAccount account;
};

#endif // MAINWINDOW_HPP
