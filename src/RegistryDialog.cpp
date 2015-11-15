#include <QMessageBox>

#include "include/RegistryDialog.hpp"
#include "ui_RegistryDialog.h"

RegistryDialog::RegistryDialog(QWidget *parent, QString registryName,
                               QStringList registryValues) :
    QDialog(parent),
    ui(new Ui::RegistryDialog)
{
    ui->setupUi(this);
    foreach (QString registry, registryValues) {
        ui->registryOutput->append(registry);
    }

    ui->amountText->setValidator(new QIntValidator(0, 1000, this));
    if (registryValues.count() == 1) ui->separateCheckBox->setCheckable(false);

    this->registryName = registryName;

    ui->registryNameBox->setText(registryName);
}

RegistryDialog::~RegistryDialog()
{
    delete this->ui;
}

QString RegistryDialog::getRegistryName()
{
    return this->registryName;
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
        messageBox.critical(this,"Error","Ingrese el numero de preguntas");
        messageBox.setFixedSize(500, 200);

        return;
    }
    this->amount = ui->amountText->text().toInt();

    this->hide();

    registryName = ui->registryNameBox->text();
    correct = true;
    separate = ui->separateCheckBox->isChecked();
}

void RegistryDialog::on_registryDialogCancel_clicked()
{
    this->hide();
    correct = false;
}


