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

    void createExam();
    void addByArea(int amount, QStringList areaNames);
    void addBySubject(int amount, QStringList subjectNames);
    void addByTopic(int amount, QStringList topicNames);
    void addAny(int amount);

    QList<Question> getQuestionList();
    QList<Question> getQuestionList(int lo, int hi);
    Question getQuestionAt(int index);

    int getAmountQuestions();

    QStringList getDBAreas();
    QStringList getDBSubjects(QString area);
    QStringList getDBTopics(QString subject);
    QStringList getDBTopicsPerArea(QString area);
    int getDBAmountQuestions(QString column, QStringList columnNames);

    void clearExamInfo();
    void addRegistry(QString registryName, QStringList registtrValue, int amount, QString criteria);
    void removeRegistry(QString registryName);
    void editRegistry(QString oldName, QString newName,
                      QStringList newValues, int newAmount);

    QStringList getRegistryNames();
    QStringList getRegistryValues(QString registryName);
    int getRegistryAmount(QString registryName);

    QStringList getSummary();

    void setTime(int time);
    int getTime();
private:
    void addQuestions(QString column, int amount, QStringList columnNames);
    Question getQuestion(int questionID, QString questionDescription);

    QSqlDatabase db;

    QuizManExam exam;

    int time = 0;
    int count = 0;
    QHash<int, QString> registryCount;
    QHash<int, QStringList> registers;
    QHash<int, int> infoAmount;
    QHash<int, QString> registryCriteria;
};

#endif // EXAMMANAGER_HPP
