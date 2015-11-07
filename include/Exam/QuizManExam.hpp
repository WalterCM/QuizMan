#ifndef QUIZMANEXAM_HPP
#define QUIZMANEXAM_HPP

#include <QString>
#include <QList>
#include <QHash>
#include "Question.hpp"

class QuizManExam
{
public:
    void setName(QString name);
    void addQuestion(QString area, QString subject, Question question);

    QString getName();
    QStringList getAreas();
    QStringList getSubjects();

    QList<Question> getQuestions();
    QHash<QString, QMultiHash<QString, Question> > getQuestionTree();

    int questionCount();

    bool questionExists(Question question);
    bool questionExists(QString subject, Question question);
    bool questionExists(QString area, QString subject, Question question);

private:
    QHash<QString, QMultiHash<QString, Question> > questionTree;
    QString name;

};

#endif // QUIZMANEXAM_HPP
