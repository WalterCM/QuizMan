#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent, UserAccount account) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    account(account)
{
    ui->setupUi(this);
    ui->summaryReport->setText(account.getSummaryReport());
    ui->summaryHistory->setText(account.getSummaryHistory());
}

MainWindow::~MainWindow()
{
    delete ui;
}
