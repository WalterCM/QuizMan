
#include <QMessageBox>
#include <QScrollArea>
#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"
#include "include/SectionDialog.hpp"
#include "include/SectionEdit.hpp"
#include "include/QuestionForm.hpp"
#include "include/QuestionEditor.hpp"
#include "include/IntroWindow.hpp"

MainWindow::MainWindow(QString accountName, QSqlDatabase database) :
    ui(new Ui::MainWindow),
    accountManager(AccountManager(database)),
    examManager(ExamManager(database)),
    questionManager(database)
{
    this->database = database;
    ui->setupUi(this);
    ui->summaryReport->setText(accountManager.getSummaryReport(accountName));
    ui->summaryHistory->setText(accountManager.getSummaryHistory(accountName));
    accountManager = AccountManager(database),
    examManager = ExamManager(database);
    questionManager = QuestionDBManager(database);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createCustonExamButton_clicked()
{
    examManager = ExamManager(database);

    ui->addSection->setEnabled(false);
    ui->editSection->setEnabled(false);
    ui->removeSection->setEnabled(false);
    ui->customExamSave->setEnabled(false);
    ui->customExamCreate->setEnabled(false);

    ui->areaList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->areaList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->subjectList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->subjectList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->topicList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->topicList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->stackedWidget->setCurrentWidget(ui->CustomExamCreationPage);
    ui->areaList->clear();
    ui->areaList->addItems(questionManager.getDBAreas());
    ui->areaList->sortItems();

    ui->subjectList->clear();
    ui->topicList->clear();
    ui->sectionList->clear();
    ui->examSummary->clear();

    ui->cronometerMinutes->setEnabled(false);
    ui->cronometerMinutes->setText("60");
    ui->cronometerMinutes->setAlignment(Qt::AlignCenter);
    ui->cronometerMinutes->setValidator(new QIntValidator(1, 999, this));

    QDoubleValidator *doubleValidator = new QDoubleValidator(0, 100, 2, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->positivePoints->setEnabled(false);
    ui->positivePoints->setText("0");
    ui->positivePoints->setValidator(doubleValidator);
    ui->positivePoints->setAlignment(Qt::AlignCenter);

    ui->negativePoints->setEnabled(false);
    ui->negativePoints->setText("0");
    ui->negativePoints->setValidator(doubleValidator);
    ui->negativePoints->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_custonExamBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_customExamCreate_clicked()
{
    if (ui->sectionList->count() == 0) return;
    QMessageBox messageBox;
    int answer;
    answer = messageBox.question(this,"Esta apunto de iniciar",
                        "Esta seguro que desea continuar?",
                        "Regresar", "Comenzar Examen");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;
    ui->examArea->clear();
    groups = 1;
    ui->stackedWidget->setCurrentWidget(ui->ExamPage);

    examManager.createExam();

    int t = examManager.getTime();
    hours = t / 60;
    minutes = t % 60;
    seconds = 1;

    ui->cronometer->setDigitCount(8);
    if (t > 0)
        startCronometer();
    foreach (QString sectionName, examManager.getListOfSections()) {
        QWidget *page = new QWidget();

        page->setLayout(new QGridLayout());

        QScrollArea *area = new QScrollArea();
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QWidget *contents = new QWidget();
        area->setWidget(contents);

        QVBoxLayout *layout = new QVBoxLayout;

        contents->setLayout(layout);

        area->setWidgetResizable(true);

        QMap<int, Question> questions = examManager.getQuestionsAtSection(sectionName);
        foreach (int index, questions.keys()) {
            QuestionForm *form = new QuestionForm();
            form->setQuestion(QString::number(index) + ". " +
                              questions[index].getDescription());

            QList<Option> options = questions[index].getOptions();
            foreach (Option option, options) {
                form->addAlternative(option.getDescription());
            }
            form->setImage(questions[index].getImageLocation());
            answers.insert(index, form->getGroup()) ;
            layout->addWidget(form);
        }

        page->layout()->addWidget(area);

        ui->examArea->addTab(page, sectionName);
    }

    ui->positiveDisplay->setText(ui->positivePoints->text());
    ui->negativeDisplay->setText(ui->negativePoints->text());
}

void MainWindow::on_customExamSave_clicked()
{

}

void MainWindow::on_clearSeleccion_clicked()
{
    examManager.clearExamInfo();
    ui->sectionList->clear();
    ui->content->clear();
}

void MainWindow::on_areaList_itemClicked(QListWidgetItem *item)
{
    item->setSelected(false);
    item->setSelected(true);
}

void MainWindow::on_subjectList_itemClicked(QListWidgetItem *item)
{
    item->setSelected(false);
    item->setSelected(true);
}

void MainWindow::on_topicList_itemClicked(QListWidgetItem *item)
{
    item->setSelected(false);
    item->setSelected(true);
}

void MainWindow::on_areaList_itemSelectionChanged()
{
    if (!isAreaSelected() && !isTopicSelected())
        ui->addSection->setEnabled(false);
    QModelIndexList indexes = ui->areaList->selectionModel()->selectedIndexes();

    QStringList areas;
    foreach (QModelIndex index, indexes) {
        areas.push_back(ui->areaList->item(index.row())->text());
    }
    if (areas.count() == 0) {
        clearSubjects();
        ui->subjectList->clear();
        ui->topicList->clear();
        return;
    }
    ui->addSection->setEnabled(true);

    ui->subjectList->clear();
    ui->topicList->clear();

    foreach (QString area, areas) {
        QStringList subjects = questionManager.getDBSubjects(area);
        QStringList topics = questionManager.getDBTopicsByArea(area);

        ui->subjectList->addItems(subjects);
        ui->topicList->addItems(topics);

        ui->subjectList->sortItems();
        ui->topicList->sortItems();
    }

    areaSelected = areas;
}

void MainWindow::on_subjectList_itemSelectionChanged()
{
    if (!isAreaSelected() && !isTopicSelected())
        ui->addSection->setEnabled(false);

    QModelIndexList indexes = ui->subjectList->selectionModel()->selectedIndexes();

    QStringList subjects;
    foreach (QModelIndex index, indexes) {
        subjects.push_back(ui->subjectList->item(index.row())->text());
    }

    if (subjects.count() == 0) {
        clearTopics();

        if (isAreaSelected()) {
            ui->topicList->clear();

            QStringList areas = areaSelected;
            foreach (QString area, areas) {
                QStringList topics = questionManager.getDBTopicsByArea(area);

                ui->topicList->addItems(topics);
                ui->topicList->sortItems();
            }
        }
        return;
    }

    ui->addSection->setEnabled(true);

    ui->topicList->clear();
    QStringList topics;

    foreach (QString subject, subjects) {
        topics.append(questionManager.getDBTopics(subject));
    }

    ui->topicList->addItems(topics);
    ui->topicList->sortItems();

    subjectSelected = subjects;
}

void MainWindow::on_topicList_itemSelectionChanged()
{
    if (!isAreaSelected() && !isSubjectSelected())
        ui->addSection->setEnabled(false);
    QModelIndexList indexes = ui->topicList->selectionModel()->selectedIndexes();

    QStringList topics;
    foreach (QModelIndex index, indexes) {
        topics.push_back(ui->topicList->item(index.row())->text());
    }

    if (topics.count() == 0) {
        return;
    }

    ui->addSection->setEnabled(true);

    topicSelected = topics;
}

void MainWindow::on_sectionList_itemSelectionChanged()
{
    ui->editSection->setEnabled(isSectionSelected());
    ui->removeSection->setEnabled(isSectionSelected());
}

void MainWindow::on_addSection_clicked()
{
    if (!isSomethingSelected()) return;

    QStringList selected;
    QString criteria;
    if (isTopicSelected()) {
        selected = topicSelected;
        criteria = "Topic";
    } else if (isSubjectSelected()){
        selected = subjectSelected;
        criteria = "Subject";
    } else if (isAreaSelected()) {
        selected = areaSelected;
        criteria = "Area";
    }

    QString SectionName = ("Seccion " + QString::number(groups));

    SectionDialog SectionDialog(this, SectionName, selected);
    SectionDialog.setModal(true);
    SectionDialog.setFixedSize(SectionDialog.size());
    SectionDialog.exec();

    if (!SectionDialog.isCorrect()) return;

    int amount = SectionDialog.getAmountOfQuestions();

    if (SectionDialog.isSeparated()) {
        foreach (QString item, selected) {
            QStringList SectionValues;
            SectionValues << item;
            int maxAmount = questionManager.getDBAmountQuestions(criteria, SectionValues);
            if (maxAmount < amount) {
                amountError(maxAmount);
                on_addSection_clicked();
                return;
            }
        }

        foreach (QString item, selected) {
            QStringList SectionValue;
            SectionValue.push_back(item);
            SectionName = "";
            SectionName.append("Seccion " + QString::number(groups++));

            examManager.addSection(SectionName, SectionValue, amount, criteria);

            ui->sectionList->addItem(SectionName);
        }
    } else {
        QStringList SectionValues = selected;

        SectionName = SectionDialog.getSectionName();
        int maxAmount = questionManager.getDBAmountQuestions(criteria, SectionValues);
        if (maxAmount < amount) {
            amountError(maxAmount);
            on_addSection_clicked();
            return;
        }
        groups++;

        examManager.addSection(SectionName, SectionValues, amount, criteria);

        ui->sectionList->addItem(SectionName);
    }

    ui->customExamSave->setEnabled(true);
    ui->customExamCreate->setEnabled(true);

    updateSummary();
}

void MainWindow::on_editSection_clicked()
{
    if (!isSectionSelected()) return;

    QString SectionName = ui->sectionList->currentItem()->text();
    QStringList SectionValues = examManager.getSectionValues(SectionName);
    int amount = examManager.getSectionAmount(SectionName);
    SectionEdit SectionEdit(this, SectionName, SectionValues, amount);
    SectionEdit.setModal(true);
    SectionEdit.setFixedSize(SectionEdit.size());
    SectionEdit.exec();
    QString oldName = SectionName;
    SectionName = SectionEdit.getSectionName();
    SectionValues = SectionEdit.getSectionValues();
    amount = SectionEdit.getSectionAmount();
    if (!SectionEdit.areChangesAccepted()) return;
    QString criteria = examManager.getSectionCriteria(oldName);

    int maxAmount = questionManager.getDBAmountQuestions(criteria, SectionValues);
    if (maxAmount < amount) {
        amountError(maxAmount);
        on_editSection_clicked();
        return;
    }

    examManager.editSection(oldName, SectionName, SectionValues, amount, criteria);

    ui->content->clear();
    ui->sectionList->clear();
    ui->sectionList->addItems(examManager.getSectionNames());

    updateSummary();
}

void MainWindow::on_removeSection_clicked()
{
    if (!isSectionSelected()) return;
    examManager.removeSection(ui->sectionList->currentItem()->text());
    ui->sectionList->takeItem(ui->sectionList->currentRow());
    ui->content->clear();

    updateSummary();

    if (ui->sectionList->count() == 0) {
        ui->customExamSave->setEnabled(false);
        ui->customExamCreate->setEnabled(false);
    }
}

void MainWindow::on_sectionList_itemClicked(QListWidgetItem *item)
{
    QStringList contentList = examManager.getSectionValues(item->text());

    ui->content->clear();
    QString amount = QString::number(examManager.getSectionAmount(item->text()));

    ui->content->append("<b>Cantidad: " + amount + "</b>");
    ui->content->append("");
    foreach (QString item, contentList) {
        ui->content->append(item);
    }
}

void MainWindow::on_cronometerCheckBox_clicked(bool checked)
{
    ui->cronometerMinutes->setEnabled(checked);
    examManager.setTime(ui->cronometerMinutes->text().toInt() * checked);

    if (ui->sectionList->count() == 1)
        updateSummary();
}

void MainWindow::on_cronometerMinutes_textEdited(const QString &arg1)
{
    examManager.setTime(arg1.toInt());
    updateSummary();
}

void MainWindow::on_positiveCheckBox_clicked(bool checked)
{
    ui->positivePoints->setEnabled(checked);
    examManager.setPositive(ui->positivePoints->text().toDouble() * checked);

    if (ui->sectionList->count() == 1)
        updateSummary();
}

void MainWindow::on_positivePoints_textEdited(const QString &arg1)
{
    examManager.setPositive(arg1.toDouble());
    updateSummary();
}

void MainWindow::on_negativeCheckBox_clicked(bool checked)
{
    ui->negativePoints->setEnabled(checked);
    examManager.setNegative(ui->negativePoints->text().toDouble() * checked);

    if (ui->sectionList->count() == 1)
        updateSummary();
}

void MainWindow::on_negativePoints_textEdited(const QString &arg1)
{
    examManager.setNegative(arg1.toDouble());
    updateSummary();
}

void MainWindow::on_actionQuizManEditor_triggered()
{
    QuestionEditor editor(this, database);
    editor.setModal(true);
    editor.setFixedSize(editor.size());
    editor.exec();
}

void MainWindow::on_endExam_clicked()
{
    setChosenAnswers();
    ui->stackedWidget->setCurrentWidget(ui->ResultsPage);
    ui->resultArea->clear();
    ui->resultSections->clear();
    ui->resultQuestions->clear();
    ui->resultByQuestion->clear();
    foreach (QString line, examManager.getResults()) {
        ui->resultArea->append(line);
    }

    ui->resultSections->addItems(examManager.getSectionNames());
}

void MainWindow::on_examBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->CustomExamCreationPage);
}

void MainWindow::on_resultSections_itemClicked(QListWidgetItem *item)
{
    QString sectionName = item->text();
    QList<Question> questions = examManager.getQuestionsAtSection(sectionName).values();
    foreach (Question question, questions) {
        ui->resultQuestions->addItem(question.getDescription());
    }
}

void MainWindow::on_resultQuestions_itemClicked(QListWidgetItem *item)
{
    int questionRow = ui->resultQuestions->row(item);
    ui->resultByQuestion->clear();
    foreach (QString line, examManager.getResultsByQuestion(questionRow)) {
        ui->resultByQuestion->append(line);
    }
}

void MainWindow::on_resultsBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_actionSignOut_triggered()
{
    this->hide();
    introWindow = new IntroWindow(database);
    introWindow->show();

}

void MainWindow::on_createExamButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ExamSelecionPage);
}

