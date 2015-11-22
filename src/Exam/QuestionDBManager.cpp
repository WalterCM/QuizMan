#include "include/Exam/QuestionDBManager.hpp"

QuestionDBManager::QuestionDBManager()
{}

QuestionDBManager::QuestionDBManager(QSqlDatabase db)
{
    this->db = db;
}

void QuestionDBManager::insertDBQuestion(QString areaName,
                                         QString subjectName,
                                         QString questionDescription,
                                         QString questionImageLocation,
                                         QStringList topicNames,
                                         QStringList options,
                                         QList<int> correctOptions)
{
    if (!db.open()) {
        qDebug() << "Database not fonud";
        return;
    }

    int areaID = insertDBArea(areaName);
    int subjectID = insertDBSubject(subjectName, areaID);
    int questionID = insertDBQuestionQuery(questionDescription, questionImageLocation);

    insertDBOptions(options, questionID, correctOptions);
    insertDBTopics(topicNames, subjectID, questionID);
    db.close();
}

void QuestionDBManager::deleteDBQuestion(int questionID)
{
    if (!db.open()) {
        qDebug() << "Database not fonud";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT a.AreaID, s.SubjectID\
                   FROM Questions AS q\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.QuestionID = q.QuestionID\
                   INNER JOIN Topics AS t\
                   ON t.TopicID = tq.TopicID\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE q.QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();
    int areaID = 0;
    int subjectID = 0;
    while (query.next()) {
        areaID = query.value(0).toInt();
        subjectID = query.value(1).toInt();
    }

    query.prepare("DELETE FROM Questions\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    deleteDBOptions(questionID);
    deleteDBTopics(questionID);
    deleteDBSubject(subjectID);
    deleteDBArea(areaID);

    db.close();
}

void QuestionDBManager::editDBQuestion(int questionID, QString areaName, QString subjectName,
                                       QString questionDescription,
                                       QString questionImageLocation,
                                       QStringList topicNames, QStringList options,
                                       QList<int> correctOptions)
{

}

QStringList QuestionDBManager::getDBAreas()
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "5";
    }

    QSqlQuery query;
    query.exec("SELECT AreaName FROM Areas");
    QStringList areaList;

    while (query.next()) {
        areaList << query.value(0).toString();
    }
    db.close();

    return areaList;
}

QStringList QuestionDBManager::getDBSubjects(QString area)
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
    QStringList subjectList;
    while (query.next()) {
        subjectList << query.value(0).toString();
    }
    db.close();

    return subjectList;
}

QStringList QuestionDBManager::getDBTopics(QString subject)
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
    QStringList topicList;

    while (query.next()) {
        topicList << query.value(0).toString();
    }
    db.close();

    return topicList;
}

QStringList QuestionDBManager::getDBTopicsByArea(QString area)
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
    QStringList topicList;

    while (query.next()) {
        topicList << query.value(0).toString();
    }
    db.close();

    return topicList;
}

QStringList QuestionDBManager::getDBQuestionListByTopic(QString topic)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "7";
    }

    QSqlQuery query;
    query.prepare("SELECT q.QuestionDescription\
                   FROM Questions AS q\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.QuestionID = q.QuestionID\
                   INNER JOIN Topics AS t\
                   ON t.TopicID = tq.TopicID\
                   WHERE t.TopicName = :name");
    query.bindValue(":name", topic);
    query.exec();
    QStringList questionList;

    while (query.next()) {
        questionList << query.value(0).toString();
    }
    db.close();

    return questionList;
}

QHash<int, QString> QuestionDBManager::getDBQuestionHashByTopic(QString topic)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "7";
    }

    QSqlQuery query;
    query.prepare("SELECT q.QuestionID, q.QuestionDescription\
                   FROM Questions AS q\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.QuestionID = q.QuestionID\
                   INNER JOIN Topics AS t\
                   ON t.TopicID = tq.TopicID\
                   WHERE t.TopicName = :name");
    query.bindValue(":name", topic);
    query.exec();
    QHash<int, QString> questionList;

    while (query.next()) {
        questionList.insert(query.value(0).toInt(),
                            query.value(1).toString());
    }

    db.close();

    return questionList;
}

