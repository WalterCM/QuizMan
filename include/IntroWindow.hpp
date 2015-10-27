#ifndef INTROWINDOW_HPP
#define INTROWINDOW_HPP

#include <QMainWindow>
#include <QStringListModel>
#include "MainWindow.hpp"
#include "Account.hpp"

namespace Ui {
class IntroWindow;
}

class IntroWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IntroWindow(QWidget *parent = 0);
    ~IntroWindow();

private slots:
    void on_useAccountButton_clicked();
    void on_createAccountButton_clicked();
    void on_accountSelectionBack_clicked();
    void on_accountCreationBack_clicked();

    void on_noAccountButton_clicked();

private:
    Ui::IntroWindow *ui;
    QStringListModel *listModel;
    MainWindow *mainWindow;

    QSqlDatabase dataBase;
    AccountManager accountManager;
};

#endif // INTROWINDOW_HPP
