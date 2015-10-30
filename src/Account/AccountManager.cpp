#include "include/Account.hpp"

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
}

QStringList AccountManager::getAccountList()
{
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
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
