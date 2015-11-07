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

    void addByArea(int amount, QString areaName);
    void addBySubject(int amount, QString subjectName);
    void addByTopic(int amount, QString topicName);
    void addAny(int amount);

    QList<Question> getQuestionList();

    QStringList getDBAreas();
    QHash<QString, QStringList> getDBSubjectTree();
    QHash<QString, QHash<QString, QStringList> > getDBTopicTree();
private:
    void addQuestions(QString column, int amount, QString colimnName);
    Question getQuestion(int questionID, QString questionDescription);
    QSqlDatabase db;
    QuizManExam exam;
};

#endif // EXAMMANAGER_HPP
