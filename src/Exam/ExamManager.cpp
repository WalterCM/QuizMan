
#include <QList>
#include <QStringList>
#include "include/Exam/ExamManager.hpp"

ExamManager::ExamManager()
{}

ExamManager::ExamManager(QSqlDatabase db)
{
    this->db = db;
}

void ExamManager::createExam()
{
    foreach (int index, registryCount.keys()) {
        QString section = registryCount[index];
        QStringList list = registers[index];
        QString criteria = registryCriteria[index];

        if (criteria == "Area") {
            addByArea(section, infoAmount[index], list);
        } else if (criteria == "Subject") {
            addBySubject(section, infoAmount[index], list);
        } else if (criteria == "Topic") {
            addByTopic(section, infoAmount[index], list);
        }
    }
}

void ExamManager::addByArea(QString section, int amount, QStringList areaNames)
{
    this->addQuestions(section, "a.AreaName", amount, areaNames);
}

void ExamManager::addBySubject(QString section, int amount, QStringList subjectNames)
{
    this->addQuestions(section, "s.SubjectName", amount, subjectNames);
}

void ExamManager::addByTopic(QString section, int amount, QStringList topicNames)
{
    this->addQuestions(section, "t.TopicName", amount, topicNames);
}

void ExamManager::addAny(QString section, int amount)
{
    this->addQuestions(section, "Any", amount, QStringList());
}

QList<Question> ExamManager::getQuestionList()
{
    return this->exam.getQuestions();
}

QList<Question> ExamManager::getQuestionList(int lo, int hi)
{
    QList<Question> questions = exam.getQuestions();
    QList<Question> section;

    for (int i = lo; i < hi; i++) {
        section.push_back(questions.at(i));
    }

    return section;
}

Question ExamManager::getQuestionAt(int index)
{
    return exam.getQuestionAt(index);
}

QStringList ExamManager::getListOfSections()
{
    return exam.getListOfSections();
}

QMap<int, Question> ExamManager::getQuestionsAtSection(QString section)
{
    return exam.getQuestionsAtSection(section);
}

int ExamManager::getAmountQuestions()
{
    int amount = 0;
    foreach (int amounts, infoAmount.values())
        amount += amounts;

    return amount;
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

int ExamManager::getDBAmountQuestions(QString column, QStringList columnNames)
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

void ExamManager::clearExamInfo()
{
    if (!infoAmount.empty())
        infoAmount.clear();

    if (!registers.empty())
        registers.clear();

    if (!registryCount.empty())
        registryCount.clear();

    count = 0;
}

bool ExamManager::addRegistry(QString registryName, QStringList registryValues,
                              int amount, QString criteria)
{
    int maxAmount = getDBAmountQuestions(criteria, registryValues);
    if (amount > maxAmount)
        return false;
    registryCount.insert(count, registryName);
    registers.insert(count, registryValues);
    infoAmount.insert(count, amount);
    registryCriteria.insert(count++, criteria);

    return true;
}

bool ExamManager::removeRegistry(QString registryName)
{
    int oldCount = count;
    count = registryCount.key(registryName);
    registryCriteria.remove(count);
    infoAmount.remove(count);
    registers.remove(count);
    registryCount.remove(count);

    for (int i = count; i < oldCount - 1; i++) {
        registryCount[i] = registryCount[i + 1];
        registers[i] = registers[i + 1];
        infoAmount[i] = infoAmount[i + 1];
    }

    return true;
}

bool ExamManager::editRegistry(QString oldName, QString newName,
                               QStringList newValues, int newAmount, QString criteria)
{
    int maxAmount = getDBAmountQuestions(criteria, newValues);
    if (newAmount > maxAmount)
        return false;

    int index = registryCount.key(oldName);
    registryCount[index] = newName;
    registers[index] = newValues;
    infoAmount[index] = newAmount;

    return true;
}

QStringList ExamManager::getRegistryNames()
{
    QStringList registryNames;
    for (int i = 0; i < count; i++) {
        registryNames.push_back(registryCount.value(i));
    }

    return registryNames;
}

QStringList ExamManager::getRegistryValues(QString registryName)
{
    return registers[registryCount.key(registryName)];;
}

int ExamManager::getRegistryAmount(QString registryName)
{
    return infoAmount[registryCount.key(registryName)];
}

QString ExamManager::getRegistryCriteria(QString registryName)
{
    return registryCriteria[registryCount.key(registryName)];
}

QStringList ExamManager::getSummary()
{
    QStringList summary;
    summary << ("<b>Cronometro: " + QString::number(time) + " minutos");
    summary << ("<b>Cantidad de secciones: </b>" + QString::number(count));
    summary << ("<b>Cantidad de preguntas: </b>" + QString::number(getAmountQuestions()));

    return summary;
}

void ExamManager::setTime(int time)
{
    this->time = time;
}

int ExamManager::getTime()
{
    return time;
}

void ExamManager::addQuestions(QString section, QString column, int amount, QStringList columnNames)
{
    if (!db.open()) {
        qDebug() << "Database not found";
        qDebug() << "8";
        return;
    }

    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT q.QuestionID, q.QuestionDescription\
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
        queryString.append("NULL ");
    }

    queryString.append("ORDER BY Random() LIMIT :amount");
    query.prepare(queryString);
    query.bindValue(":amount", amount);

    query.exec();

    QList<Question> questionList;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString description = query.value(1).toString();
        qDebug() << "description: " << description;
        Question question = getQuestion(id, description);

        questionList << question;
    }
    exam.addQuestions(section, questionList);
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
