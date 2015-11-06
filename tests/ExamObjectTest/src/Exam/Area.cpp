#include "include/Exam/Area.hpp"

void Area::setName(QString name)
{
    this->name = name;
}

void Area::setSubjects(QList<Subject> subjects)
{
    this->subjects = subjects;
}

void Area::addSubject(Subject subject)
{
    int newIndex = this->subjects.count() + 1;
    this->subjects[newIndex] = subject;
}

QString Area::getName() const
{
    return this->name;
}

QList<Subject> Area::getSubjects() const
{
    return this->subjects;
}

QList<Question> Area::getQuestions() const
{
    QList<Question> questions;
    for (Subject s: this->subjects)
        questions.append(s.getQuestions());
    return questions;
}

bool Area::subjectExists(Subject subject)
{
    if (!this->subjects.empty())
        foreach (Subject s, this->subjects)
            if (subject == s)
                return true;

    return false;
}

bool Area::operator==(const Area area) const
{
    return this->getName() == area.getName() &&
            this->getSubjects() == area.getSubjects();
}
