#include <QDebug>
#include "include/Exam/QuizManExam.hpp"

void QuizManExam::setName(QString name)
{
    this->name = name;
}

void QuizManExam::addQuestion(QString area,
                              QString subject,
                              Question question)
{
    questions[area].insert(subject, question);
}

QString QuizManExam::getName()
{
    return this->name;
}

QStringList QuizManExam::getAreas()
{
    return this->questions.keys();
}

QStringList QuizManExam::getSubjects()
{
    QStringList subjects;
    foreach (auto s, this->questions.values()) {
        subjects.append(s.keys());
    }

    return subjects;
}

QList<Question> QuizManExam::getQuestions()
{
    QList<Question> numberedQuestions;
    foreach (auto s, questions.values())
        numberedQuestions.append(s.values());

    return numberedQuestions;
}

QHash<QString, QMultiHash<QString, Question>> QuizManExam::getQuestionTree()
{
    return this->questions;
}

int QuizManExam::questionCount()
{
    int count = 0;
    foreach (Question q, getQuestions())
        count++;

    return count;
}

bool QuizManExam::questionExists(Question question)
{
    foreach (QString a, questions.keys()) {
        foreach (QString s, questions[a].keys()) {
            if (questions[a].contains(s, question))
                return true;
        }
    }

    return false;
}

bool QuizManExam::questionExists(QString subject, Question question)
{
    foreach (QString a, questions.keys()) {
        if (questions[a].contains(subject, question))
            return true;
    }

    return false;
}

bool QuizManExam::questionExists(QString area, QString subject, Question question)
{
    if (questions.contains(area)) {
        if (questions[area].contains(subject, question))
            return true;
    }

    return false;
}

