#include <QMessageBox>
#include <QDragEnterEvent>
#include <QLineEdit>
#include "include/QuestionEditor.hpp"
#include "ui_QuestionEditor.h"

QuestionEditor::QuestionEditor(QWidget *parent, QSqlDatabase db) :
    QDialog(parent),
    ui(new Ui::QuestionEditor)
{
    ui->setupUi(this);
    questionManager = ExamManager(db);
    this->db = db;
    updateAreas();

    ui->deleteQuestion->setEnabled(false);
    ui->editQuestion->setEnabled(false);

    imageEdit = new DroppableImageView;
    ui->imageLayout->addWidget(imageEdit);
}

QuestionEditor::~QuestionEditor()
{
    delete ui;
}

void QuestionEditor::on_addQuestion_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->editor);
    updateAreaEdit();
    adding = true;

    ui->subjectEdit->setEnabled(false);
    ui->topicEditLine->setEnabled(false);

    ui->addTopic->setEnabled(false);
    ui->removeTopic->setEnabled(false);

    ui->removeOption->setEnabled(false);
}

void QuestionEditor::on_deleteQuestion_clicked()
{
    if (!isQuestionSelected()) return;
    QMessageBox messageBox;
    int answer = messageBox.question(this,"Esta apunto de eliminar una pregunta",
                        "Esta seguro que desea eliminar la pregunta?",
                        "Regresar", "Eliminar pregunta");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;

    if (!db.open()) {
        qDebug() << "Database not fonud";
        return;
    }
    int questionID;
    questionID = ui->questionList->currentItem()->text().split("-")[0].toInt();
    qDebug() << "questionID: " << questionID;
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
    qDebug() << "area: " << areaID;
    qDebug() << "curso: " << subjectID;

    query.prepare("DELETE FROM Questions\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    query.prepare("SELECT TopicID\
                   FROM TopicsQuestions\
                   WHERE QuestionID = :ID");

    query.bindValue(":ID", questionID);
    query.exec();
    QList<int> topicsID;
    while(query.next()) {
        qDebug() << "topicID: " << query.value(0).toString();
        topicsID << query.value(0).toInt();
    }

    qDebug() << "borrando opciones...";
    query.prepare("DELETE FROM Options\
                   WHERE QuestionID = :ID");
    query.bindValue(":ID", questionID);
    query.exec();

    qDebug() << "borrando topicsQuestions...";
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
        qDebug() << "count of topics: " << query.value(0).toString();
        if (query.value(0).toInt() == 0) {
            qDebug() << "borrando temas...";
            query.prepare("DELETE FROM Topics\
                           WHERE TopicID = :ID");
            query.bindValue(":ID", topicID);
            query.exec();
            query.next();
        }
    }

    query.prepare("SELECT COUNT(*)\
                   FROM Topics AS t\
                   INNER JOIN Subjects AS s\
                   ON s.SubjectID = t.SubjectID\
                   WHERE s.SubjectID = :ID");
    query.bindValue(":ID", subjectID);
    query.exec();
    query.next();
    qDebug() << "count of subjects: " << query.value(0).toString();
    if (query.value(0).toInt() == 0) {
        qDebug() << "borrando cursos...";
        query.prepare("DELETE FROM Subjects\
                       WHERE SubjectID = :ID");
        query.bindValue(":ID", subjectID);
        query.exec();
        query.next();
    }

    query.prepare("SELECT COUNT(*)\
                   FROM Subjects AS s\
                   INNER JOIN Areas AS a\
                   ON a.AreaID = s.AreaID\
                   WHERE a.AreaID = :ID");
    query.bindValue(":ID", areaID);
    query.exec();
    query.next();
    qDebug() << "count of areas: " << query.value(0).toString();
    if (query.value(0).toInt() == 0) {
        qDebug() << "borrando areas...";
        query.prepare("DELETE FROM Areas\
                       WHERE AreaID = :ID");
        query.bindValue(":ID", areaID);
        query.exec();
        query.next();
    }
    db.close();

    ui->detailsBrowser->clear();
    ui->questionList->clear();
    ui->topicList->clear();
    ui->subjectList->clear();
    ui->areaList->clear();
    updateAreas();

    ui->deleteQuestion->setEnabled(false);
    ui->editQuestion->setEnabled(false);
}

void QuestionEditor::on_editQuestion_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->editor);
    updateAreaEdit();
    editing = true;
}

void QuestionEditor::on_areaList_itemClicked(QListWidgetItem *item)
{
    updateSubjects(item->text());
}

void QuestionEditor::on_subjectList_itemClicked(QListWidgetItem *item)
{
    updateTopics(item->text());
}

void QuestionEditor::on_topicList_itemClicked(QListWidgetItem *item)
{
    updateQuestions(item->text());
}


void QuestionEditor::on_questionList_itemClicked(QListWidgetItem *item)
{
    updateDetails(item->text());
    ui->deleteQuestion->setEnabled(true);
    ui->editQuestion->setEnabled(true);
}

void QuestionEditor::on_addTopic_clicked()
{
    QListWidgetItem *item = new QListWidgetItem("Nuevo tema");
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui->topicEdit->addItem(item);
}

