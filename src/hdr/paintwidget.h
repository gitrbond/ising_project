#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();
    QImage *image; //internal cache picture in memory

//outgoing events that this object emits
signals:
    void canvas_resized(QSize old_size, QSize new_size);

//handlers that process incoming events
public slots:

//overridden methods of base classes - processing events
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *);
};

#endif // PAINTWIDGET_H
