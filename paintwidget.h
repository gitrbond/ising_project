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

//outgoing events that this object emits (tries to call these methods from somewhere)
signals: //"signals" keyword is an extension of Qt framework
    void paint_resized(QSize old_size, QSize new_size);
    void paint_double_clicked(int x, int y);
    /*void paint_scrolled(int w, int x, int y);
    void paint_mouse_move(int x, int y);
    void paint_mouse_button_down(int x, int y);
    void paint_mouse_button_up();*/

//handlers that process incoming events
public slots: //"slots" keyword is an extension of Qt framework

//overridden methods of base classes - processing events
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *);
    /*virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);*/
};

#endif // PAINTWIDGET_H
