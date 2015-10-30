#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <QString>
#include <QMap>
#include <QtSql>

class Question
{
public:
    Question();
    void setDescription(QString description);
    void setOptions(QMap<int, QString> options);
    void setNewOption(QString newOption, bool correct);
    bool oneCorrect();
    bool isCorrect(int index);
private:
    QString description;
    QMap<int, QString> options;
    QList<int> correct;
};

#endif // QUESTION_HPP
