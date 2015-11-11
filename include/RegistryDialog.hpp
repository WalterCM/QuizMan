#ifndef REGISTRYDIALOG_HPP
#define REGISTRYDIALOG_HPP

#include <QDialog>

namespace Ui {
class RegistryDialog;
}

class RegistryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistryDialog(QWidget *parent = 0, QStringList registryList = QStringList());
    ~RegistryDialog();

    int getAmountOfQuestions();
    bool isCorrect();
    bool isSeparated();
private slots:
    void on_registryDialogOk_clicked();

    void on_registryDialogCancel_clicked();
private:
    Ui::RegistryDialog *ui;
    int amount;

    bool correct = false;
    bool separate = false;
};

#endif // REGISTRYDIALOG_HPP
