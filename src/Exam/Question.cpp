#include "include/Exam.hpp"

Question::Question()
{
}

void Question::setDescription(QString description)
{
    this->description = description;
}

void Question::setOptions(QMap<int, QString> options)
{
    this->options = options;
}

void Question::setNewOption(QString newOption, bool correct)
{
    int index = this->options.count() + 1;
    this->options[index] = newOption;
    if (correct)
        this->correct.push_back(index);
}

bool Question::oneCorrect()
{
    return this->correct.count() == 1;
}

bool Question::isCorrect(int index)
{
    for (int i : this->correct) {
        if (index == i)
            return true;
    }
    return false;
}
