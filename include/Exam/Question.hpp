#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <QString>
#include <QList>
#include <QStringList>
#include "Option.hpp"

class Question
{
public:
    void setID(int id);
    void setDescription(QString description);
    void setOptions(QList<Option> options);
    void addOption(Option option);
    void setTopics(QStringList topics);
    void addTopic(QString topic);

    int getID() const;
    QString getDescription() const;
    QList<Option> getOptions() const;
    QStringList getTopics() const;

    bool operator==(const Question &question) const;

    void randomizeOptionsOrder();
private:
    int id;
    QString description;
    QList<Option> options;
    QStringList topics;
};

#endif // QUESTION_HPP
