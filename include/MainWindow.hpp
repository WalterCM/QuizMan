#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtSql>
#include <QButtonGroup>
#include <QRadioButton>
#include "Account.hpp"
#include "Exam.hpp"

class IntroWindow;

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
signals:
    void alternativeSelected(QWidget *alternative);
private slots:
    void on_createExamButton_clicked();
    void on_createSavedExamButton_clicked();
    void on_createCustonExamButton_clicked();

    void on_examSelectionBack_clicked();
    void on_examSelectionStart_clicked();

    void on_custonExamBack_clicked();
    void on_customExamSave_clicked();
    void on_customExamCreate_clicked();

    void on_clearSeleccion_clicked();
    void on_areaList_itemClicked(QListWidgetItem *item);
    void on_subjectList_itemClicked(QListWidgetItem *item);
    void on_topicList_itemClicked(QListWidgetItem *item);
    void on_areaList_itemSelectionChanged();
    void on_subjectList_itemSelectionChanged();
    void on_topicList_itemSelectionChanged();

    void on_sectionList_itemSelectionChanged();
    void on_addSection_clicked();
    void on_editSection_clicked();
    void on_removeSection_clicked();
    void on_sectionList_itemClicked(QListWidgetItem *item);

    void on_cronometerCheckBox_clicked(bool checked);
    void on_cronometerMinutes_textEdited(const QString &arg1);
    void updateCronometer();

    void on_positiveCheckBox_clicked(bool checked);
    void on_positivePoints_textEdited(const QString &arg1);

    void on_negativeCheckBox_clicked(bool checked);
    void on_negativePoints_textEdited(const QString &arg1);

    void on_actionQuizManEditor_triggered();

    void on_endExam_clicked();
    void on_examBack_clicked();

    void on_resultSections_itemClicked(QListWidgetItem *item);
    void on_resultQuestions_itemClicked(QListWidgetItem *item);

    void on_resultsBack_clicked();
    void on_actionSignOut_triggered();

    void on_actionNewExam_triggered();

    void on_examList_itemClicked(QListWidgetItem *item);
    void on_sectionList_examSelection_itemClicked(QListWidgetItem *item);
private:
    bool isSomethingSelected();
    bool isAreaSelected();
    bool isSubjectSelected();
    bool isTopicSelected();
    bool isSectionSelected();
    void clearSelections();
    void clearAreas();
    void clearSubjects();
    void clearTopics();

    void amountError(int maxAmount);

    void updateSummary();
    void startCronometer();

    void setChosenAnswers();

    Ui::MainWindow *ui;
    QSqlDatabase database;
    AccountManager accountManager;
    ExamManager examManager;
    QuestionDBManager questionManager;

    QStringList areaSelected;
    QStringList subjectSelected;
    QStringList topicSelected;

    int groups = 1;

    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    QTimer *timer;

    QMap<int, QButtonGroup *> answers;

    IntroWindow *introWindow;
};

#endif // MAINWINDOW_HPP
