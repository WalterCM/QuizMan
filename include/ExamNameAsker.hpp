#ifndef EXAMNAMEASKER_H
#define EXAMNAMEASKER_H

#include <QDialog>

namespace Ui {
class ExamNameAsker;
}

class ExamNameAsker : public QDialog
{
    Q_OBJECT

public:
    explicit ExamNameAsker(QWidget *parent = 0);
    ~ExamNameAsker();

    QString getExamName();
    bool doesSave();
private slots:
    void on_askerBack_clicked();
    void on_askerSave_clicked();
    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::ExamNameAsker *ui;

    QString examName;
    bool save;
};

#endif // EXAMNAMEASKER_H
