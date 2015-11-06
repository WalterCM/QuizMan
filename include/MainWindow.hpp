#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtSql>
#include "Account.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString accountName = "Guest");
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;\
};

#endif // MAINWINDOW_HPP