QStringList QuestionDBManager::getDBDetailsOfQuestion(int questionID)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "7";
    }
    QStringList detailsList;

    QSqlQuery query;
    query.prepare("SELECT a.AreaName, s.SubjectName, q.QuestionDescription\
                   FROM Questions AS q\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.QuestionID = q.QuestionID\
                   INNER JOIN Topics AS t\
                   ON t.TopicID = tq.TopicID\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE q.QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    while (query.next()) {
        detailsList << QString("Area: " + query.value(0).toString());
        detailsList << QString("Curso: " + query.value(1).toString());
        detailsList << QString("Pregunta: " + query.value(2).toString());
    }

    query.prepare("SELECT t.TopicName\
                   FROM Topics AS t\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.TopicID = t.TopicID\
                   INNER JOIN Questions AS q\
                   ON q.QuestionID = tq.QuestionID\
                   WHERE q.QuestionID = :ID\
                   ORDER BY t.TopicName ASC");
    query.bindValue(":ID", questionID);
    query.exec();

    detailsList << "";
    detailsList << "Temas: ";
    while (query.next()) {
        detailsList << QString("* " + query.value(0).toString());
    }
    query.prepare("SELECT o.OptionDescription\
                   FROM Options AS o\
                   INNER JOIN Questions AS q\
                   ON q.QuestionID = o.QuestionID\
                   WHERE q.QuestionID = :ID\
                   ORDER BY o.OptionDescription ASC");
    query.bindValue(":ID", questionID);
    query.exec();

    detailsList << "";
    detailsList << "Opciones: ";
    while (query.next()) {
        detailsList << QString("* " + query.value(0).toString());
    }
    db.close();

    return detailsList;
}

QStringList QuestionDBManager::getDBOptions(int questionID)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        return QStringList();
    }

    QSqlQuery query;

    query.prepare("SELECT o.OptionDescription\
                   FROM Options AS o\
                   INNER JOIN Questions AS q\
                   ON q.QuestionID = o.QuestionID\
                   WHERE q.QuestionDescription = :questionDescription");
    query.bindValue(":questionID", questionID);
    query.exec();

    QStringList options;
    while (query.next())
        options << query.value(0).toString();

    return options;
}

int QuestionDBManager::getDBAmountQuestions(QString column, QStringList columnNames)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "10";
        return 0;
    }

    if (column == "Topic")
        column = "t.TopicName";
    else if (column == "Subject")
        column = "s.SubjectName";
    else if (column == "Area")
        column = "a.AreaName";

    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT COUNT(*)\
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
        queryString.append(" WHERE ");
        foreach (QString columnName, columnNames) {
            queryString.append(column + " = '" + columnName);
            queryString.append("' OR ");
        }
        queryString.append("NULL");
    }

    query.exec(queryString);

    query.next();
    int amount = query.value(0).toInt();

    db.close();

    return amount;
}

int QuestionDBManager::getDBLastID()
{
    if (!db.open()) {
        qDebug() << "Database not found";
    }
    QSqlQuery query;

    query.exec("SELECT MAX(QuestionID)\
                FROM Questions");
    query.next();
    int maxID = 1;
    int count = query.value(0).toInt();

    if (count > 0)
        maxID = count + 1;

    db.close();
    return maxID;
}

