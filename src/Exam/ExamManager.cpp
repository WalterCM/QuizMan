
#include <QList>
#include "include/Exam/ExamManager.hpp"

ExamManager::ExamManager()
{}

ExamManager::ExamManager(QSqlDatabase db)
{
    this->db = db;
}

void ExamManager::addByArea(int amount, QStringList areaNames)
{
    this->addQuestions("a.AreaName", amount, areaNames);
}

void ExamManager::addBySubject(int amount, QStringList subjectNames)
{
    this->addQuestions("s.SubjectName", amount, subjectNames);
}

void ExamManager::addByTopic(int amount, QStringList topicNames)
{
    this->addQuestions("t.TopicName", amount, topicNames);
}

void ExamManager::addAny(int amount)
{
    this->addQuestions("Any", amount, QStringList());
}

QList<Question> ExamManager::getQuestionList()
{
    return this->exam.getQuestions();
}

QStringList ExamManager::getDBAreas()
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "5";
    }

    QSqlQuery query;
    query.exec("SELECT AreaName FROM Areas");
    QMap<QString, int> areaList;

    while (query.next()) {
        areaList[query.value(0).toString()] = true;
    }
    db.close();

    return areaList.keys();
}

QStringList ExamManager::getDBSubjects(QString area)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "6";
    }

    QSqlQuery query;
    query.prepare("SELECT s.SubjectName\
                   FROM Subjects AS s\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE a.AreaName = :name");
    query.bindValue(":name", area);
    query.exec();
    QMap<QString, bool> subjectList;
    while (query.next()) {
        subjectList[query.value(0).toString()] = true;
    }
    db.close();

    return subjectList.keys();
}

QStringList ExamManager::getDBTopics(QString subject)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "7";
    }

    QSqlQuery query;
    query.prepare("SELECT t.TopicName\
                   FROM Topics AS t\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   WHERE s.SubjectName = :name");
    query.bindValue(":name", subject);
    query.exec();
    QMap<QString, bool> topicList;

    while (query.next()) {
        topicList[query.value(0).toString()] = true;
    }
    db.close();

    return topicList.keys();
}

QStringList ExamManager::getDBTopicsPerArea(QString area)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "7";
    }

    QSqlQuery query;
    query.prepare("SELECT t.TopicName\
                   FROM Topics AS t\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE a.AreaName = :name");
    query.bindValue(":name", area);
    query.exec();
    QMap<QString, bool> topicList;

    while (query.next()) {
        topicList[query.value(0).toString()] = true;
    }
    db.close();

    return topicList.keys();
}

void ExamManager::clearExamInfo()
{
    if (!infoAmount.empty())
        infoAmount.clear();

    if (!registers.empty())
        registers.clear();
}

void ExamManager::addRegister(QString registerName, QStringList registerValue, int amount)
{
    registryCount.insert(count++, registerName);
    registers.insert(registerName, registerValue);
    infoAmount.insert(registerName, amount);
}

void ExamManager::removeRegister(QString registerName)
{
    infoAmount.remove(registerName);
    registers.remove(registerName);
    registryCount.remove(registryCount.key(registerName));
}

QStringList ExamManager::getRegisterNames()
{
    QStringList registryNames;
    for (int i = 0; i < count; i++) {
        registryNames.push_back(registryCount.value(i));
    }

    return registryNames;
}

QStringList ExamManager::getRegisterValue(QString registerName)
{
    return registers[registerName];
}

int ExamManager::getRegisterAmount(QString registerName)
{
    return infoAmount[registerName];
}

void ExamManager::setTime(int time)
{
    this->time = time;
}

int ExamManager::getTime()
{
    return time;
}

void ExamManager::addQuestions(QString column, int amount, QStringList columnNames)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "8";
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
                          ON a.AreaID = s.AreaID");
    if (columnNames.count() > 0) {
        queryString.append("WHERE ");
        foreach (QString columnName, columnNames) {
            queryString.append("%1 = ").arg(column);
            queryString.append(columnName);
            queryString.append(" AND ");
        }
        queryString.append("TRUE");
    }
    queryString.append("ORDER BY Random() LIMIT :amount");

    query.prepare(queryString);
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
