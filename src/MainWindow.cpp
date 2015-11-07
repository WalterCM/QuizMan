
#include "ui_MainWindow.h"
#include "include/MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent, QString accountName, QSqlDatabase database) :
    QMainWindow(parent),
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
    ui->stackedWidget->setCurrentWidget(ui->CustomExamCreationPage);
    ui->examStackedWidget->setCurrentWidget(ui->ByArea);
    QStringList areaList = examManager.getDBAreas();
    ui->areaTree->setColumnCount(1);
    ui->areaTree->setHeaderLabel("Areas");
    ui->areaTree->setSelectionMode(QAbstractItemView::NoSelection);

    foreach (QString area, areaList) {
        QTreeWidgetItem *areaItem = new QTreeWidgetItem(ui->areaTree);
        areaItem->setText(0, area);
    }

    QHash<QString, QStringList> subjectList = examManager.getDBSubjectTree();
    ui->subjectTee->setColumnCount(1);
    ui->subjectTee->setHeaderLabel("Cursos");
    ui->subjectTee->setSelectionMode(QAbstractItemView::NoSelection);

    foreach (QString area, subjectList.keys()) {
        QTreeWidgetItem *areaItem = new QTreeWidgetItem(ui->subjectTee);
        areaItem->setText(0, area);
        foreach (QString subject, subjectList[area]) {
            QTreeWidgetItem *subjectItem = new QTreeWidgetItem();
            subjectItem->setText(0, subject);
            areaItem->addChild(subjectItem);
        }
    }

    QHash<QString, QHash<QString, QStringList> > topicList =examManager.getDBTopicTree();
    ui->topicTree->setColumnCount(1);
    ui->topicTree->setHeaderLabel("Temas");
    ui->topicTree->setSelectionMode(QAbstractItemView::NoSelection);

    foreach (QString area, topicList.keys()) {
        QTreeWidgetItem *areaItem = new QTreeWidgetItem(ui->topicTree);
        areaItem->setText(0, area);
        foreach (QString subject, topicList[area].keys()) {
            QTreeWidgetItem *subjectItem = new QTreeWidgetItem();
            subjectItem->setText(0, subject);
            areaItem->addChild(subjectItem);
            foreach(QString topic, topicList[area][subject]) {
                QTreeWidgetItem *topicItem = new QTreeWidgetItem();
                topicItem->setText(0, topic);
                subjectItem->addChild(topicItem);
            }
        }
    }
}


void MainWindow::on_examCriteriaCB_currentIndexChanged(int index)
{
    criteria = index;

    switch (index) {
    case ByArea:
        ui->examStackedWidget->setCurrentWidget(ui->ByArea);
        break;
    case BySubject:
        ui->examStackedWidget->setCurrentWidget(ui->BySubject);
        break;
    case ByTopic:
        ui->examStackedWidget->setCurrentWidget(ui->ByTopic);
        break;
    }
}
