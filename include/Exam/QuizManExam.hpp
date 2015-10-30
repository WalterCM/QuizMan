#ifndef QUIZMANEXAM_HPP
#define QUIZMANEXAM_HPP

#include <QMap>
#include "Question.hpp"

class QuizManExam
{
public:
    QuizManExam();
    QMap<int, Question> exam;
};

#endif // QUIZMANEXAM_HPP
