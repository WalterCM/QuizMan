#include <QMessageBox>

#include "include/SectionDialog.hpp"
#include "ui_SectionDialog.h"

SectionDialog::SectionDialog(QWidget *parent, QString SectionName,
                               QStringList SectionValues) :
    QDialog(parent),
    ui(new Ui::SectionDialog)
{
    ui->setupUi(this);
    foreach (QString Section, SectionValues) {
        ui->SectionOutput->append(Section);
    }

    ui->amountText->setValidator(new QIntValidator(0, 1000, this));
    if (SectionValues.count() == 1) ui->separateCheckBox->setCheckable(false);

    this->SectionName = SectionName;

    ui->SectionNameBox->setText(SectionName);
}

SectionDialog::~SectionDialog()
{
    delete this->ui;
}

QString SectionDialog::getSectionName()
{
    return this->SectionName;
}

int SectionDialog::getAmountOfQuestions()
{
    return this->amount;
}

bool SectionDialog::isCorrect()
{
    return this->correct;
}

bool SectionDialog::isSeparated()

{
    return this->separate;
}

void SectionDialog::on_SectionDialogOk_clicked()
{
    if (ui->amountText->text() == "") {
        QMessageBox messageBox;
        messageBox.critical(this,"Error","Ingrese el numero de preguntas");
        messageBox.setFixedSize(500, 200);

        return;
    }
    this->amount = ui->amountText->text().toInt();

    this->hide();

    SectionName = ui->SectionNameBox->text();
    correct = true;
    separate = ui->separateCheckBox->isChecked();
}

void SectionDialog::on_SectionDialogCancel_clicked()
{
    this->hide();
    correct = false;
}


