#ifndef DROPPABLEIMAGEVIEW_HPP
#define DROPPABLEIMAGEVIEW_HPP

#include <QWidget>
#include <QGraphicsView>

namespace Ui {
class DroppableImageView;
}

class DroppableImageView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit DroppableImageView(QGraphicsView *parent = 0);
    ~DroppableImageView();

    QString getFileName();

    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
private:
    Ui::DroppableImageView *ui;
    QString fileName = "";
};

#endif // DROPPABLEIMAGEVIEW_HPP
