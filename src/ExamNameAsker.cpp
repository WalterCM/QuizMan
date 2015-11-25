
#include <QMessageBox>
#include "include/ExamNameAsker.hpp"
#include "ui_ExamNameAsker.h"

ExamNameAsker::ExamNameAsker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExamNameAsker)
{
    ui->setupUi(this);
    this->examName = "";
}

ExamNameAsker::~ExamNameAsker()
{
    delete ui;
}

QString ExamNameAsker::getExamName()
{
    return this->examName;
}

bool ExamNameAsker::doesSave()
{
    return this->save;
}

void ExamNameAsker::on_askerBack_clicked()
{
    save = false;
    this->hide();
}

void ExamNameAsker::on_askerSave_clicked()
{
    QMessageBox messageBox;

    if (this->examName == "") {
        messageBox.critical(this,"El nombre es necesario",
                            "El examen guardado necesita un nombre.\n"
                            "Por favor ingrese un nombre antes de continuar");
        messageBox.setFixedSize(700,200);

        return;
    }

    int answer;
    answer = messageBox.question(this,"Esta apunto de guardar el examen?",
                        "Esta seguro que desea guardar el examen?",
                        "Regresar", "Guardar Examen");
    messageBox.setFixedSize(500,200);

    if (answer == 0) return;

    save = true;
    this->hide();
}

void ExamNameAsker::on_lineEdit_textEdited(const QString &arg1)
{
    this->examName = arg1;
}