void MainWindow::on_actionNewExam_triggered()
{
    on_createExamButton_clicked();
}

void MainWindow::on_examSelectionBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

bool MainWindow::isSomethingSelected()
{
    return isAreaSelected() || isSubjectSelected() || isTopicSelected();
}

bool MainWindow::isAreaSelected()
{
    bool isAreaSelected = false;
    for (int i = 0; i < ui->areaList->count(); i++) {
        QListWidgetItem *item = ui->areaList->item(i);
        if (ui->areaList->isItemSelected(item)) isAreaSelected = true;
    }

    return isAreaSelected;
}

bool MainWindow::isSubjectSelected()
{
    bool isSubjectSelected = false;
    for (int i = 0; i < ui->subjectList->count(); i++) {
        QListWidgetItem *item = ui->subjectList->item(i);
        if (ui->subjectList->isItemSelected(item)) isSubjectSelected = true;
    }

    return isSubjectSelected;
}

bool MainWindow::isTopicSelected()
{
    bool isTopicSelected = false;
    for (int i = 0; i < ui->topicList->count(); i++) {
        QListWidgetItem *item = ui->topicList->item(i);
        if (ui->topicList->isItemSelected(item)) isTopicSelected = true;
    }

    return isTopicSelected;
}

bool MainWindow::isSectionSelected()
{
    bool isSectionSelected = false;
    for (int i = 0; i < ui->sectionList->count(); i++) {
        QListWidgetItem *item = ui->sectionList->item(i);
        if (ui->sectionList->isItemSelected(item)) isSectionSelected = true;
    }

    return isSectionSelected;
}

