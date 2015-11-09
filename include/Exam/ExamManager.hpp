#ifndef EXAMMANAGER_HPP
#define EXAMMANAGER_HPP

#include <QtSql>
#include <QString>
#include "QuizManExam.hpp"

class ExamManager
{
public:
    ExamManager();
    ExamManager(QSqlDatabase db);

    void addByArea(int amount, QStringList areaNames);
    void addBySubject(int amount, QStringList subjectNames);
    void addByTopic(int amount, QStringList topicNames);
    void addAny(int amount);

    QList<Question> getQuestionList();

    QStringList getDBAreas();
    QStringList getDBSubjects(QString area);
    QStringList getDBTopics(QString subject);
    QStringList getDBTopicsPerArea(QString area);

    void clearExamInfo();
    void addRegister(QString registerName, QStringList registerValue, int amount);
    void removeRegister(QString registerName);
    QStringList getRegisterNames();
    QStringList getRegisterValue(QString registerName);
    int getRegisterAmount(QString registerName);

    void setTime(int time);
    int getTime();
private:
    void addQuestions(QString column, int amount, QStringList columnNames);
    Question getQuestion(int questionID, QString questionDescription);

    QSqlDatabase db;

    QuizManExam exam;

    int time;
    int count = 0;
    QHash<int, QString> registryCount;
    QHash<QString, QStringList> registers;
    QHash<QString, int> infoAmount;
};

#endif // EXAMMANAGER_HPP
