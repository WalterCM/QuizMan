#include "include/Exam/Option.hpp"

void Option::setDescription(QString description)
{
    this->description = description;
}

void Option::setTruthValue(bool truthValue)
{
    this->truthValue = truthValue;
}

QString Option::getDescription()
{
    return this->description;
}

bool Option::isCorrect()
{
    return truthValue;
}
