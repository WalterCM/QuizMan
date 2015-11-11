#ifndef REGISTRYEDIT_HPP
#define REGISTRYEDIT_HPP

#include <QDialog>

namespace Ui {
class RegistryEdit;
}

class RegistryEdit : public QDialog
{
    Q_OBJECT

public:
    explicit RegistryEdit(QWidget *parent = 0,
                          QString registryName = "",
                          QStringList registryValues = QStringList(),
                          int amount = 0);
    ~RegistryEdit();

    QString getRegistryName();
    QStringList getRegistryValues();
    int getRegistryAmount();
    bool areChangesAccepted();
private slots:
    void on_registryValuesEdit_itemSelectionChanged();

    void on_moveUpButton_clicked();
    void on_moveDownButton_clicked();

    void on_removeItemButton_clicked();

    void on_okButton_clicked();
    void on_cancelButton_clicked();
private:
    bool isValueSelected();

    Ui::RegistryEdit *ui;

    QString criteria;
    bool changesAccepted;
};

#endif // REGISTRYEDIT_HPP
