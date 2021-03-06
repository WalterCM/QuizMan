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
    void editDBQuestion(int questionID, QString areaName, QString subjectName,
                        QString questionDescription,
                        QString questionImageLocation,
                        QStringList topicNames, QStringList options,
                        QList<int> correctOptions);

    int getDBQuestionID(QString questionDescription);

    QStringList getDBAreas();
    QString getDBAreaByQuestion(int questionID);
    QStringList getDBSubjects(QString area);
    QString getDBSubjectByQuestion(int questionID);
    QStringList getDBTopics(QString subject);
    QStringList getDBTopicsByArea(QString area);
    QStringList getDBTopicsByQuestionID(int questionID);
    QString getDBQuestionDescription(int questionID);

    QStringList getDBQuestionListByTopic(QString topic);
    QHash<int, QString> getDBQuestionHashByTopic(QString topic);
    QStringList getDBDetailsOfQuestion(int questionID);
    QStringList getDBOptions(int questionID);
    QString getDBOptionCorrect(int questionID);

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

    bool safeOpen();
    void safeClose(bool openedBefore);

    QSqlDatabase db;
};

#endif // QUESTIONDBMANAGER_H
