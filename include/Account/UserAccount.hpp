#ifndef USERACCOUNT_HPP
#define USERACCOUNT_HPP

#include <QString>
#include <QtSql>

class UserAccount
{
public:
    UserAccount() {}
    UserAccount(QSqlDatabase db);
    UserAccount(QSqlDatabase db, QString accountName);
private:
    QString accountName;
};

#endif // USERACCOUNT_HPP
