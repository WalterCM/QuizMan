#ifndef OPTION_HPP
#define OPTION_HPP

#include <QString>

class Option
{
public:
    void setDescription(QString description);
    void setTruthValue(bool truthValue);

    QString getDescription();
    bool isCorrect();
private:
    QString description;
    bool truthValue;
};

#endif // OPTION_HPP
