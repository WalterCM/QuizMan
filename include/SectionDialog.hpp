#ifndef SectionDIALOG_HPP
#define SectionDIALOG_HPP

#include <QDialog>

namespace Ui {
class SectionDialog;
}

class SectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SectionDialog(QWidget *parent = 0, QString SectionName = QString(),
                            QStringList SectionValues = QStringList());
    ~SectionDialog();

    QString getSectionName();
    int getAmountOfQuestions();
    bool isCorrect();
    bool isSeparated();
private slots:
    void on_SectionDialogOk_clicked();

    void on_SectionDialogCancel_clicked();
private:
    Ui::SectionDialog *ui;
    QString SectionName;
    int amount;

    bool correct = false;
    bool separate = false;
};

#endif // SectionDIALOG_HPP
