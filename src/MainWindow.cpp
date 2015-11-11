
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"
#include "include/RegistryDialog.hpp"
#include "include/RegistryEdit.hpp"

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
    ui->addRegistry->setEnabled(false);
    ui->editRegistry->setEnabled(false);
    ui->removeRegistry->setEnabled(false);

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

    ui->cronometerMinutes->setEnabled(false);
    ui->cronometerMinutes->setText("60");
    ui->cronometerMinutes->setAlignment(Qt::AlignCenter);
    ui->cronometerMinutes->setValidator(new QIntValidator(1, 999, this));
}

void MainWindow::on_custonExamBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_customExamCreate_clicked()
{
    if (ui->registryList->count() == 0) return;
    QMessageBox messageBox;
    int answer;
    answer = messageBox.question(this,"Esta apunto de iniciar",
                        "Esta seguro que desea continuar?",
                        "Regresar", "Comenzar Examen");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;

    ui->stackedWidget->setCurrentWidget(ui->ExamPage);

    examManager.createExam();

    int t = examManager.getTime();
    hours = t / 60;
    minutes = t % 60;
    seconds = 1;
    qDebug() << "h: " << hours << " m: " << minutes << " s: " << seconds;

    ui->cronometer->setDigitCount(8);
    startCronometer();
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
    if (!isAreaSelected() && !isTopicSelected())
        ui->addRegistry->setEnabled(false);
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
    ui->addRegistry->setEnabled(true);

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

    areaSelected = areas;
}

void MainWindow::on_subjectList_itemSelectionChanged()
{
    if (!isAreaSelected() && !isTopicSelected())
        ui->addRegistry->setEnabled(false);

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
                QStringList topics = examManager.getDBTopicsPerArea(area);

                ui->topicList->addItems(topics);
                ui->topicList->sortItems();
            }
        }
        return;
    }

    ui->addRegistry->setEnabled(true);

    ui->topicList->clear();
    QStringList topics;

    foreach (QString subject, subjects) {
        topics.append(examManager.getDBTopics(subject));
    }

    ui->topicList->addItems(topics);
    ui->topicList->sortItems();

    subjectSelected = subjects;
}

void MainWindow::on_topicList_itemSelectionChanged()
{
    if (!isAreaSelected() && !isSubjectSelected())
        ui->addRegistry->setEnabled(false);
    QModelIndexList indexes = ui->topicList->selectionModel()->selectedIndexes();

    QStringList topics;
    foreach (QModelIndex index, indexes) {
        topics.push_back(ui->topicList->item(index.row())->text());
    }

    if (topics.count() == 0) {
        return;
    }

    ui->addRegistry->setEnabled(true);

    topicSelected = topics;
}

void MainWindow::on_registryList_itemSelectionChanged()
{
    if (ui->registryList->count() == 0) {
        ui->editRegistry->setEnabled(false);
        ui->removeRegistry->setEnabled(false);
    } else {
        ui->editRegistry->setEnabled(true);
        ui->removeRegistry->setEnabled(true);
    }
}

void MainWindow::on_addRegistry_clicked()
{
    if (!isSomethingSelected()) return;

    QStringList selected;
    if (isTopicSelected())
        selected = topicSelected;
    else if (isSubjectSelected())
        selected = subjectSelected;
    else if (isAreaSelected())
        selected = areaSelected;

    RegistryDialog registryDialog(this, selected);
    registryDialog.setModal(true);
    registryDialog.setFixedSize(registryDialog.size());
    registryDialog.exec();

    if (!registryDialog.isCorrect()) return;

    if (registryDialog.isSeparated()) {
        int amount = registryDialog.getAmountOfQuestions();
        foreach (QString item, selected) {
            QStringList registryValue;
            registryValue.push_back(item);
            QString registryName = item;
            if (isTopicSelected()) registryValue.push_front("Topic");
            else if (isSubjectSelected()) registryValue.push_front("Subject");
            else if (isAreaSelected()) registryValue.push_front("Area");

            examManager.addRegistry(registryName, registryValue, amount);

            ui->registryList->addItem(registryName);
        }
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

        examManager.addRegistry(registryName, registryValue, amount);

        ui->registryList->addItem(registryName);
    }

    updateSummary();
}

void MainWindow::on_editRegistry_clicked()
{
    if (!isRegistrySelected()) return;

    QString registryName = ui->registryList->currentItem()->text();
    QStringList registryValues = examManager.getRegistryValues(registryName);
    int amount = examManager.getRegistryAmount(registryName);
    RegistryEdit registryEdit(this, registryName, registryValues, amount);
    registryEdit.setModal(true);
    registryEdit.setFixedSize(registryEdit.size());
    registryEdit.exec();

    if (!registryEdit.areChangesAccepted()) return;
    QString oldName = registryName;
    registryName = registryEdit.getRegistryName();
    registryValues = registryEdit.getRegistryValues();
    amount = registryEdit.getRegistryAmount();

    examManager.editRegistry(oldName, registryName, registryValues, amount);

    ui->content->clear();
    ui->registryList->clear();
    ui->registryList->addItems(examManager.getRegistryNames());

    updateSummary();
}

void MainWindow::on_removeRegistry_clicked()
{
    if (!isRegistrySelected()) return;
    examManager.removeRegistry(ui->registryList->currentItem()->text());
    ui->registryList->takeItem(ui->registryList->currentRow());
    ui->content->clear();

    updateSummary();
}

void MainWindow::on_registryList_itemClicked(QListWidgetItem *item)
{
    QStringList contentList = examManager.getRegistryValues(item->text());
    contentList.pop_front();

    ui->content->clear();
    QString amount = QString::number(examManager.getRegistryAmount(item->text()));

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

    updateSummary();
}

void MainWindow::on_cronometerMinutes_textEdited(const QString &arg1)
{
    examManager.setTime(arg1.toInt());

    updateSummary();
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

bool MainWindow::isRegistrySelected()
{
    bool isRegistrySelected = false;
    for (int i = 0; i < ui->registryList->count(); i++) {
        QListWidgetItem *item = ui->registryList->item(i);
        if (ui->registryList->isItemSelected(item)) isRegistrySelected = true;
    }

    return isRegistrySelected;
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
    QObject::connect(timer ,  SIGNAL(timeout ()),this,SLOT(updateCronometer()));

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

    qDebug() << "hours: " << hours << " minutes: " << minutes << " seconds: " << seconds;
    time.setHMS(hours, minutes, seconds, 0);
    QString text = time.toString("hh:mm:ss");
    qDebug() << "time.hour: " << time.hour();
    qDebug() << "time.minute: " << time.minute();
    qDebug() << "time.second: " << time.second();
    if ((time.second() % 2) == 0) {
        text[2] = ' ';
        text[5] = ' ';
    }
    ui->cronometer->display(text);
}
