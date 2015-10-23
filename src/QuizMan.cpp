
#include "QuizMan.hpp"
#include "SQLiteCpp/SQLiteCpp.h"

QuizMan::QuizMan()
{
    filename = ":memory:";
}

QuizMan::QuizMan(std::string filename)
{
    this->filename = filename;
}

QuestionMap QuizMan::getRandomQuestions(int amount)
{
    try
    {
        // Open a database file
        SQLite::Database    db(filename);

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement   query(db, "SELECT * FROM Questions");

        while (query.executeStep()) {
            int i = query.getColumn(0);
            questions[i].push_back(query.getColumn(1));
            questions[i].push_back(query.getColumn(2));

            for (std::string s : questions[i]) {
                std::cout << s << " ";
            }
            std::cout << std::endl;
        }

        return questions;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}
