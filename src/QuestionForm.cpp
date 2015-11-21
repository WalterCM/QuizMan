#include <QRadioButton>
#include <QDebug>
#include "include/QuestionForm.hpp"
#include "ui_QuestionForm.h"

QuestionForm::QuestionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionForm)
{
    ui->setupUi(this);

    group = new QButtonGroup;
    imageView = new QGraphicsView;

    ui->imageLayout->addWidget(imageView);
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
    QRadioButton *alternativeRadio = new QRadioButton(alternative, this);
    group->addButton(alternativeRadio);
    ui->alternativeLayout->addWidget(alternativeRadio);
}

void QuestionForm::setImage(QString path)
{
    QPixmap pixmap(path);
    QGraphicsScene *scene = new QGraphicsScene(this);

    imageView->setSceneRect(pixmap.rect());
    imageView->setScene(scene);
    imageView->setFixedSize(400, 400);
    scene->addPixmap(pixmap);
    imageView->fitInView(QRect(0, 0, 1600, 1600), Qt::KeepAspectRatio);
}
