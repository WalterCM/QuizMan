#include <QRadioButton>
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

void QuestionForm::setQuestion(QString question)
{
    ui->questionArea->setText(question);
}

void QuestionForm::addAlternative(QString alternative)
{
    ui->alternativesArea->setLayout(new QGridLayout);
    //ui->alternativesArea->layout()->addChildWidget(new QRadioButton);
}
