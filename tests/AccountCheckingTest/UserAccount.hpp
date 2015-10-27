#ifndef USERACCOUNT_HPP
#define USERACCOUNT_HPP

#include <QString>
#include <QtSql>

class UserAccount
{
public:
    UserAccount(QSqlDatabase db);
    bool exist(QString accountName);
private:
    QString accountName;
    QSqlDatabase db;
};

#endif // USERACCOUNT_HPP
