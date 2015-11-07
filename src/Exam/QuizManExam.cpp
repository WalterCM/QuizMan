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
    questionTree[area].insert(subject, question);
}

QString QuizManExam::getName()
{
    return this->name;
}

QStringList QuizManExam::getAreas()
{
    return this->questionTree.keys();
}

QStringList QuizManExam::getSubjects()
{
    QStringList subjects;
    foreach (auto s, this->questionTree.values()) {
        subjects.append(s.keys());
    }

    return subjects;
}

QList<Question> QuizManExam::getQuestions()
{
    QList<Question> numberedQuestions;
    foreach (auto s, questionTree.values())
        numberedQuestions.append(s.values());

    return numberedQuestions;
}

QHash<QString, QMultiHash<QString, Question>> QuizManExam::getQuestionTree()
{
    return this->questionTree;
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
    foreach (QString a, questionTree.keys()) {
        foreach (QString s, questionTree[a].keys()) {
            if (questionTree[a].contains(s, question))
                return true;
        }
    }

    return false;
}

bool QuizManExam::questionExists(QString subject, Question question)
{
    foreach (QString a, questionTree.keys()) {
        if (questionTree[a].contains(subject, question))
            return true;
    }

    return false;
}

bool QuizManExam::questionExists(QString area, QString subject, Question question)
{
    if (questionTree.contains(area)) {
        if (questionTree[area].contains(subject, question))
            return true;
    }

    return false;
}


