#include <QDebug>
#include "include/RegistryEdit.hpp"
#include "ui_RegistryEdit.h"

RegistryEdit::RegistryEdit(QWidget *parent, QString registryName, QStringList registryValues, int amount) :
    QDialog(parent),
    ui(new Ui::RegistryEdit)
{
    ui->setupUi(this);

    ui->registryNameEdit->setText(registryName);

    foreach (QString value, registryValues) {
        QListWidgetItem *item = new QListWidgetItem(value);
        ui->registryValuesEdit->addItem(item);
    }

    ui->amountEdit->setText(QString::number(amount));
    ui->amountEdit->setAlignment(Qt::AlignCenter);
    ui->amountEdit->setValidator(new QIntValidator(0, 1000, this));

    ui->moveUpButton->setEnabled(false);
    ui->moveDownButton->setEnabled(false);
    ui->removeItemButton->setEnabled(false);
}

RegistryEdit::~RegistryEdit()
{
    delete ui;
}

QString RegistryEdit::getRegistryName()
{
    return ui->registryNameEdit->text();
}

QStringList RegistryEdit::getRegistryValues()
{
    QStringList registryValues;
    for (int i = 0; i < ui->registryValuesEdit->count(); i++) {
        registryValues << ui->registryValuesEdit->item(i)->text();
    }

    return registryValues;
}

int RegistryEdit::getRegistryAmount()
{
    return ui->amountEdit->text().toInt();
}

bool RegistryEdit::areChangesAccepted()
{
    return this->changesAccepted;
}

void RegistryEdit::on_registryValuesEdit_itemSelectionChanged()
{
    bool isSelected = isValueSelected();
    ui->moveUpButton->setEnabled(isSelected);
    ui->moveDownButton->setEnabled(isSelected);
    ui->removeItemButton->setEnabled(isSelected);

    int row = ui->registryValuesEdit->currentRow();
    int count = ui->registryValuesEdit->count();
    if (row == 0)
        ui->moveUpButton->setEnabled(false);
    if (row == count - 1)
        ui->moveDownButton->setEnabled(false);

    if (count == 1)
        ui->removeItemButton->setEnabled(false);
}

void RegistryEdit::on_moveUpButton_clicked()
{
    if (!isValueSelected()) return;

    int row = ui->registryValuesEdit->currentRow();

    if (!(row > 0)) return;

    QListWidgetItem * item = ui->registryValuesEdit->takeItem(row);

    ui->registryValuesEdit->insertItem(row - 1, item);
    ui->registryValuesEdit->setCurrentRow(row - 1);
}

void RegistryEdit::on_moveDownButton_clicked()
{
    if (!isValueSelected()) return;

    int row = ui->registryValuesEdit->currentRow();

    if (!(row < ui->registryValuesEdit->count())) return;

    QListWidgetItem * item = ui->registryValuesEdit->takeItem(row);

    ui->registryValuesEdit->insertItem(row + 1, item);
    ui->registryValuesEdit->setCurrentRow(row + 1);
}

void RegistryEdit::on_removeItemButton_clicked()
{
    if (ui->registryValuesEdit->count() > 1)
        ui->registryValuesEdit->takeItem(ui->registryValuesEdit->currentRow());

    on_registryValuesEdit_itemSelectionChanged();
}

void RegistryEdit::on_okButton_clicked()
{
    changesAccepted = true;
    this->hide();
}

void RegistryEdit::on_cancelButton_clicked()
{
    changesAccepted = false;
    this->hide();
}

bool RegistryEdit::isValueSelected()
{
    bool isValueSelected = false;
    for (int i = 0; i < ui->registryValuesEdit->count(); i++) {
        if (ui->registryValuesEdit->item(i)->isSelected())
            isValueSelected = true;
    }

    return isValueSelected;
}


