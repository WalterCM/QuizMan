#ifndef ACCOUNTMANAGER_HPP
#define ACCOUNTMANAGER_HPP

#include <QString>
#include <QtSql>

class AccountManager
{
public:
    AccountManager(QSqlDatabase db);
    bool exists(QString accountName);
    void createAccount(QString accountName);
    void deleteAccount(QString accountName);
    QStringList getAccountList();
private:
    QSqlDatabase db;
};

#endif // ACCOUNTMANAGER_HPP
