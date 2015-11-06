
#include <QList>
#include "include/Exam/ExamManager.hpp"

ExamManager::ExamManager()
{}

ExamManager::ExamManager(QSqlDatabase db)
{
    this->db = db;
}

void ExamManager::addByArea(int amount, QString areaName)
{
    this->addQuestions("a.AreaName", amount, areaName);
}

void ExamManager::addBySubject(int amount, QString subjectName)
{
    this->addQuestions("s.SubjectName", amount, subjectName);
}

void ExamManager::addByTopic(int amount, QString topicName)
{
    this->addQuestions("t.TopicName", amount, topicName);
}

void ExamManager::addAny(int amount)
{
    this->addQuestions("'1'", amount, "1");
}

QList<Question> ExamManager::getQuestionList()
{
    return this->exam.getQuestions();
}

void ExamManager::addQuestions(QString column, int amount, QString columnName)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        return;
    }

    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT q.QuestionID, q.QuestionDescription, s.SubjectName, a.AreaName\
                          FROM Questions AS q\
                          INNER JOIN TopicsQuestions AS tq\
                          ON tq.QuestionID = q.QuestionID\
                          INNER JOIN Topics AS t\
                          ON t.TopicID = tq.TopicID\
                          INNER JOIN Subjects AS s\
                          ON s.SubjectID = t.SubjectID\
                          INNER JOIN Areas AS a\
                          ON a.AreaID = s.AreaID\
                          WHERE %1 = :columnName\
                          ORDER BY Random() LIMIT :amount").arg(column);

    query.prepare(queryString);
    query.bindValue(":columnName", columnName);
    query.bindValue(":amount", amount);

    query.exec();

    while (query.next()) {
        Question question;
        QString area, subject, description;
        int id = query.value(0).toInt();
        description = query.value(1).toString();
        subject = query.value(2).toString();
        area = query.value(3).toString();
        question = getQuestion(id, description);

        exam.addQuestion(area, subject, question);
    }

    db.close();
}

Question ExamManager::getQuestion(int questionID, QString questionDescription)
{
    Question question;
    QSqlQuery query;
    query.prepare("SELECT OptionDescription, OptionCorrect"
                  "FROM Options WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();
    while (query.next()) {
        qDebug() << "Descripcion de opcion: " << query.value(0).toString();
        Option option;
        option.setDescription(query.value(0).toString());
        option.setTruthValue(query.value(1).toBool());
        question.addOption(option);
    }

    query.prepare("SELECT t.TopicName"
                  "FROM TOpics AS t"
                  "INNER JOIN TopicsQuestions AS tq"
                  "ON tq.TopicID = t.TopicID"
                  "INNER JOIN Questions AS q"
                  "ON q.QuestionID = tq.QUestionID"
                  "WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();
    while (query.next()) {
        question.addTopic(query.value(0).toString());
    }


    question.setID(questionID);
    question.setDescription(questionDescription);
    return question;
}