void MainWindow::clearSelections()
{
    clearAreas();
}

void MainWindow::clearAreas()
{
    for (int i = 0; i < ui->areaList->count(); i++) {
        QListWidgetItem *item = ui->areaList->item(i);
        item->setSelected(false);
    }

    clearSubjects();
    areaSelected.clear();
}

void MainWindow::clearSubjects()
{
    for (int i = 0; i < ui->subjectList->count(); i++) {
        QListWidgetItem *item = ui->subjectList->item(i);
        item->setSelected(false);
    }

    clearTopics();
    subjectSelected.clear();
}

void MainWindow::clearTopics()
{
    for (int i = 0; i < ui->topicList->count(); i++) {
        QListWidgetItem *item = ui->topicList->item(i);
        item->setSelected(false);
    }

    topicSelected.clear();
}

void MainWindow::amountError(int maxAmount)
{
    QMessageBox messageBox;
    messageBox.critical(this,"No hay suficientes preguntas",
                        "Reduzca el numero de preguntas\n"
                        "El maximo numero de preguntas es " +
                        QString::number(maxAmount));
    messageBox.setFixedSize(700,200);
}


void MainWindow::updateSummary()
{
    ui->examSummary->clear();
    foreach (QString line, examManager.getSummary()) {
        ui->examSummary->append(line);
    }
}

