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
}

RegistryDialog::~RegistryDialog()
{
    delete ui;
}

int RegistryDialog::getAmountOfQuestions()
{
    return amount;
}

void RegistryDialog::on_pushButton_clicked()
{
    this->amount = ui->amountText->text().toInt();
    this->hide();
}
