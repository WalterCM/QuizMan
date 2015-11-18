#ifndef EXAMMANAGER_HPP
#define EXAMMANAGER_HPP

#include <QtSql>
#include <QString>
#include <QMap>
#include "QuizManExam.hpp"

class ExamManager
{
public:
    ExamManager();
    ExamManager(QSqlDatabase db);

    void createExam();
    void addByArea(QString section, int amount, QStringList areaNames);
    void addBySubject(QString section, int amount, QStringList subjectNames);
    void addByTopic(QString section, int amount, QStringList topicNames);
    void addAny(QString section, int amount);

    QList<Question> getQuestionList();
    QList<Question> getQuestionList(int lo, int hi);
    Question getQuestionAt(int index);

    QStringList getListOfSections();
    QMap<int, Question> getQuestionsAtSection(QString section);

    int getAmountQuestions();

    QStringList getDBAreas();
    QStringList getDBSubjects(QString area);
    QStringList getDBTopics(QString subject);
    QStringList getDBTopicsByArea(QString area);
    QStringList getDBQuestionListByTopic(QString topic);
    QHash<int, QString> getDBQuestionHashByTopic(QString topic);
    QStringList getDBDetailsOfQuestion(int questionID);

    int getDBAmountQuestions(QString column, QStringList columnNames);

    void clearExamInfo();
    bool addRegistry(QString registryName, QStringList registryValues,
                     int amount, QString criteria);
    bool removeRegistry(QString registryName);
    bool editRegistry(QString oldName, QString newName,
                      QStringList newValues, int newAmount, QString criteria);

    QStringList getRegistryNames();
    QStringList getRegistryValues(QString registryName);
    int getRegistryAmount(QString registryName);
    QString getRegistryCriteria(QString registryName);

    QStringList getSummary();

    void setTime(int time);
    int getTime();
private:
    void addQuestions(QString section, QString column, int amount, QStringList columnNames);
    Question getQuestion(int questionID, QString questionDescription);

    QSqlDatabase db;

    QuizManExam exam;

    int time = 0;
    int sections = 0;
    QMap<int, QString> registryCount;
    QMap<int, QStringList> registers;
    QMap<int, int> infoAmount;
    QMap<int, QString> registryCriteria;
};

#endif // EXAMMANAGER_HPP
