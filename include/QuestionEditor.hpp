#ifndef QUESTIONEDITOR_HPP
#define QUESTIONEDITOR_HPP

#include <QDialog>
#include <QtSql>
#include <QListWidgetItem>
#include <include/Exam/ExamManager.hpp>
#include "include/DroppableImageView.hpp"

namespace Ui {
class QuestionEditor;
}

class QuestionEditor : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionEditor(QWidget *parent = 0,
                            QSqlDatabase db = QSqlDatabase());
    ~QuestionEditor();

private slots:
    void on_addQuestion_clicked();
    void on_deleteQuestion_clicked();
    void on_editQuestion_clicked();

    void on_areaList_itemClicked(QListWidgetItem *item);
    void on_subjectList_itemClicked(QListWidgetItem *item);
    void on_topicList_itemClicked(QListWidgetItem *item);
    void on_questionList_itemClicked(QListWidgetItem *item);


    void on_addTopic_clicked();
    void on_removeTopic_clicked();
    void on_addOption_clicked();
    void on_removeOption_clicked();
    void on_editorBack_clicked();
    void on_editorSave_clicked();
    void on_areaEdit_currentTextChanged(const QString &arg1);
    void on_subjectEdit_currentTextChanged(const QString &arg1);

private:
    void updateAll();
    void updateAreas();
    void updateSubjects(QString area);
    void updateTopics(QString subject);
    void updateQuestions(QString topic);
    void updateDetails(QString question);

    bool isQuestionSelected();

    void updateAreaEdit();
    void updateSubjectEdit();
    void updateTopicEdit();
    void updateOptionEdit();

    Ui::QuestionEditor *ui;
    QSqlDatabase db;

    ExamManager questionManager;

    bool adding = false;
    bool editing = false;

    QString area;
    QString subject;

    DroppableImageView *imageEdit;
};

#endif // QUESTIONEDITOR_HPP