int QuestionDBManager::insertDBArea(QString areaName)
{
    QSqlQuery query;

    query.prepare("INSERT OR IGNORE\
                   INTO Areas(AreaName)\
                   VALUES(:name)");
    query.bindValue(":name", areaName);
    query.exec();


    query.prepare("SELECT AreaID\
                   FROM Areas\
                   WHERE AreaName = :name");
    query.bindValue(":name", areaName);
    query.exec();

    while (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

int QuestionDBManager::insertDBSubject(QString subjectName, int areaID)
{
    QSqlQuery query;

    query.prepare("INSERT OR IGNORE\
                   INTO Subjects(SubjectName, AreaID)\
                   VALUES(:subjectName, :areaID)");
    query.bindValue(":subjectName", subjectName);
    query.bindValue(":areaID", areaID);
    query.exec();


    query.prepare("SELECT SubjectID\
                   FROM Subjects\
                   WHERE SubjectName = :name");
    query.bindValue(":name", subjectName);
    query.exec();

    while (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

int QuestionDBManager::insertDBQuestionQuery(QString questionDescription,
                                             QString questionImageLocation)
{
    QSqlQuery query;

    query.prepare("INSERT OR IGNORE\
                   INTO Questions(QuestionDescription, QuestionImageLocation)\
                   VALUES(:questionDescription, :questionImageLocation)");
    query.bindValue(":questionDescription", questionDescription);
    query.bindValue(":questionImageLocation", questionImageLocation);
    query.exec();


    query.prepare("SELECT QuestionID\
                   FROM Questions\
                   WHERE QuestionDescription = :questionDescription");
    query.bindValue(":questionDescription", questionDescription);
    query.exec();

    while (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

void QuestionDBManager::insertDBTopics(QStringList topics, int subjectID,
                                       int questionID)
{
    QSqlQuery query;

    foreach (QString topic, topics) {
        query.prepare("INSERT OR IGNORE\
                       INTO Topics(TopicName, SubjectID)\
                       VALUES(:topicName, :subjectID)");
        query.bindValue(":topicName", topic);
        query.bindValue(":subjectID", subjectID);
        query.exec();

        query.prepare("SELECT TopicID\
                       FROM Topics\
                       WHERE TopicName = :topicName");
        query.bindValue(":topicName", topic);
        query.exec();
        query.next();
        int topicID = query.value(0).toInt();


        qDebug() << "topicID: " << topicID;
        qDebug() << "questionID: " << questionID;
        query.prepare("INSERT OR IGNORE\
                       INTO TopicsQuestions(TopicID, QuestionID)\
                       VALUES(:topicID, :questionID)");
        query.bindValue(":topicID", topicID);
        query.bindValue(":questionID", questionID);
        query.exec();
    }
}

void QuestionDBManager::insertDBOptions(QStringList options, int questionID,
                                        QList<int> correctOptions)
{
    QSqlQuery query;

    for (int i = 0; i < options.count(); i++) {
        bool optionCorrect = false;
        if (correctOptions.contains(i))
            optionCorrect = true;

        qDebug() << "opcion: " << options.at(i);
        query.prepare("INSERT OR IGNORE\
                       INTO Options(OptionDescription, QuestionID, OptionCorrect)\
                       VALUES(:optionDescription, :questionID, :optionCorrect)");
        query.bindValue(":optionDescription", options.at(i));
        query.bindValue(":questionID", questionID);
        query.bindValue(":optionCorrect", optionCorrect);
        query.exec();
    }
}

void QuestionDBManager::deleteDBArea(int areaID)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*)\
                   FROM Subjects AS s\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE a.AreaID = :ID");
    query.bindValue(":ID", areaID);
    query.exec();
    query.next();
    if (query.value(0).toInt() == 0) {
        query.prepare("DELETE FROM Areas\
                       WHERE AreaID = :ID");
        query.bindValue(":ID", areaID);
        query.exec();
        query.next();
    }
}

void QuestionDBManager::deleteDBSubject(int subjectID)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*)\
                   FROM Topics AS t\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   WHERE s.SubjectID = :ID");
    query.bindValue(":ID", subjectID);
    query.exec();
    query.next();
    if (query.value(0).toInt() == 0) {
        query.prepare("DELETE FROM Subjects\
                       WHERE SubjectID = :ID");
        query.bindValue(":ID", subjectID);
        query.exec();
        query.next();
    }
}

void QuestionDBManager::deleteDBTopics(int questionID)
{
    QSqlQuery query;
    query.prepare("SELECT TopicID\
                   FROM TopicsQuestions\
                   WHERE QuestionID = :ID");

    query.bindValue(":ID", questionID);
    query.exec();

    QList<int> topicsID;
    while(query.next()) {
        topicsID << query.value(0).toInt();
    }

    query.prepare("DELETE FROM TopicsQuestions\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    foreach (int topicID, topicsID) {
        query.prepare("SELECT COUNT(*)\
                       FROM TopicsQuestions AS tq\
                       INNER JOIN Topics AS t\
                       ON t.TopicID = tq.TopicID\
                       WHERE tq.TopicID = :ID");
        query.bindValue(":ID", topicID);
        query.exec();
        query.next();
        if (query.value(0).toInt() == 0) {
            query.prepare("DELETE FROM Topics\
                           WHERE TopicID = :ID");
            query.bindValue(":ID", topicID);
            query.exec();
            query.next();
        }
    }
}

void QuestionDBManager::deleteDBOptions(int questionID)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Options\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();
}

void QuestionDBManager::editQuestionquery(int questionID,
                                          QString questionDescription,
                                          QString questionImageLocation)
{
    QSqlQuery query;
    query.prepare("UPDATE Questions\
                   SET QuestionDescription = :questionDescription,\
                       QuestionImageLocation = :questionImageLocation\
                   WHERE QuestionID = :questionID");
    query.bindValue(":questionDescription", questionDescription);
    query.bindValue(":questionImageLocation", questionImageLocation);
    query.bindValue(":questionID", questionID);
    query.exec();
}
