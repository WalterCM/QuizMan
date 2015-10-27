#include "UserAccount.hpp"

#include <QDebug>

UserAccount::UserAccount(QSqlDatabase db)
{
    this->db = db;

}

bool UserAccount::exist(QString accountName)
{
    if (!db.open())
    {
        qDebug() << "Database not found";

    }
    QSqlQuery query;
    query.prepare("SELECT EXISTS(SELECT 1 FROM Accounts WHERE AccountName =:name LIMIT 1)");
    query.bindValue(":name", QVariant(accountName));
    query.exec();
    query.next();
    bool result = query.value(0).toBool();
    db.close();
    return result;
}
