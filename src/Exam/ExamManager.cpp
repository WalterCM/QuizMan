
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

QStringList ExamManager::getDBExamList(int examCustom)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT ExamName FROM Exams\
                   WHERE ExamCustom = :examCustom");
    query.bindValue(":examCustom", examCustom);
    query.exec();
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

int ExamManager::getDBExamDuratioin(QString examName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT ExamDuration\
                   FROM Exams\
                   WHERE ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    int examDuration;
    while (query.next()) {
        examDuration = query.value(0).toInt();
    }

    safeClose(openedBefore);

    return examDuration;
}

double ExamManager::getDBExamPointsPerCorrect(QString examName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT ExamPointsFavor\
                   FROM Exams\
                   WHERE ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    double examPointsPerCorrect;
    while (query.next()) {
        examPointsPerCorrect = query.value(0).toDouble();
    }

    safeClose(openedBefore);

    return examPointsPerCorrect;
}

double ExamManager::getDBExamPointsPerMistake(QString examName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT ExamPointsAgainst\
                   FROM Exams\
                   WHERE ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    double examPointsPerMistake;
    while (query.next()) {
        examPointsPerMistake = query.value(0).toDouble();
    }

    safeClose(openedBefore);

    return examPointsPerMistake;
}

int ExamManager::getDBSectionAmount(QString sectionName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT SectionNumberQuestions\
                   FROM Sections\
                   WHERE SectionName = :sectionName");
    query.bindValue(":sectionName", sectionName);
    query.exec();

    int sectionNumberQuestions;
    while (query.next()) {
        sectionNumberQuestions = query.value(0).toInt();
    }

    safeClose(openedBefore);

    return sectionNumberQuestions;
}

QString ExamManager::getDBSectionCriteria(QString sectionName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT SectionCriteria\
                   FROM Sections\
                   WHERE SectionName = :sectionName");
    query.bindValue(":sectionName", sectionName);
    query.exec();

    QString sectionCriteria;
    while (query.next()) {
        sectionCriteria = query.value(0).toString();
    }

    safeClose(openedBefore);

    return sectionCriteria;
}

QStringList ExamManager::getDBExamContent(QString examName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT ExamDuration, ExamPointsFavor, ExamPointsAgainst\
                   FROM Exams\
                   WHERE ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    QStringList examContent;
    while (query.next()) {
        examContent << ("Nombre del examen: " + examName);
        examContent << "";
        examContent << ("Duracion del examen: " + query.value(0).toString());
        examContent << ("Puntos a favor: " + query.value(1).toString());
        examContent << ("Puntos en contra: " + query.value(2).toString());
    }

    safeClose(openedBefore);

    return examContent;
}

QStringList ExamManager::getDBSectionContent(QString sectionName)
{
    bool openedBefore = safeOpen();

    QSqlQuery query;
    query.prepare("SELECT SectionID, SectionNumberQuestions\
                   FROM Sections\
                   WHERE SectionName = :sectionName");
    query.bindValue(":sectionName", sectionName);
    query.exec();
    QStringList sectionContent;
    int sectionID;
    while (query.next()) {
        sectionID = query.value(0).toInt();
        sectionContent << ("Cantidad: " + query.value(1).toString());
        sectionContent << "";
    }

    query.prepare("SELECT RegisterName\
                   FROM Registers\
                   WHERE SectionID = :sectionID");
    query.bindValue(":sectionID", sectionID);
    query.exec();

    while (query.next()) {
        sectionContent << query.value(0).toString();
    }

    safeClose(openedBefore);

    return sectionContent;
}

