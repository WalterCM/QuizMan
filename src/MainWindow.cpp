#include "ui_MainWindow.h"
#include "../include/MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent, UserAccount account) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    account(account)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
