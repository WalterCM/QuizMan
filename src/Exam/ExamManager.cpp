
#include <QList>
#include <QStringList>
#include "include/Exam/ExamManager.hpp"

ExamManager::ExamManager()
{}

ExamManager::ExamManager(QSqlDatabase db)
{
    this->db = db;
    questionManager = QuestionDBManager(db);
}

QStringList ExamManager::getDBExamList()
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.exec("SELECT ExamName FROM Exams");
    QStringList examList;
    while (query.next()) {
        examList << query.value(0).toString();
    }

    safeClose(openedBefore);

    return examList;
}

QStringList ExamManager::getDBSectionList(QString examName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT s.SectionName\
                   FROM Sections AS s\
                   INNER JOIN Exams AS e\
                   ON e.ExamID = s.ExamID\
                   WHERE e.ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    QStringList sectionList;
    while (query.next()) {
        sectionList << query.value(0).toString();
    }
    if (!openedBefore)
        db.close();

    return sectionList;
}

QStringList ExamManager::getDBRestigerList(QString sectionName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT r.RegisterName\
                   FROM Registers as r\
                   INNER JOIN Sections AS s\
                   ON s.SectionID = r.SectionID\
                   WHERE s.SectionName = :sectionName");
    query.bindValue(":sectionName", sectionName);
    query.exec();
    QStringList registerList;
    while (query.next()) {
        registerList << query.value(0).toString();
    }

    safeClose(openedBefore);

    return registerList;
}

void ExamManager::createExam()
{
    foreach (int index, sectionCount.keys()) {
        QString section = sectionCount[index];
        QStringList list = sections[index];
        QString criteria = sectionCriteria[index];

        if (criteria == "Area") {
            addByArea(section, infoAmount[index], list);
        } else if (criteria == "Subject") {
            addBySubject(section, infoAmount[index], list);
        } else if (criteria == "Topic") {
            addByTopic(section, infoAmount[index], list);
        }
    }
}