void ExamManager::createExam()
{
    exam = QuizManExam();
    foreach (int index, sectionCount.keys()) {
        QString section = sectionCount[index];
        QStringList list = sections[index];
        QString criteria = sectionCriterias[index];

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
    bool openedBefore = safeOpen();

    QSqlQuery query;

    query.prepare("INSERT INTO Exams (ExamName, ExamDuration, ExamPointsFavor, ExamPointsAgainst, ExamCustom)\
                   VALUES (:examName, :examDuration, :examPointsFavor, :examPointsAgainst, 1)");
    query.bindValue(":examName", examName);
    query.bindValue(":examDuration", duration);
    query.bindValue(":examPointsFavor", pointsPerCorrect);
    query.bindValue(":examPointsAgainst", pointsPerMistake);
    query.exec();

    query.prepare("SELECT examID\
                   FROM Exams\
                   WHERE ExamName = :examName");
    query.bindValue(":examName", examName);
    query.exec();
    query.next();

    int examID = query.value(0).toInt();

    foreach (int sectionKey, sectionCount.keys()) {
        QString sectionName = sectionCount[sectionKey];
        int sectionNumberQuestions = infoAmount[sectionKey];
        QString sectionCriteria = sectionCriterias[sectionKey];

        QSqlQuery sectionQuery;
        sectionQuery.prepare("INSERT INTO Sections (SectionName, ExamID, SectionNumberQuestions, SectionCriteria)\
                              VALUES (:sectionName, :examID, :sectionNumberQuestions, :sectionCriteria)");

        sectionQuery.bindValue(":sectionName", sectionName);
        sectionQuery.bindValue(":examID", examID);
        sectionQuery.bindValue(":sectionNumberQuestions", sectionNumberQuestions);
        sectionQuery.bindValue(":sectionCriteria", sectionCriteria);

        sectionQuery.exec();

        sectionQuery.prepare("SELECT sectionID\
                              FROM Sections\
                              WHERE SectionName = :sectionName");
        sectionQuery.bindValue(":sectionName", sectionName);
        sectionQuery.exec();
        sectionQuery.next();

        int sectionID = sectionQuery.value(0).toInt();
        foreach (QString registerName, sections[sectionKey]) {
            QSqlQuery registerQuery;
            registerQuery.prepare("INSERT INTO Registers (RegisterName, SectionID)\
                                   VALUES (:registerName, :sectionID)");
            registerQuery.bindValue(":registerName", registerName);
            registerQuery.bindValue(":sectionID", sectionID);
            qDebug() << "registeRName: " << registerName;
            qDebug() << "sectionID: " << sectionID;
            registerQuery.exec();
            qDebug() << "debugging1" << sectionQuery.lastError();
        }
    }

    safeClose(openedBefore);
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
    if (!sectionCriterias.empty())
        sectionCount.clear();

    if (!infoAmount.empty())
        infoAmount.clear();

    if (!sections.empty())
        sections.clear();

    if (!sectionCount.empty())
        sectionCount.clear();

    sectionID = 0;

    duration = 0;
    pointsPerCorrect = 0;
    pointsPerMistake = 0;

    if (!correctAnswers.empty())
        correctAnswers.clear();

    if (!chosenAnswers.empty())
        chosenAnswers.clear();
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
    sectionCriterias.insert(sectionID++, criteria);

    return true;
}

bool ExamManager::removeSection(QString sectionName)
{
    sectionID = sectionCount.key(sectionName);
    sectionCriterias.remove(sectionID);
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
    return sections[sectionCount.key(sectionName)];
}

int ExamManager::getSectionAmount(QString sectionName)
{
    return infoAmount[sectionCount.key(sectionName)];
}

QString ExamManager::getSectionCriteria(QString sectionName)
{
    return sectionCriterias[sectionCount.key(sectionName)];
}

QStringList ExamManager::getSummary()
{
    QStringList summary;
    summary << ("<b>Cronometro: " + QString::number(duration) + " minutos");
    summary << "";
    summary << "<b>Puntos a favor: </b>" + QString::number(pointsPerCorrect);
    summary << "<b>Puntos en contra: </b>" + QString::number(pointsPerMistake);
    summary << "";
    summary << ("<b>Cantidad de secciones: </b>" + QString::number(sectionCount.count()));
    summary << ("<b>Cantidad de preguntas: </b>" + QString::number(getAmountQuestions()));

    return summary;
}

void ExamManager::setDuration(int duration)
{
    this->duration = duration;
}

int ExamManager::getDuration()
{
    return duration;
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
    int incorrectNum = getAmountAnswered() - correctNum;
    double maxPoints = getAmountQuestions() * pointsPerCorrect;
    double actualPoints = correctNum * pointsPerCorrect - incorrectNum * pointsPerMistake;

    result << "<h1>Termino el examen!</h1>";
    result << "________________________________";
    result << "";
    result << "<b>Tiempo restante para acabar el examen: </b>" + QString::number(duration) + " minutos";
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

/*int ExamManager::getAmountCorrectBySection(QString sectionName)
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

}*/

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

