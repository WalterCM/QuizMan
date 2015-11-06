#include <QCoreApplication>
#include <QDebug>
#include <QtSql>
#include <QDir>
#include "include/Exam.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QDir::currentPath();
    path.append("test.db");
    db.setDatabaseName(path);
    int t = 10;
    while (t--) {
        qDebug() << 10 - t;
        ExamManager manager(db);

        manager.addAny(4);

        foreach (Question q, manager.getQuestionList()) {
            qDebug() << q.getDescription();
        }
    }

    return a.exec();
}

