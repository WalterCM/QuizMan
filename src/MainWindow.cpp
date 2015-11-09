
#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"

MainWindow::MainWindow(QString accountName, QSqlDatabase database) :
    ui(new Ui::MainWindow),
    accountManager(AccountManager(database)),
    examManager(ExamManager(database))
{
    this->database = database;
    ui->setupUi(this);
    ui->summaryReport->setText(accountManager.getSummaryReport(accountName));
    ui->summaryHistory->setText(accountManager.getSummaryHistory(accountName));
    accountManager = AccountManager(database),
    examManager = ExamManager(database);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createCustonExamButton_clicked()
{
    ui->areaList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->areaList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->subjectList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->subjectList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->topicList->setDragDropMode(QAbstractItemView::DropOnly);
    ui->topicList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->stackedWidget->setCurrentWidget(ui->CustomExamCreationPage);
    ui->areaList->clear();
    QStringList areas = examManager.getDBAreas();
    ui->areaList->addItems(areas);
}

void MainWindow::on_custonExamBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_customExamCreate_clicked()
{

}

void MainWindow::on_clearSeleccion_clicked()
{
    examManager.clearExamInfo();
}

void MainWindow::on_areaList_itemSelectionChanged()
{
    QModelIndexList indexes = ui->areaList->selectionModel()->selectedIndexes();

    QStringList areas;
    foreach (QModelIndex index, indexes) {
        areas.push_back(ui->areaList->item(index.row())->text());
    }
    if (areas.count() == 0 || addingRegistry) return;
    ui->subjectList->clear();
    ui->topicList->clear();

    if (areas.count() == 1) {
        areaSaved = areas.at(0);
        QStringList subjects = examManager.getDBSubjects(areaSaved);
        QStringList topics = examManager.getDBTopicsPerArea(areaSaved);

        ui->subjectList->addItems(subjects);
        ui->topicList->addItems(topics);
    }

    if (areas.count() > 1) {
        foreach (QString area, areas) {
            QStringList subjects = examManager.getDBSubjects(area);
            QStringList topics = examManager.getDBTopicsPerArea(area);

            ui->subjectList->addItems(subjects);
            ui->topicList->addItems(topics);
        }
    }

    selected = areas;
}

void MainWindow::on_subjectList_itemSelectionChanged()
{
    QModelIndexList indexes = ui->subjectList->selectionModel()->selectedIndexes();

    QStringList subjects;
    foreach (QModelIndex index, indexes) {
        subjects.push_back(ui->subjectList->item(index.row())->text());
    }

    if (subjects.count() == 0 || addingRegistry) return;

    ui->topicList->clear();
    QStringList areas = examManager.getDBAreas();
    QStringList topics;

    if (subjects.count() == 1) {
        topics.append(examManager.getDBTopics(subjects.at(0)));
    }

    if (subjects.count() > 1) {
        foreach (QString subject, subjects) {
            topics.append(examManager.getDBTopics(subject));
        }
    }

    ui->areaList->clear();
    ui->areaList->addItems(areas);


    ui->topicList->addItems(topics);
    ui->topicList->sortItems();

    selected = subjects;
}

void MainWindow::on_topicList_itemSelectionChanged()
{
    QModelIndexList indexes = ui->topicList->selectionModel()->selectedIndexes();

    QStringList topics;
    foreach (QModelIndex index, indexes) {
        topics.push_back(ui->topicList->item(index.row())->text());
    }

    if (topics.count() == 0 || addingRegistry) return;

    if (topics.count() == 1) {
        QStringList areas = examManager.getDBAreas();
        QStringList subjects = examManager.getDBSubjects(areaSaved);

        ui->areaList->clear();
        ui->subjectList->clear();

        ui->areaList->addItems(areas);
        ui->subjectList->addItems(subjects);
    }

    selected = topics;
}


void MainWindow::on_addRegistry_clicked()
{
    addingRegistry = true;

    bool isAreaSelected = false;
    for (int i = 0; i < ui->areaList->count(); i++) {
        QListWidgetItem *item = ui->areaList->item(i);
        if (ui->areaList->isItemSelected(item)) isAreaSelected = true;
    }

    bool isSubjectSelected = false;
    for (int i = 0; i < ui->subjectList->count(); i++) {
        QListWidgetItem *item = ui->subjectList->item(i);
        if (ui->subjectList->isItemSelected(item)) isSubjectSelected = true;
    }

    bool isTopicSelected = false;
    for (int i = 0; i < ui->topicList->count(); i++) {
        QListWidgetItem *item = ui->topicList->item(i);
        if (ui->topicList->isItemSelected(item)) isTopicSelected = true;
    }

    if (!(isAreaSelected || isSubjectSelected || isTopicSelected)) {
        return;
    }

    RegistryDialog registryDialog(this, selected);
    registryDialog.setModal(true);
    registryDialog.setFixedSize(registryDialog.size());
    registryDialog.exec();

    QString registryName;
    int amount = registryDialog.getAmountOfQuestions();
    if (selected.count() == 1)
        registryName = selected.at(0);
    else
        registryName.append("Grupo " + QString::number(groups++));

    if (isAreaSelected) selected.push_front("Area");
    if (isSubjectSelected) selected.push_front("Subject");
    if (isTopicSelected) selected.push_front("Topic");

    examManager.addRegister(registryName, selected, amount);

    for (int i = 0; i < ui->areaList->count(); i++) {
        QListWidgetItem *item = ui->areaList->item(i);
        item->setSelected(false);
    }

    for (int i = 0; i < ui->subjectList->count(); i++) {
        QListWidgetItem *item = ui->subjectList->item(i);
        item->setSelected(false);
    }

    for (int i = 0; i < ui->topicList->count(); i++) {
        QListWidgetItem *item = ui->topicList->item(i);
        item->setSelected(false);
    }

    ui->registryList->clear();
    ui->registryList->addItems(examManager.getRegisterNames());

    addingRegistry = false;
}



