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
    QString getAccountName();
    QString getHistory();
    QString getReport();
    QString getSummaryHistory();
    QString getSummaryReport();
private:
    QString accountName;
};

#endif // USERACCOUNT_HPP
