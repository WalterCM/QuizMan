
#ifndef QUIZMAN_QUIZMAN_HPP
#define QUIZMAN_QUIZMAN_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<int, std::vector<std::string>> QuestionMap;

class QuizMan
{
public:
    QuizMan();
    QuizMan(std::string filename);
    QuestionMap getRandomQuestions(int amount);
private:
    std::string filename;
    QuestionMap questions;
};

#endif //QUIZMAN_QUIZMAN_HPP
