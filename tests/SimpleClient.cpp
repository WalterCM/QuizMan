#include "../src/QuizMan.hpp"

int main()
{
    QuizMan q("assets/databases/questions.qm");
    q.getRandomQuestions(0);
    return 0;
}