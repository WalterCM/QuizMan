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
private slots:
    void on_pushButton_clicked();

private:
    Ui::RegistryDialog *ui;
    int amount;
};

#endif // REGISTRYDIALOG_HPP
