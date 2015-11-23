#include <QDebug>
#include "include/SectionEdit.hpp"
#include "ui_SectionEdit.h"

SectionEdit::SectionEdit(QWidget *parent, QString SectionName, QStringList SectionValues, int amount) :
    QDialog(parent),
    ui(new Ui::SectionEdit)
{
    ui->setupUi(this);

    ui->SectionNameEdit->setText(SectionName);

    foreach (QString value, SectionValues) {
        QListWidgetItem *item = new QListWidgetItem(value);
        ui->SectionValuesEdit->addItem(item);
    }

    ui->amountEdit->setText(QString::number(amount));
    ui->amountEdit->setAlignment(Qt::AlignCenter);
    ui->amountEdit->setValidator(new QIntValidator(0, 1000, this));

    ui->moveUpButton->setEnabled(false);
    ui->moveDownButton->setEnabled(false);
    ui->removeItemButton->setEnabled(false);
}

SectionEdit::~SectionEdit()
{
    delete ui;
}

QString SectionEdit::getSectionName()
{
    return ui->SectionNameEdit->text();
}

QStringList SectionEdit::getSectionValues()
{
    QStringList SectionValues;
    for (int i = 0; i < ui->SectionValuesEdit->count(); i++) {
        SectionValues << ui->SectionValuesEdit->item(i)->text();
    }

    return SectionValues;
}

int SectionEdit::getSectionAmount()
{
    return ui->amountEdit->text().toInt();
}

bool SectionEdit::areChangesAccepted()
{
    return this->changesAccepted;
}

void SectionEdit::on_SectionValuesEdit_itemSelectionChanged()
{
    bool isSelected = isValueSelected();
    ui->moveUpButton->setEnabled(isSelected);
    ui->moveDownButton->setEnabled(isSelected);
    ui->removeItemButton->setEnabled(isSelected);

    int row = ui->SectionValuesEdit->currentRow();
    int count = ui->SectionValuesEdit->count();
    if (row == 0)
        ui->moveUpButton->setEnabled(false);
    if (row == count - 1)
        ui->moveDownButton->setEnabled(false);

    if (count == 1)
        ui->removeItemButton->setEnabled(false);
}

void SectionEdit::on_moveUpButton_clicked()
{
    if (!isValueSelected()) return;

    int row = ui->SectionValuesEdit->currentRow();

    if (!(row > 0)) return;

    QListWidgetItem * item = ui->SectionValuesEdit->takeItem(row);

    ui->SectionValuesEdit->insertItem(row - 1, item);
    ui->SectionValuesEdit->setCurrentRow(row - 1);
}

void SectionEdit::on_moveDownButton_clicked()
{
    if (!isValueSelected()) return;

    int row = ui->SectionValuesEdit->currentRow();

    if (!(row < ui->SectionValuesEdit->count())) return;

    QListWidgetItem * item = ui->SectionValuesEdit->takeItem(row);

    ui->SectionValuesEdit->insertItem(row + 1, item);
    ui->SectionValuesEdit->setCurrentRow(row + 1);
}

void SectionEdit::on_removeItemButton_clicked()
{
    if (ui->SectionValuesEdit->count() > 1)
        ui->SectionValuesEdit->takeItem(ui->SectionValuesEdit->currentRow());

    on_SectionValuesEdit_itemSelectionChanged();
}

void SectionEdit::on_okButton_clicked()
{
    changesAccepted = true;
    this->hide();
}

void SectionEdit::on_cancelButton_clicked()
{
    changesAccepted = false;
    this->hide();
}

bool SectionEdit::isValueSelected()
{
    bool isValueSelected = false;
    for (int i = 0; i < ui->SectionValuesEdit->count(); i++) {
        if (ui->SectionValuesEdit->item(i)->isSelected())
            isValueSelected = true;
    }

    return isValueSelected;
}