void MainWindow::startCronometer()
{
    timer = new QTimer(this);
    QObject::connect(timer ,  SIGNAL(timeout()),this,SLOT(updateCronometer()));

    timer->start(1000);

    updateCronometer();
}

void MainWindow::updateCronometer()
{
    QTime time;

    if (minutes == 0 && seconds == 0) {
        hours--;
        minutes = 59;
        seconds = 59;
    } else if (seconds == 0) {
        minutes--;
        seconds = 59;
    }

    seconds--;

    time.setHMS(hours, minutes, seconds, 0);
    QString text = time.toString("hh:mm:ss");
    if ((time.second() % 2) == 0) {
        text[2] = ' ';
        text[5] = ' ';
    }
    ui->cronometer->display(text);

    examManager.setTime(hours * 60 + minutes + seconds / 60);

    if (hours == 0 && minutes == 0 && seconds == 0) {
        on_endExam_clicked();
    }
}

 void MainWindow::setChosenAnswers()
 {
    QVector<QString> chosenAnswers;
    foreach (QButtonGroup *answer, answers.values()) {
        if (answer->checkedButton())
            chosenAnswers.push_back(answer->checkedButton()->text());
        else
            chosenAnswers.push_back("");
    }
    examManager.setChosenAnswers(chosenAnswers);
 }
