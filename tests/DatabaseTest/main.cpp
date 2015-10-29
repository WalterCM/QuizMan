#include <QCoreApplication>
#include <QDebug>
#include <QtSql>
#include <iostream>

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

void prompt()
{
    std::cout << "\n\t>> ";
}

int mainMenu()
{
    qDebug() << "\t\t\tMenu";
    qDebug() << "\t\t\t====";
    qDebug() << "\t1. Mostrar todas las preguntas";
    qDebug() << "\t2. Mostrar preguntas por area";
    qDebug() << "\t3. Mostrar preguntas por curso";
    qDebug() << "\t4. Mostrar preguntas por tema";
    qDebug() << "\t5. Salir";
    int op;
    prompt();
    std::cin >> op;
    return op;
}

void allQuestions()
{
    QSqlQuery query;
    query.exec("SELECT * FROM Questions");
    while (query.next()) {
        qDebug() << "\n\t" << query.value(1).toString();

        QSqlQuery subQuery;
        subQuery.prepare("SELECT OptionDescription FROM Options\
                      WHERE QuestionID = :ID");
        subQuery.bindValue(":ID", query.value(0));
        subQuery.exec();
        for (int i = 1; subQuery.next(); i++) {
            qDebug() << "\t" << i << " " << subQuery.value(0).toString();
        }
    }
}

int areaOptions()
{
    QSqlQuery query;
    query.exec("SELECT * FROM Areas");
    while (query.next()) {
        qDebug() << "\t" << query.value(0).toString() << " "
                 << query.value(1).toString();
    }
    int op;
    prompt();
    std::cin >> op;
    return op;
}

void areaQuestions()
{
    int op = areaOptions();
    QSqlQuery query;
    query.prepare("SELECT q.QuestionID, q.QuestionDescription\
                  FROM Questions AS q\
                  INNER JOIN TopicsQuestions AS tq\
                  ON tq.QuestionID = q.QuestionID\
                  INNER JOIN Topics AS t\
                  ON t.TopicID = tq.TopicID\
                  INNER JOIN Subjects AS s\
                  ON t.SubjectID = s.SubjectID\
                  WHERE s.AreaID = :ID");
    query.bindValue(":ID", QVariant(op));
    query.exec();
    while (query.next()) {
        qDebug() << "\n\t" << query.value(1).toString();
        QSqlQuery subQuery;
        subQuery.prepare("SELECT OptionDescription FROM Options\
                      WHERE QuestionID = :ID");
        subQuery.bindValue(":ID", query.value(0));
        subQuery.exec();
        for (int i = 1; subQuery.next(); i++) {
            qDebug() << "\t" << i << " " << subQuery.value(0).toString();
        }
    }
}

int subjectOptions()
{
    QSqlQuery query;
    query.exec("SELECT * FROM Subjects");
    while (query.next()) {
        qDebug() << "\t" << query.value(0).toString() << " "
                 << query.value(1).toString();
    }

    int op;
    prompt();
    std::cin >> op;
    return op;
}

void subjectQuestions()
{
    int op = subjectOptions();
    QSqlQuery query;
    query.prepare("SELECT q.QuestionID, q.QuestionDescription\
                  FROM Questions AS q\
                  INNER JOIN TopicsQuestions AS tq\
                  ON tq.QuestionID = q.QuestionID\
                  INNER JOIN Topics AS t\
                  ON t.TopicID = tq.TopicID\
                  WHERE t.SubjectID = :ID");
    query.bindValue(":ID", op);
    query.exec();
    while (query.next()) {
        qDebug() << "\n\t" << query.value(1).toString();
        QSqlQuery subQuery;
        subQuery.prepare("SELECT OptionDescription FROM Options\
                      WHERE QuestionID = :ID");
        subQuery.bindValue(":ID", query.value(0));
        subQuery.exec();
        for (int i = 1; subQuery.next(); i++) {
            qDebug() << "\t" << i << " " << subQuery.value(0).toString();
        }
    }
}


int topicOptions()
{
    QSqlQuery query;
    query.exec("SELECT * FROM Topics");
    while (query.next()) {
        qDebug() << "\n\t" << query.value(0).toString() << " "
                 << query.value(1).toString();
    }

    int op;
    prompt();
    std::cin >> op;
    return op;
}

void topicQuestions()
{
    int op = topicOptions();
    QSqlQuery query;
    query.prepare("SELECT q.QuestionID, q.QuestionDescription\
                  FROM Questions AS q\
                  INNER JOIN TopicsQuestions AS tq\
                  ON tq.QuestionID = q.QuestionID\
                  WHERE tq.TopicID = :ID");
    query.bindValue(":ID", op);
    query.exec();
    while (query.next()) {
        qDebug() << "\t" << query.value(1).toString();
        QSqlQuery subQuery;
        subQuery.prepare("SELECT OptionDescription FROM Options\
                      WHERE QuestionID = :ID");
        subQuery.bindValue(":ID", query.value(0));
        subQuery.exec();
        for (int i = 1; subQuery.next(); i++) {
            qDebug() << "\t" << i << " " << subQuery.value(0).toString();
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    db.setDatabaseName("../../resources/database/test.db");
    if (!db.open()) {
        qDebug("La base de datos no encontro");
    }

    while (1) {
        system("clear");
        int op = mainMenu();
        switch (op) {
        case 1:
            allQuestions();
            break;
        case 2:
            areaQuestions();
            break;
        case 3:
            subjectQuestions();
            break;
        case 4:
            topicQuestions();
            break;
        case 5:
            exit(0);
        default:
            qDebug() << "No existe esa opcion";
        }
        std::cin.clear();
        std::cin.ignore(255, '\n');
        std::cin.get();
    }
    db.close();
    return a.exec();
}

