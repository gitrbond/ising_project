#include "paintwidget.h"
#include <QPainter>
#include <QMouseEvent>

//constructor - initialization
PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent) //call constructor of base class
{
    //setMouseTracking(true);
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
    //ask MainWidget class to draw picture again ("emit" keyword is an extension of Qt framework)
    emit paint_resized(event->oldSize(), event->size());
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
    painter.drawImage(0, 0, *image);
}

/*void PaintWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        emit paint_double_clicked(event->x(), event->y());
}

void PaintWidget::wheelEvent(QWheelEvent *event)
{
    emit paint_scrolled(event->delta(), event->x(), event->y());
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    emit paint_mouse_move(event->x(), event->y());
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        emit paint_mouse_button_down(event->x(), event->y());
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *)
{
    emit paint_mouse_button_up();
}*/
