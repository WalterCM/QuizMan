#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtSql>
#include "Account.hpp"
#include "Exam.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString accountName = "Guest",
                        QSqlDatabase database = QSqlDatabase());
    ~MainWindow();

private slots:

    void on_createCustonExamButton_clicked();
    void on_custonExamBack_clicked();
    void on_customExamCreate_clicked();

    void on_clearSeleccion_clicked();
    void on_areaList_itemClicked(QListWidgetItem *item);
    void on_subjectList_itemClicked(QListWidgetItem *item);
    void on_topicList_itemClicked(QListWidgetItem *item);
    void on_areaList_itemSelectionChanged();
    void on_subjectList_itemSelectionChanged();
    void on_topicList_itemSelectionChanged();

    void on_registryList_itemSelectionChanged();
    void on_addRegistry_clicked();
    void on_editRegistry_clicked();
    void on_removeRegistry_clicked();
    void on_registryList_itemClicked(QListWidgetItem *item);

    void on_cronometerCheckBox_clicked(bool checked);

    void on_cronometerMinutes_textEdited(const QString &arg1);

    void updateCronometer();
private:
    bool isSomethingSelected();
    bool isAreaSelected();
    bool isSubjectSelected();
    bool isTopicSelected();
    bool isRegistrySelected();
    void clearSelections();
    void clearAreas();
    void clearSubjects();
    void clearTopics();

    void amountError(int maxAmount);

    void updateSummary();

    void startCronometer();

    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;
    ExamManager examManager;

    QStringList areaSelected;
    QStringList subjectSelected;
    QStringList topicSelected;

    int groups = 1;

    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    QTimer *timer;
};

#endif // MAINWINDOW_HPP
