#include "include/Account/AccountManager.hpp"
#include <iostream>
AccountManager::AccountManager(QSqlDatabase db)
{
    this->db = db;
}


bool AccountManager::exists(QString accountName)
{
    // Intenta abrir la base de datos y se verifica que fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
        qDebug() << "1";
        return 0;
    }

    // Crea una nueva consulta
    // Prepara una consulta que devuelve 1 o 0 si existe lo que se busca
    // Configura el valor de :name como @accountName que se busca
    // Ejecuta la consulta
    // Y obtiene el siguiente y unico valor de la consulta
    QSqlQuery query;
    query.prepare("SELECT EXISTS(SELECT 1 FROM Accounts"
                  "WHERE AccountName = :name LIMIT 1)");
    query.bindValue(":name", QVariant(accountName));
    query.exec();
    query.next();

    // Guarda el resultado de la consulta en un bool
    bool result = query.value(0).toBool();

    // Cierra la base de datos
    db.close();
    return result;
}

void AccountManager::createAccount(QString accountName)
{
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
        qDebug() << "2";
        return;
    }

    // Crea una nueva consulta
    // Prepara una consulta que crea un nuevo registro
    // Configura el valor de :name como @accountName
    // Ejecuta la consulta
    QSqlQuery query;
    query.prepare("INSERT INTO Accounts (AccountName)"
                  "VALUES (:name)");
    query.bindValue(":name", QVariant(accountName));
    query.exec();

    // Cierra la base de datos
    db.close();
}

void AccountManager::deleteAccount(QString accountName)
{
    // TODO: write the code for deleting accounts
}

QStringList AccountManager::getAccountList()
{
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
        qDebug() << "3";
        return QStringList();
    }

    // Crea una nueva consulta
    // Ejecuta una consulta que devuelve todos los nombres de cuenta
    QSqlQuery query;
    query.exec("SELECT AccountName FROM Accounts");

    // Crea una lista de strings
    // Mientras haya mas cuentas, agrega el nombre en la lista
    QStringList list;
    while (query.next()) {
        // No agrega la cuenta de invitado a la lista
        if (query.value(0).toString() == "Guest")
            continue;
        list << query.value(0).toString();
    }

    // Cierra la base de datos y retorna la lista
    db.close();
    return list;
}

QString AccountManager::getAccountName(int index)
{
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
        qDebug() << "4";
        return "";
    }

    // Crea una nueva consulta
    // Ejecuta una consulta que devuelve todos los nombres de cuenta
    QSqlQuery query;
    query.prepare("SELECT AccountName FROM Accounts WHERE AccountID = :ID");
    query.bindValue(":ID", index);
    query.exec();
    query.next();

    QString accountName = query.value(0).toString();

    // Cierra la base de datos y retorna la lista
    db.close();
    return accountName;
}

QString AccountManager::getHistory(QString accountName)
{
    return QString("No hay historial");
}

QString AccountManager::getReport(QString accountName)
{
    return QString("No hay reporte");
}

QString AccountManager::getSummaryHistory(QString accountName)
{
    return QString("No hay historial");
}

QString AccountManager::getSummaryReport(QString accountName)
{
    return QString("Hola, " + accountName);
}
