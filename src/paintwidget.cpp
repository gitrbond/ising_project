#include "hdr/paintwidget.h"
#include <QPainter>
#include <QMouseEvent>

//constructor - initialization
PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent) //call constructor of base class
{
    //create image in size of widget
    image = new QImage(size(), QImage::Format_ARGB32_Premultiplied); //fast RGB with alpha
    image->fill(0); //erase trash pixels
}

//destructor - free resources
PaintWidget::~PaintWidget()
{
    delete image;
}

void PaintWidget::resizeEvent(QResizeEvent *event)
{
    //recreate cache image with new size
    delete image;
    image = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
    image->fill(0);
    //ask MainWidget class to draw picture again
    emit canvas_resized(event->oldSize(), event->size());
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
    painter.drawImage(0, 0, *image);
}
