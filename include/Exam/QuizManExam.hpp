#ifndef QUIZMANEXAM_HPP
#define QUIZMANEXAM_HPP

#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include "Question.hpp"

class QuizManExam
{
public:
    void setName(QString name);
    void addQuestions(QString section, QList<Question> questionList);

    QString getName();
    QStringList getAreas();
    QStringList getSubjects();

    QList<Question> getQuestions();
    Question getQuestionAt(int index);

    QStringList getListOfSections();
    QMap<int, Question> getQuestionsAtSection(QString section);

    int questionCount();

    bool questionExists(Question question);
    bool questionExists(QString subject, Question question);
    bool questionExists(QString area, QString subject, Question question);
private:
    QMap<int, QString> sectionMap;
    QMap<int, Question> questionMap;

    QMultiHash<int, int> examTree;

    int sections = 0;
    int questions = 0;
    QString name;
};

#endif // QUIZMANEXAM_HPP
