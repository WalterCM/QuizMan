#ifndef QUESTIONFORM_HPP
#define QUESTIONFORM_HPP

#include <QWidget>

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
private:
    Ui::QuestionForm *ui;
};

#endif // QUESTIONFORM_HPP
