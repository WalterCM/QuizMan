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

    void on_areaList_itemSelectionChanged();

    void on_subjectList_itemSelectionChanged();

    void on_topicList_itemSelectionChanged();

    void on_addRegistry_clicked();
private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;
    ExamManager examManager;

    QString areaSaved = "";
    QStringList selected;
    bool addingRegistry = false;

    int groups = 1;
};

#endif // MAINWINDOW_HPP
