#include "../../include/Account/UserAccount.hpp"

UserAccount::UserAccount(QSqlDatabase db) {
    this->accountName = "Invitado";
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
    }

    // Crea una nueva consulta
    // Ejecuta una consulta que devuelve todos los nombres de cuenta
    QSqlQuery query;
    query.exec("SELECT * FROM Accounts"
               "WHERE AccountName = 'Guest'");

    while (query.next()) {
        // TODO: Llenar los datos del perfil
    }

    // Cierra la base de datos y retorna la lista
    db.close();
}

UserAccount::UserAccount(QSqlDatabase db, QString accountName)
{
    this->accountName = accountName;
    // Verifica que la base de datos fue encontrada
    if (!db.open())
    {
        qDebug() << "Database not found";
    }

    // Crea una nueva consulta
    // Ejecuta una consulta que devuelve todos los nombres de cuenta
    QSqlQuery query;
    query.prepare("SELECT * FROM Accounts"
               "WHERE AccountName = :name");
    query.bindValue(":name", QVariant(accountName));
    query.exec();

    while (query.next()) {
        // TODO: Llenar los datos del perfil
    }

    // Cierra la base de datos y retorna la lista
    db.close();
}
