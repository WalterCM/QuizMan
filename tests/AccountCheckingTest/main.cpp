#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QtSql>
#include "UserAccount.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string accountName;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../../resources/database/quizman.db");
    UserAccount ac(db);
    while (std::cin >> accountName) {
        QString qAccountName;
        qAccountName = QString::fromStdString(accountName);

        if (ac.exist(qAccountName)) {
            qDebug() << "El usuario existe";
        } else {
            qDebug() << "El usuario no existe";
        }
    }
    return a.exec();
}

