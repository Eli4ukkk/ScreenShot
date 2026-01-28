#ifndef SHOT_WIDGET_H
#define SHOT_WIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>

class ShotWidget : public QWidget
{
Q_OBJECT
public:
    ShotWidget(QWidget *parent = nullptr);
    ~ShotWidget();
    void getfullscreen(QPixmap isFullScreen);
signals:
    void ShotTaken(QPixmap selectRegion);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QPoint startPoint;
    QPoint endPoint;
    QPixmap img;
    bool isDrawing;
};

#endif //SHOT_WIDGET_H