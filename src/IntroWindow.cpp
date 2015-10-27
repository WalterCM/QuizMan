#include <QtSql>
#include "../include/IntroWindow.hpp"
#include "ui_IntroWindow.h"
#include "../include/ProgramConstants.hpp"

IntroWindow::IntroWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::IntroWindow),
        accountManager(AccountManager(dataBase))
{
    // Configura la interfaz de usuario
    ui->setupUi(this);

    // Previene los cambios de tamano de pantalla
    this->setFixedSize(this->size());

    // Agrega una base de datos SQLite a nuestro objeto QSqlDatabase
    dataBase = QSqlDatabase::addDatabase("QSQLITE");

    // Obtiene la direccion absoluta del programa
    // Y le agrega la direccion relativa de la base de datos
    // Y configura el camino a la base de datos en el objeto
    QString path = QDir::currentPath();
    path.append(DATABASE_RELATIVE_PATH);
    dataBase.setDatabaseName(path);

    // Configura la base de datos en el administrador de cuentas
    accountManager = AccountManager(dataBase);

    // Instancia un nuevo modelo de lista
    listModel = new QStringListModel(this);
    mainWindow = new MainWindow(this);
}

IntroWindow::~IntroWindow()
{
    delete ui;
    delete listModel;
    delete mainWindow;
}

void IntroWindow::on_useAccountButton_clicked()
{
    // Cambia el widget mostrado a la pagina de seleccion de cuentas
    ui->stackedWidget->setCurrentWidget(ui->accountSelectionPage);

    // Guarda la lista de cuentas de la base de datos
    listModel->setStringList(accountManager.getAccountList());

    // Configura el modelo en la lista de la interfaz de usuario
    // Evita modificaciones de la lista y su respectivo modelo
    ui->usersListUI->setModel(listModel);
    ui->usersListUI->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void IntroWindow::on_createAccountButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->accountCreationPage);
}

void IntroWindow::on_accountSelectionBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->accountModeSelectionPage);
}

void IntroWindow::on_accountCreationBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->accountModeSelectionPage);
}

void IntroWindow::on_noAccountButton_clicked()
{
    this->hide();
    mainWindow->showMaximized();
}