void QuestionEditor::on_removeTopic_clicked()
{

}

void QuestionEditor::on_addOption_clicked()
{

}

void QuestionEditor::on_removeOption_clicked()
{

}

void QuestionEditor::on_editorBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->display);
    ui->areaEdit->clear();
    ui->subjectEdit->clear();
    ui->descriptionEdit->clear();
    ui->topicEdit->clear();
    ui->optionEdit->clear();
    imageEdit->setScene(new QGraphicsScene());
}

void QuestionEditor::on_editorSave_clicked()
{
    QMessageBox messageBox;
    int answer = messageBox.question(this,"Esta apunto de agregar una pregunta",
                        "Esta seguro que desea agregar la pregunta?",
                        "Regresar", "Agregar pregunta");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;

    if (!db.open()) {
        qDebug() << "Database not found";
        return;
    }
    QSqlQuery query;

    if (adding) {
        query.prepare("");
    } else if (editing) {
        query.prepare("");
    }

    db.close();
    adding = false;
    editing = false;
}

void QuestionEditor::on_areaEdit_currentTextChanged(const QString &arg1)
{
    this->area = arg1;
    if (area == "") {
        ui->subjectEdit->setEnabled(false);
        ui->topicEditLine->setEnabled(false);
        return;
    }
    else {
        ui->subjectEdit->setEnabled(true);
        updateSubjectEdit();
    }
}

void QuestionEditor::on_subjectEdit_currentTextChanged(const QString &arg1)
{
    this->subject = arg1;
    if (subject == "") {
        ui->topicEditLine->setEnabled(false);
        return;
    }
    else {
        ui->topicEditLine->setEnabled(true);
        updateTopicEdit();
    }
}

void QuestionEditor::updateAll()
{
    updateAreas();
    updateSubjects("");
    updateTopics("");
    updateQuestions("");
    updateDetails("");
}

void QuestionEditor::updateAreas()
{
    ui->areaList->clear();
    ui->areaList->addItems(questionManager.getDBAreas());
    ui->areaLabel->setText("Areas (" +
                           QString::number(ui->areaList->count()) +
                           ")");

    ui->areaList->sortItems();

}

void QuestionEditor::updateSubjects(QString area)
{
    ui->subjectList->clear();
    ui->subjectList->addItems(questionManager.getDBSubjects(area));
    ui->subjectLabel->setText("Cursos (" +
                           QString::number(ui->subjectList->count()) +
                           ")");

    ui->subjectList->sortItems();
}

void QuestionEditor::updateTopics(QString subject)
{
    ui->topicList->clear();
    ui->topicList->addItems(questionManager.getDBTopics(subject));
    ui->topicLabel->setText("Temas (" +
                           QString::number(ui->topicList->count()) +
                           ")");

    ui->topicList->sortItems();
}

void QuestionEditor::updateQuestions(QString topic)
{
    ui->questionList->clear();
    QHash<int, QString> questions;
    questions = questionManager.getDBQuestionHashByTopic(topic);
    foreach (int id, questions.keys())
        ui->questionList->addItem(QString::number(id) + "- " +
                                  questions[id]);

    ui->questionLabel->setText("Preguntas (" +
                           QString::number(ui->questionList->count()) +
                           ")");

    ui->questionList->sortItems();
}

void QuestionEditor::updateDetails(QString question)
{
    ui->detailsBrowser->clear();
    int id = question.split("-")[0].toInt();

    ui->detailsBrowser->clear();
    foreach (QString line, questionManager.getDBDetailsOfQuestion(id))
        ui->detailsBrowser->append(line);

}

bool QuestionEditor::isQuestionSelected()
{
    for (int i = 0; i < ui->questionList->count(); i++)
        if (ui->questionList->item(i)->isSelected())
            return true;

    return false;
}

void QuestionEditor::updateAreaEdit()
{
    ui->areaEdit->clear();
    ui->areaEdit->addItems(questionManager.getDBAreas());

    ui->areaEdit->setInsertPolicy(QComboBox::NoInsert);
    ui->areaEdit->setCurrentIndex(-1);

    QLineEdit *lineEdit = new QLineEdit(this);
    ui->areaEdit->setLineEdit(lineEdit);
}

void QuestionEditor::updateSubjectEdit()
{
    ui->subjectEdit->clear();
    ui->subjectEdit->addItems(questionManager.getDBSubjects(area));

    ui->subjectEdit->setInsertPolicy(QComboBox::NoInsert);
    ui->subjectEdit->setCurrentIndex(-1);

    QLineEdit *lineEdit = new QLineEdit(this);
    ui->subjectEdit->setLineEdit(lineEdit);
}

void QuestionEditor::updateTopicEdit()
{
    ui->topicEditLine->clear();
    ui->topicEditLine->addItems(questionManager.getDBTopics(subject));

    ui->topicEditLine->setInsertPolicy(QComboBox::NoInsert);
    ui->topicEditLine->setCurrentIndex(-1);

    QLineEdit *lineEdit = new QLineEdit(this);
    ui->topicEditLine->setLineEdit(lineEdit);
}

void QuestionEditor::updateOptionEdit()
{

}


