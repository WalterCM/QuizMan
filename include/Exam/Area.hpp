#ifndef AREA_HPP
#define AREA_HPP

#include <QString>
#include <QList>
#include "Subject.hpp"

class Area
{
public:
    void setName(QString name);
    void setSubjects(QList<Subject> subjects);
    void addSubject(Subject subject);

    QString getName() const;
    QList<Subject> getSubjects() const;
    QList<Question> getQuestions() const;

    bool subjectExists(Subject subject);
    bool operator==(const Area area) const;
private:
    QString name;
    QList<Subject> subjects;
};

#endif // AREA_HPP
