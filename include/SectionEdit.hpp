#ifndef SectionEDIT_HPP
#define SectionEDIT_HPP

#include <QDialog>

namespace Ui {
class SectionEdit;
}

class SectionEdit : public QDialog
{
    Q_OBJECT

public:
    explicit SectionEdit(QWidget *parent = 0,
                          QString SectionName = "",
                          QStringList SectionValues = QStringList(),
                          int amount = 0);
    ~SectionEdit();

    QString getSectionName();
    QStringList getSectionValues();
    int getSectionAmount();
    bool areChangesAccepted();
private slots:
    void on_SectionValuesEdit_itemSelectionChanged();

    void on_moveUpButton_clicked();
    void on_moveDownButton_clicked();

    void on_removeItemButton_clicked();

    void on_okButton_clicked();
    void on_cancelButton_clicked();
private:
    bool isValueSelected();

    Ui::SectionEdit *ui;

    QString criteria;
    bool changesAccepted;
};

#endif // SectionEDIT_HPP
