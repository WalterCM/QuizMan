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
    bool openedBefore = safeOpen();

    int areaID = insertDBArea(areaName);
    int subjectID = insertDBSubject(subjectName, areaID);
    int questionID = insertDBQuestionQuery(questionDescription, questionImageLocation);

    insertDBOptions(options, questionID, correctOptions);
    insertDBTopics(topicNames, subjectID, questionID);

    safeClose(openedBefore);
}

void QuestionDBManager::deleteDBQuestion(int questionID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);
}

void QuestionDBManager::editDBQuestion(int questionID, QString areaName, QString subjectName,
                                       QString questionDescription,
                                       QString questionImageLocation,
                                       QStringList topicNames, QStringList options,
                                       QList<int> correctOptions)
{
    bool openedBefore = safeOpen();

    deleteDBQuestion(questionID);
    insertDBQuestion(areaName, subjectName, questionDescription, questionImageLocation,
                     topicNames, options, correctOptions);

    safeClose(openedBefore);
}

int QuestionDBManager::getDBQuestionID(QString questionDescription)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT QuestionID\
                   FROM Questions\
                   WHERE QuestionDescription = :questionDescription");
    query.bindValue(":questionDescription", questionDescription);
    query.exec();
    int questionID = 0;

    while (query.next()) {
        questionID << query.value(0).toInt();
    }

    safeClose(openedBefore);

    return questionID;
}

QStringList QuestionDBManager::getDBAreas()
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.exec("SELECT AreaName FROM Areas");
    QStringList areaList;

    while (query.next()) {
        areaList << query.value(0).toString();
    }

    safeClose(openedBefore);

    return areaList;
}

QString QuestionDBManager::getDBAreaByQuestion(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT AreaName FROM Areas AS a\
                   INNER JOIN Subjects AS s\
                   ON s.AreaID = a.AreaID\
                   INNER JOIN Topics AS t\
                   ON t.SubjectID = s.SubjectID\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.TopicID = t.TopicID\
                   WHERE tq.QuestionID = :questionID");
    query.bindValue(":questionID", questionID);
    query.exec();
    QString areaName;

    while (query.next()) {
        areaName = query.value(0).toString();
    }

    safeClose(openedBefore);

    return areaName;
}

QStringList QuestionDBManager::getDBSubjects(QString area)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return subjectList;
}

QString QuestionDBManager::getDBSubjectByQuestion(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT SubjectName FROM Subjects AS s\
                   INNER JOIN Topics AS t\
                   ON t.SubjectID = s.SubjectID\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.TopicID = t.TopicID\
                   WHERE tq.QuestionID = :questionID");
    query.bindValue(":questionID", questionID);
    query.exec();
    QString subjectName;

    while (query.next()) {
        subjectName = query.value(0).toString();
    }
    safeClose(openedBefore);

    return subjectName;
}

QStringList QuestionDBManager::getDBTopics(QString subject)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return topicList;
}

QStringList QuestionDBManager::getDBTopicsByArea(QString area)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return topicList;
}

QStringList QuestionDBManager::getDBTopicsByQuestionID(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT t.TopicName FROM Topics AS t\
                   INNER JOIN TopicsQuestions AS tq\
                   ON tq.TopicID = t.TopicID\
                   WHERE tq.QuestionID = :questionID");
    query.bindValue(":questionID", questionID);
    query.exec();
    QStringList topicNames;

    while (query.next()) {
        topicNames << query.value(0).toString();
    }

    safeClose(openedBefore);

    return topicNames;
}

QString QuestionDBManager::getDBQuestionDescription(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT QuestionDescription\
                   FROM Questions\
                   WHERE QuestionID = :questionID");
            query.bindValue(":questionID", questionID);
    query.exec();
    QString questionDescription;

    while (query.next()) {
        questionDescription = query.value(0).toString();
    }

    safeClose(openedBefore);

    return questionDescription;
}

QStringList QuestionDBManager::getDBQuestionListByTopic(QString topic)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return questionList;
}

QHash<int, QString> QuestionDBManager::getDBQuestionHashByTopic(QString topic)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return questionList;
}

