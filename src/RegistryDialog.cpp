#include <QMessageBox>

#include "include/RegistryDialog.hpp"
#include "ui_RegistryDialog.h"

RegistryDialog::RegistryDialog(QWidget *parent, QStringList registryList) :
    QDialog(parent),
    ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
    foreach (QString registry, registryList) {
        ui->registryOutput->append(registry);
    }

    ui->amountText->setValidator(new QIntValidator(0, 1000, this));
}

RegistryDialog::~RegistryDialog()
{
    delete this->ui;
}

int RegistryDialog::getAmountOfQuestions()
{
    return this->amount;
}

bool RegistryDialog::isCorrect()
{
    return this->correct;
}

bool RegistryDialog::isSeparated()

{
    return this->separate;
}

void RegistryDialog::on_registryDialogOk_clicked()
{
    if (ui->amountText->text() == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Ingrese el numero de preguntas");
        messageBox.setFixedSize(500,200);

        return;
    }
    this->amount = ui->amountText->text().toInt();
    this->hide();

    correct = true;
    separate = ui->separateCheckBox->isChecked();
}

void RegistryDialog::on_registryDialogCancel_clicked()
{
    this->hide();
    correct = false;
}


