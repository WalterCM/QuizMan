#ifndef QUESTIONDBMANAGER_H
#define QUESTIONDBMANAGER_H

#include <QStringList>
#include <QHash>
#include <QtSql>

class QuestionDBManager
{
public:
    QuestionDBManager();
    QuestionDBManager(QSqlDatabase db);

    void insertDBQuestion(QString areaName, QString subjectName,
                          QString questionDescription,
                          QString questionImageLocation,
                          QStringList topicNames, QStringList options,
                          QList<int> correctOptions);
    void deleteDBQuestion(int questionID);
    void editDBQuestion(QString areaName, QString subjectName,
                        QString questionDescription,
                        QString questionImageLocation,
                        QStringList topicNames, QStringList options,
                        QList<int> correctOptions);

    QStringList getDBAreas();
    QStringList getDBSubjects(QString area);
    QStringList getDBTopics(QString subject);
    QStringList getDBTopicsByArea(QString area);
    QStringList getDBQuestionListByTopic(QString topic);
    QHash<int, QString> getDBQuestionHashByTopic(QString topic);
    QStringList getDBDetailsOfQuestion(int questionID);
    QStringList getDBOptions(int questionID);

    int getDBAmountQuestions(QString column, QStringList columnNames);
    int getDBLastID();
private:
    int insertDBArea(QString areaName);
    int insertDBSubject(QString subjectName, int areaID);
    int insertDBQuestionQuery(QString questionDescription,
                              QString questionImageLocation);
    void insertDBTopics(QStringList topics, int subjectID, int questionID);
    void insertDBOptions(QStringList options, int questionID, QList<int> correctOptions);

    void deleteDBArea(int areaID);
    void deleteDBSubject(int subjectID);
    void deleteDBTopics(int questionID);
    void deleteDBOptions(int questionID);

    int editQuestionquery(QString questionDescription, questionImageLocation);
    QSqlDatabase db;


};

#endif // QUESTIONDBMANAGER_H
