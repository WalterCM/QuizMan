#include <QDragEnterEvent>
#include <QMimeData>
#include "include/DroppableImageView.hpp"
#include "ui_DroppableImageView.h"

DroppableImageView::DroppableImageView(QGraphicsView *parent) :
    QGraphicsView(parent),
    ui(new Ui::DroppableImageView),
    fileName("")
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
}

DroppableImageView::~DroppableImageView()
{
    delete ui;
}

QString DroppableImageView::getFileName()
{
    return this->fileName;
}

void DroppableImageView::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void DroppableImageView::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void DroppableImageView::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        fileName = url.toLocalFile();
        QPixmap pixmap(fileName);
        QGraphicsScene *scene = new QGraphicsScene(this);
        this->setScene(scene);
        QGraphicsPixmapItem *item = scene->addPixmap(pixmap);
        this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
