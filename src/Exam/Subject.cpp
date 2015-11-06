#include "include/Exam/Subject.hpp"

void Subject::setName(QString name)
{
    this->name = name;
}

void Subject::setQuestions(QList<Question> questions)
{
    this->questions = questions;
}

void Subject::addQuestion(Question question)
{
    if (!questionExists(question))
        questions.push_back(question);
}

QString Subject::getName() const
{
    return this->name;
}

QList<Question> Subject::getQuestions() const
{
    return this->questions;
}

bool Subject::questionExists(Question question)
{
    if (!this->questions.empty())
        foreach (Question q, this->questions)
            if (q == question)
                return true;

    return false;
}

bool Subject::operator==(const Subject &subject) const
{
    return this->getName() == subject.getName() &&
            this->getQuestions() == this->getQuestions();
}

void Subject::randomizeQuestionsOrder()
{
    // TODO: Write the function that randomize the order of questions
}
