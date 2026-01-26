#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>

class ScreenWidget : public QWidget //
{
    Q_OBJECT

public:
    explicit ScreenWidget(QWidget *parent = nullptr); //
    void setScreen(QPixmap img);                      //

signals:
    void screenShotTaken(QPixmap screen);

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPoint startPoint;
    QPoint endPoint;
    QPixmap fullScreen; //
    bool isDrawing;     //
};

#endif // SCREENWIDGET_H