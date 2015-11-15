#include <QDebug>
#include "include/Exam/QuizManExam.hpp"

void QuizManExam::setName(QString name)
{
    this->name = name;
}

void QuizManExam::addQuestions(QString section, QList<Question> questionList)
{
    sectionMap[++sections] = section;
    foreach (Question question, questionList) {
        questionMap[++questions] = question;
        examTree.insert(sections, questions);
    }
}

QString QuizManExam::getName()
{
    return this->name;
}

QList<Question> QuizManExam::getQuestions()
{
    return questionMap.values();
}

Question QuizManExam::getQuestionAt(int index)
{
    return questionMap[index];
}

QStringList QuizManExam::getListOfSections()
{
    return sectionMap.values();
}

QMap<int, Question> QuizManExam::getQuestionsAtSection(QString section)
{
    int sectionIndex = sectionMap.key(section);

    QMap<int, Question> questions;
    foreach (int questionIndex, examTree.values()) {
        int index = examTree.key(questionIndex);
        if (index == sectionIndex)
            questions.insert(questionIndex, questionMap[questionIndex]);
    }

    return questions;
}

int QuizManExam::questionCount()
{
    return questions;
}

bool QuizManExam::questionExists(Question question)
{
    return getQuestions().contains(question);
}

