#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <QString>
#include <QList>
#include "Question.hpp"

class Subject
{
public:
    void setName(QString name);
    void setQuestions(QList<Question> questions);
    void addQuestion(Question question);

    QString getName() const;
    QList<Question> getQuestions() const;

    bool questionExists(Question question);

    bool operator==(const Subject &subject) const;
    void randomizeQuestionsOrder();
private:
    QString name;
    QList<Question> questions;
};

#endif // SUBJECT_HPP
