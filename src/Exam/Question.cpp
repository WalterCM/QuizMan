#include <QDebug>
#include "include/Exam/Question.hpp"

void Question::setID(int id)
{
    this->id = id;
}

void Question::setDescription(QString description)
{
    this->description = description;
}

void Question::setOptions(QList<Option> options)
{
    this->options = options;
}

void Question::addOption(Option option)
{
    int newIndex = this->options.count() + 1;
    this->options[newIndex] = option;
}

void Question::setTopics(QStringList topics)
{
    this->topics = topics;
}

void Question::addTopic(QString topic)
{
    this->topics.push_back(topic);
}

int Question::getID() const
{
    return this->id;
}

QString Question::getDescription() const
{
    return this->description;
}

QList<Option> Question::getOptions() const
{
    return this->options;
}

QStringList Question::getTopics() const
{
    return this->topics;
}

bool Question::operator==(const Question &question) const
{
    return this->getID() == question.getID() &&
            this->getDescription() == question.getDescription();
}

void Question::randomizeOptionsOrder()
{
    // TODO: Write function to randomize the orden of options
}
