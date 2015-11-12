#include "include/QuestionForm.hpp"
#include "ui_QuestionForm.h"

QuestionForm::QuestionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionForm)
{
    ui->setupUi(this);
}

QuestionForm::~QuestionForm()
{
    delete ui;
}