QStringList QuestionDBManager::getDBDetailsOfQuestion(int questionID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return detailsList;
}

QStringList QuestionDBManager::getDBOptions(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;

    query.prepare("SELECT o.OptionDescription\
                   FROM Options AS o\
                   INNER JOIN Questions AS q\
                   ON q.QuestionID = o.QuestionID\
                   WHERE q.QuestionID = :questionID");
    query.bindValue(":questionID", questionID);
    query.exec();

    QStringList options;
    while (query.next())
        options << query.value(0).toString();

    safeClose(openedBefore);

    return options;
}

QString QuestionDBManager::getDBOptionCorrect(int questionID)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;

    query.prepare("SELECT o.OptionDescription\
                   FROM Options AS o\
                   INNER JOIN Questions AS q\
                   ON q.QuestionID = o.QuestionID\
                   WHERE q.QuestionID = :questionID AND\
                         o.OptionCorrect = 1\
                   LIMIT 1");
    query.bindValue(":questionID", questionID);
    query.exec();

    QString correct;
    while (query.next())
        correct = query.value(0).toString();

    safeClose(openedBefore);

    return correct;
}

int QuestionDBManager::getDBAmountQuestions(QString column, QStringList columnNames)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return amount;
}

int QuestionDBManager::getDBLastID()
{
    bool openedBefore = safeOpen();

    QSqlQuery query;

    query.exec("SELECT MAX(QuestionID)\
                FROM Questions");
    query.next();
    int maxID = 1;
    int count = query.value(0).toInt();

    if (count > 0)
        maxID = count + 1;

    safeClose(openedBefore);

    return maxID;
}

int QuestionDBManager::insertDBArea(QString areaName)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return 0;
}

int QuestionDBManager::insertDBSubject(QString subjectName, int areaID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return 0;
}

int QuestionDBManager::insertDBQuestionQuery(QString questionDescription,
                                             QString questionImageLocation)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);

    return 0;
}

void QuestionDBManager::insertDBTopics(QStringList topics, int subjectID,
                                       int questionID)
{
    bool openedBefore = safeOpen();

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

        query.prepare("INSERT OR IGNORE\
                       INTO TopicsQuestions(TopicID, QuestionID)\
                       VALUES(:topicID, :questionID)");
        query.bindValue(":topicID", topicID);
        query.bindValue(":questionID", questionID);
        query.exec();
    }

    safeClose(openedBefore);
}

void QuestionDBManager::insertDBOptions(QStringList options, int questionID,
                                        QList<int> correctOptions)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;

    for (int i = 0; i < options.count(); i++) {
        bool optionCorrect = false;
        if (correctOptions.contains(i))
            optionCorrect = true;
        query.prepare("INSERT OR IGNORE\
                       INTO Options(OptionDescription, QuestionID, OptionCorrect)\
                       VALUES(:optionDescription, :questionID, :optionCorrect)");
        query.bindValue(":optionDescription", options.at(i));
        query.bindValue(":questionID", questionID);
        query.bindValue(":optionCorrect", optionCorrect);
        query.exec();
    }

    safeClose(openedBefore);
}

void QuestionDBManager::deleteDBArea(int areaID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);
}

void QuestionDBManager::deleteDBSubject(int subjectID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);
}

void QuestionDBManager::deleteDBTopics(int questionID)
{
    bool openedBefore = safeOpen();

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

    safeClose(openedBefore);
}

void QuestionDBManager::deleteDBOptions(int questionID)
{
    bool openedBefore = safeOpen();
    QSqlQuery query;
    query.prepare("DELETE FROM Options\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    if (!openedBefore)
        db.close();
}

bool QuestionDBManager::safeOpen()
{
    bool openedBefore = false;
    if (db.isOpen())
        openedBefore = true;

    if (!openedBefore) {
        if (!db.open()) {
            qDebug() << "Database not fonud";
            exit(0);
        }
    }

    return openedBefore;
}

void QuestionDBManager::safeClose(bool openedBefore)
{
    if (!openedBefore)
        db.close();
}
