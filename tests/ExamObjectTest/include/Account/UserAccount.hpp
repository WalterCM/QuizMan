/*#ifndef USERACCOUNT_HPP
#define USERACCOUNT_HPP

#include <QtSql>
#include <QString>

class UserAccount
{
public:
    UserAccount();
    UserAccount(QString accountName);
    QString getAccountName();
private:
    QString accountName;
    QSqlDatabase db;
};

#endif // USERACCOUNT_HPP*/
