#include "include/Account.hpp"

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

QString UserAccount::getAccountName()
{
    return accountName;
}

QString UserAccount::getHistory()
{
    return QString("No history");
}

QString UserAccount::getReport()
{
    return QString("No report");
}

QString UserAccount::getSummaryHistory()
{
    QString text = "No se han tomado examenes aun";
    return text;
}

QString UserAccount::getSummaryReport()
{
    QString text = "Hola, " + accountName;
    return text;
}
