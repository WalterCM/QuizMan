
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
    ui->stackedWidget->setCurrentWidget(ui->ExamPage);
}

void MainWindow::on_clearSeleccion_clicked()
{
    examManager.clearExamInfo();
    ui->registryList->clear();
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
    QModelIndexList indexes = ui->areaList->selectionModel()->selectedIndexes();

    QStringList areas;
    foreach (QModelIndex index, indexes) {
        areas.push_back(ui->areaList->item(index.row())->text());
    }
    if (areas.count() == 0) {

        areaSelected = false;
        return;
    }

    areaSelected = true;

    ui->subjectList->clear();
    ui->topicList->clear();

    foreach (QString area, areas) {
        QStringList subjects = examManager.getDBSubjects(area);
        QStringList topics = examManager.getDBTopicsPerArea(area);

        ui->subjectList->addItems(subjects);
        ui->topicList->addItems(topics);

        ui->subjectList->sortItems();
        ui->topicList->sortItems();
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

    if (subjects.count() == 0) {
        subjectSelected = false;
        return;
    }

    subjectSelected = true;

    ui->topicList->clear();
    QStringList topics;

    foreach (QString subject, subjects) {
        topics.append(examManager.getDBTopics(subject));
    }

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

    if (topics.count() == 0) return;

    selected = topics;
}


void MainWindow::on_addRegistry_clicked()
{
    if (!isSomethingSelected()) return;

    RegistryDialog registryDialog(this, selected);
    registryDialog.setModal(true);
    registryDialog.setFixedSize(registryDialog.size());
    registryDialog.exec();

    if (!registryDialog.isCorrect()) return;

    if (registryDialog.isSeparated()) {

    } else {
        QStringList registryValue = selected;
        QString registryName;
        int amount = registryDialog.getAmountOfQuestions();
        if (selected.count() == 1)
            registryName = selected.at(0);
        else
            registryName.append("Grupo " + QString::number(groups++));

        if (isTopicSelected()) registryValue.push_front("Topic");
        else if (isSubjectSelected()) registryValue.push_front("Subject");
        else if (isAreaSelected()) registryValue.push_front("Area");

        examManager.addRegister(registryName, registryValue, amount);

        ui->registryList->addItem(registryName);
    }
}

void MainWindow::on_registryList_itemClicked(QListWidgetItem *item)
{
    QStringList contentList = examManager.getRegisterValue(item->text());
    contentList.pop_front();

    ui->content->clear();

    foreach (QString item, contentList) {
        ui->content->append(item);
    }
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

void MainWindow::on_editRegistry_clicked()
{

}

void MainWindow::on_removeRegistry_clicked()
{

}

void MainWindow::clearSelections()
{
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

    selected.clear();
}




