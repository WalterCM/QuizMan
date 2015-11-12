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
    explicit RegistryDialog(QWidget *parent = 0, QString registryName = QString(),
                            QStringList registryValues = QStringList(),
                            int maxAmount = 0);
    ~RegistryDialog();

    QString getRegistryName();
    int getAmountOfQuestions();
    bool isCorrect();
    bool isSeparated();
private slots:
    void on_registryDialogOk_clicked();

    void on_registryDialogCancel_clicked();
private:
    Ui::RegistryDialog *ui;
    QString registryName;
    int amount;

    bool correct = false;
    bool separate = false;

    int maxAmount = 0;
};

#endif // REGISTRYDIALOG_HPP
