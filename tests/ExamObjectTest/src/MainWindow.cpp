#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent, QString accountName) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    accountManager(AccountManager(database))
{
    ui->setupUi(this);
    ui->summaryReport->setText(accountManager.getSummaryReport(accountName));
    ui->summaryHistory->setText(accountManager.getSummaryHistory(accountName));
}

MainWindow::~MainWindow()
{
    delete ui;
}
