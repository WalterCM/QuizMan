#ifndef EXAMMANAGER_HPP
#define EXAMMANAGER_HPP

#include <QtSql>
#include <QString>
#include <QMap>
#include "QuizManExam.hpp"
#include "QuestionDBManager.hpp"

class ExamManager
{
public:
    ExamManager();
    ExamManager(QSqlDatabase db);

    QStringList getDBExamList(int examCustom);
    QStringList getDBSectionList(QString examName);
    QStringList getDBRestigerList(QString sectionName);

    int getDBExamDuratioin(QString examName);
    double getDBExamPointsPerCorrect(QString examName);
    double getDBExamPointsPerMistake(QString examName);
    int getDBSectionAmount(QString sectionName);
    QString getDBSectionCriteria(QString sectionName);

    QStringList getDBExamContent(QString examName);
    QStringList getDBSectionContent(QString sectionName);

    void createExam();
    void saveExam(QString examName);

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

    void clearExamInfo();
    bool addSection(QString sectionName, QStringList sectionValues,
                     int amount, QString criteria);
    bool removeSection(QString sectionName);
    bool editSection(QString oldName, QString newName,
                      QStringList newValues, int newAmount, QString criteria);

    QStringList getSectionNames();
    QStringList getSectionValues(QString sectionName);
    int getSectionAmount(QString sectionName);
    QString getSectionCriteria(QString sectionName);

    QStringList getSummary();

    void setDuration(int duration);
    int getDuration();

    void setPositive(double points);
    int getPositive();

    void setNegative(double points);
    int getNegative();

    void setChosenAnswers(QVector<QString> answers);
    QString getCorrectAnswer(int question);
    QString getChosenAnswer(int question);

    QStringList getResults();
    QStringList getResultsByQuestion(int questionID);

    int getAmountCorrect();
    /*int getAmountCorrectBySection(QString sectionName);
    int getAmountCorrectByArea(QString areaName);
    int getAmountCorrectBySubject(QString subjectName);
    int getAmountCorrectByTopic(QString topicName);*/

    int getAmountAnswered();
private:
    void addQuestions(QString section, QString column, int amount, QStringList columnNames);

    Question getQuestion(int questionID, QString questionDescription, QString imageLocation);

    bool safeOpen();
    void safeClose(bool openedBefore);

    QSqlDatabase db;
    QuestionDBManager questionManager;

    QuizManExam exam;

    int duration = 0;

    double pointsPerCorrect = 0.0;
    double pointsPerMistake = 0.0;

    int sectionID = 0;
    QMap<int, QString> sectionCount;
    QMap<int, QStringList> sections;
    QMap<int, int> infoAmount;
    QMap<int, QString> sectionCriterias;

    QVector<QString> correctAnswers;
    QVector<QString> chosenAnswers;
};

#endif // EXAMMANAGER_HPP
