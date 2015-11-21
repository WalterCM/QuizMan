#include <QMessageBox>
#include <QDragEnterEvent>
#include <QLineEdit>
#include "include/QuestionEditor.hpp"
#include "ui_QuestionEditor.h"

QuestionEditor::QuestionEditor(QWidget *parent, QSqlDatabase db) :
    QDialog(parent),
    ui(new Ui::QuestionEditor),
    questionManager(db)
{
    ui->setupUi(this);
    this->db = db;
    updateAreas();

    ui->deleteQuestion->setEnabled(false);
    ui->editQuestion->setEnabled(false);

    imageEdit = new DroppableImageView;
    ui->imageLayout->addWidget(imageEdit);

    ui->topicEditLine->setInsertPolicy(QComboBox::NoInsert);
    ui->optionEditLine->setInsertPolicy(QComboBox::NoInsert);

    QLineEdit *lineEditTopic = new QLineEdit(this);
    ui->topicEditLine->setLineEdit(lineEditTopic);

    QLineEdit *lineEditOption = new QLineEdit(this);
    ui->optionEditLine->setLineEdit(lineEditOption);
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
    ui->setOptionCorrect->setEnabled(false);
    ui->setOptionIncorrect->setEnabled(false);
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
    int questionID = ui->questionList->currentItem()->text().split("-")[0].toInt();

    questionManager.deleteDBQuestion(questionID);

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
    for (int i = 0; i < ui->topicEdit->count(); i++) {
        if (ui->topicEdit->item(i)->text() == topicSelected)
            return;
    }

    QListWidgetItem *item = new QListWidgetItem(topicSelected);
    ui->topicEdit->addItem(item);

    updateTopicEdit();
    ui->addTopic->setEnabled(false);
}

void QuestionEditor::on_removeTopic_clicked()
{   
    int row = ui->topicEdit->currentRow();
    ui->topicEdit->takeItem(row);

    if (ui->topicEdit->count() == 0)
        ui->removeTopic->setEnabled(false);
}

void QuestionEditor::on_addOption_clicked()
{
    for (int i = 0; i < ui->optionEdit->count(); i++) {
        if (ui->optionEdit->item(i)->text() == optionSelected)
            return;
    }

    QListWidgetItem *item = new QListWidgetItem(optionSelected);
    ui->optionEdit->addItem(item);

    updateOptionEdit();
    ui->addOption->setEnabled(false);
}

void QuestionEditor::on_removeOption_clicked()
{
    int row = ui->optionEdit->currentRow();
    ui->optionEdit->takeItem(row);

    if (ui->optionEdit->count() == 0)
        ui->removeOption->setEnabled(false);
}

void QuestionEditor::on_setOptionCorrect_clicked()
{
    int row = ui->optionEdit->currentRow();
    correctOptions << row;

    QString newString = ui->optionEdit->currentItem()->text();
    newString += " (Correcta)";
    ui->optionEdit->currentItem()->setText(newString);
}

void QuestionEditor::on_setOptionIncorrect_clicked()
{
    int row = ui->optionEdit->currentRow();
    QString string = ui->optionEdit->currentItem()->text();
    if (string[string.count() - 1] == ')') {
        correctOptions.removeOne(row);
        QString newString = string.split(" (Correcta)")[0];
        ui->optionEdit->currentItem()->setText(newString);
    }
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
    QString areaName = ui->areaEdit->currentText();
    QString subjectName = ui->subjectEdit->currentText();
    QString questionDescription = ui->descriptionEdit->toPlainText();
    QString questionImageLocation = "";
    QStringList topics;
    for (int i = 0; i < ui->topicEdit->count(); i++)
        topics << ui->topicEdit->item(i)->text();

    QStringList options;
    for (int i = 0; i < ui->optionEdit->count(); i++) {
        options << ui->optionEdit->item(i)->text().split(" (Correcta)")[0];
    }

    if (areaName == "" || subjectName == "" ||
        questionDescription == "" || topics.count() == 0 ||
        options.count() == 0 || correctOptions.count() == 0) {

        QMessageBox messageBox;
        messageBox.critical(this, "Faltan datos",
                            "Solo la imagen es opcional. Los demas datos son obligatorios."
                            "Adicionalmente, al menos una opcion debe ser correcta.");
        messageBox.setFixedSize(500,200);

        return;
    }

    QMessageBox messageBox;
    int answer = messageBox.question(this, "Esta apunto de agregar una pregunta",
                        "Esta seguro que desea agregar la pregunta?",
                        "Regresar", "Agregar pregunta");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;

    QString oldPath = imageEdit->getFileName();
    if (adding) {
        if (oldPath != "") {
            QString format = oldPath.split(".")[1];
            format = "." + format;
            int num = questionManager.getDBLastID();
            QString newPath = "resources/images/question" + QString::number(num) + format;
            qDebug() << "oldpath: " << oldPath;
            qDebug() << "newpath: " << newPath;
            QFile::copy(oldPath, newPath);
            questionImageLocation = newPath;
        }

        questionManager.insertDBQuestion(areaName, subjectName,
                                         questionDescription,
                                         questionImageLocation,
                                         topics,
                                         options,
                                         correctOptions);
    } else if (editing) {
    }

    adding = false;
    editing = false;

    ui->stackedWidget->setCurrentWidget(ui->display);
    updateAll();
}

void QuestionEditor::on_areaEdit_currentTextChanged(const QString &arg1)
{
    this->areaSelected = arg1;
    if (areaSelected == "") {
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
    this->subjectSelected = arg1;
    if (subjectSelected == "") {
        ui->topicEditLine->setEnabled(false);
        return;
    }
    else {
        ui->topicEditLine->setEnabled(true);
        updateTopicEdit();
    }
}

void QuestionEditor::on_topicEditLine_editTextChanged(const QString &arg1)
{
    this->topicSelected = arg1;
    if (topicSelected == "") {
        ui->addTopic->setEnabled(false);
        return;
    }
    else {
        ui->addTopic->setEnabled(true);
    }
}

void QuestionEditor::on_optionEditLine_editTextChanged(const QString &arg1)
{
    this->optionSelected = arg1;
    if (optionSelected == "") {
        ui->addOption->setEnabled(false);
        return;
    }
    else {
        ui->addOption->setEnabled(true);
    }
}

void QuestionEditor::on_topicEdit_itemClicked(QListWidgetItem *item)
{
    ui->removeTopic->setEnabled(true);
}

void QuestionEditor::on_optionEdit_itemClicked(QListWidgetItem *item)
{
    ui->removeOption->setEnabled(true);
    ui->setOptionCorrect->setEnabled(true);
    ui->setOptionIncorrect->setEnabled(true);
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
    ui->subjectEdit->addItems(questionManager.getDBSubjects(areaSelected));

    ui->subjectEdit->setInsertPolicy(QComboBox::NoInsert);
    ui->subjectEdit->setCurrentIndex(-1);

    QLineEdit *lineEdit = new QLineEdit(this);
    ui->subjectEdit->setLineEdit(lineEdit);
}

void QuestionEditor::updateTopicEdit()
{
    ui->topicEditLine->clear();
    ui->topicEditLine->addItems(questionManager.getDBTopics(subjectSelected));

    ui->topicEditLine->setCurrentIndex(-1);
}

void QuestionEditor::updateOptionEdit()
{
    ui->optionEditLine->clear();
    //ui->topicEditLine->addItems(questionManager.getDBOptions(questionID));

    ui->optionEditLine->setCurrentIndex(-1);
}