void ExamManager::saveExam(QString examName)
{

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

void ExamManager::clearExamInfo()
{
    if (!infoAmount.empty())
        infoAmount.clear();

    if (!sections.empty())
        sections.clear();

    if (!sectionCount.empty())
        sectionCount.clear();

    sectionID = 0;
}

bool ExamManager::addSection(QString sectionName, QStringList sectionValues,
                              int amount, QString criteria)
{
    int maxAmount = questionManager.getDBAmountQuestions(criteria, sectionValues);
    if (amount > maxAmount)
        return false;
    sectionCount.insert(sectionID, sectionName);
    sections.insert(sectionID, sectionValues);
    infoAmount.insert(sectionID, amount);
    sectionCriteria.insert(sectionID++, criteria);

    return true;
}

bool ExamManager::removeSection(QString sectionName)
{
    sectionID = sectionCount.key(sectionName);
    sectionCriteria.remove(sectionID);
    infoAmount.remove(sectionID);
    sections.remove(sectionID);
    sectionCount.remove(sectionID);
    return true;
}

bool ExamManager::editSection(QString oldName, QString newName,
                               QStringList newValues, int newAmount, QString criteria)
{
    int maxAmount = questionManager.getDBAmountQuestions(criteria, newValues);
    if (newAmount > maxAmount)
        return false;

    int index = sectionCount.key(oldName);
    sectionCount[index] = newName;
    sections[index] = newValues;
    infoAmount[index] = newAmount;

    return true;
}

QStringList ExamManager::getSectionNames()
{
    return exam.getListOfSections();
}

QStringList ExamManager::getSectionValues(QString sectionName)
{
    QStringList questions;
    foreach (Question question, exam.getQuestionsAtSection(sectionName).values()) {
        questions << question.getDescription();
    }

    return questions;
}

int ExamManager::getSectionAmount(QString sectionName)
{
    return exam.getAmountOfQuestionsAtSection(sectionName);
}

QString ExamManager::getSectionCriteria(QString sectionName)
{
    return sectionCriteria[sectionCount.key(sectionName)];
}

QStringList ExamManager::getSummary()
{
    QStringList summary;
    summary << ("<b>Cronometro: " + QString::number(time) + " minutos");
    summary << "";
    summary << "<b>Puntos a favor: </b>" + QString::number(pointsPerCorrect);
    summary << "<b>Puntos en contra: </b>" + QString::number(pointsPerMistake);
    summary << "";
    summary << ("<b>Cantidad de secciones: </b>" + QString::number(sectionCount.count()));
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

void ExamManager::setPositive(double points)
{
    this->pointsPerCorrect = points;
}

int ExamManager::getPositive()
{
    return pointsPerCorrect;
}

void ExamManager::setNegative(double points)
{
    this->pointsPerMistake = points;
}

int ExamManager::getNegative()
{
    return pointsPerMistake;
}

void ExamManager::setChosenAnswers(QVector<QString> answers)
{
    this->chosenAnswers = answers;
}

QString ExamManager::getCorrectAnswer(int question)
{
    return correctAnswers[question];
}

QString ExamManager::getChosenAnswer(int question)
{
    return chosenAnswers[question];
}

QStringList ExamManager::getResults()
{
    QStringList result;
    int correctNum = getAmountCorrect();
    int incorrectNum = getAmountAnswered() - getAmountCorrect();
    double maxPoints = getAmountQuestions() * pointsPerCorrect;
    double actualPoints = correctNum * pointsPerCorrect - incorrectNum * pointsPerMistake;

    result << "<h1>Termino el examen!</h1>";
    result << "________________________________";
    result << "";
    result << "<b>Tiempo restante para acabar el examen: </b>" + QString::number(time) + " minutos";
    result << "";
    result << "<b>Puntos ganados por correcta: </b>" + QString::number(pointsPerCorrect);
    result << "<b>Puntos perdidos por incorrecta: </b>" + QString::number(pointsPerMistake);
    result << "";
    result << "<b>Total correctas: </b>" + QString::number(correctNum);
    result << "<b>Total incorrectas: </b>" + QString::number(incorrectNum);
    result << "";
    result << "<b>Puntaje maximo: </b>" + QString::number(maxPoints);
    result << "<b>Puntaje obtenido: </b>" + QString::number(actualPoints);

    return result;
}\

QStringList ExamManager::getResultsByQuestion(int questionID)
{
    QStringList result;
    QString correct = getCorrectAnswer(questionID);
    QString chosen = getChosenAnswer(questionID);
    result << "<b>Enunciado: </b>";
    result << exam.getQuestionAt(questionID + 1).getDescription();
    result << "";
    result << "<b>Respuesta correcta: </b>" + correct;
    result << "<b>Respuesta escogida: </b>" + chosen;

    bool isCorrect = (correct == chosen);
    bool wasAnswered = (chosen != "");
    double neto = wasAnswered * (pointsPerCorrect * isCorrect - pointsPerMistake * !isCorrect);
    result << "<b>Puntaje neto: </b>" + QString::number(neto);

    return result;
}

int ExamManager::getAmountCorrect()
{
    int amount = 0;
    for (int i = 0; i < correctAnswers.size(); i++) {
        if (correctAnswers[i] == chosenAnswers[i])
            amount++;
    }
    return amount;
}

int ExamManager::getAmountCorrectBySection(QString sectionName)
{

}

int ExamManager::getAmountCorrectByArea(QString areaName)
{

}

int ExamManager::getAmountCorrectBySubject(QString subjectName)
{

}

int ExamManager::getAmountCorrectByTopic(QString topicName)
{

}

int ExamManager::getAmountAnswered()
{
    int amountOfAnswers = 0;
    foreach (QString answer, chosenAnswers) {
        if (answer != "")   amountOfAnswers++;
    }

    return amountOfAnswers;
}

void ExamManager::addQuestions(QString section, QString column,
                               int amount, QStringList columnNames)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT q.QuestionID, q.QuestionDescription, q.QuestionImageLocation\
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
        QString imageLocation = query.value(2).toString();
        Question question = getQuestion(id, description, imageLocation);

        questionList << question;
        correctAnswers.push_back(questionManager.getDBOptionCorrect(id));
    }
    exam.addQuestions(section, questionList);

    safeClose(openedBefore);
}

Question ExamManager::getQuestion(int questionID, QString questionDescription,
                                  QString imageLocation)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT OptionDescription, OptionCorrect\
                   FROM Options WHERE QuestionID = :ID\
                   ORDER BY Random()");
    query.bindValue(":ID", questionID);
    query.exec();

    Question question;
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
    question.setImageLocation(imageLocation);

    safeClose(openedBefore);

    return question;
}

bool ExamManager::safeOpen()
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

void ExamManager::safeClose(bool openedBefore)
{
    if (!openedBefore)
        db.close();
}

