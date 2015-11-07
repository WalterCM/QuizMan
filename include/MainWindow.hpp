#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
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
    explicit MainWindow(QWidget *parent = 0,
                        QString accountName = "Guest",
                        QSqlDatabase database = QSqlDatabase());
    ~MainWindow();

private slots:

    void on_createCustonExamButton_clicked();

    void on_examCriteriaCB_currentIndexChanged(int index);

private:
    enum {
        ByArea,
        BySubject,
        ByTopic
    };

    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;
    ExamManager examManager;

    int criteria;
};

#endif // MAINWINDOW_HPP
