#ifndef QUESTIONFORM_HPP
#define QUESTIONFORM_HPP

#include <QWidget>
#include <QButtonGroup>
#include <QGraphicsView>

namespace Ui {
class QuestionForm;
}

class QuestionForm : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionForm(QWidget *parent = 0);
    ~QuestionForm();

    void setQuestion(QString question);
    void addAlternative(QString alternative);
    void setImage(QString path);
private:
    Ui::QuestionForm *ui;
    QGraphicsView *imageView;
    QButtonGroup *group;
};

#endif // QUESTIONFORM_HPP
