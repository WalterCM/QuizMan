#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtSql>
#include "Account.hpp"
#include "Exam.hpp"
#include "RegistryDialog.hpp"

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
    void on_addRegistry_clicked();
    void on_editRegistry_clicked();
    void on_removeRegistry_clicked();
    void on_registryList_itemClicked(QListWidgetItem *item);
private:
    bool isSomethingSelected();
    bool isAreaSelected();
    bool isSubjectSelected();
    bool isTopicSelected();
    void clearSelections();

    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;
    ExamManager examManager;

    QString areaSaved = "";
    QStringList selected;

    bool areaSelected = false;
    bool subjectSelected = false;
    bool topicSelected = false;

    int groups = 1;
};

#endif // MAINWINDOW_HPP
