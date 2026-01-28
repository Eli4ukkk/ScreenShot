#include "ShotWidget.h"

ShotWidget::ShotWidget(QWidget *parent) : QWidget(parent)
{
    this->isDrawing = true;
    this->update();
}
ShotWidget::~ShotWidget(){};
void ShotWidget::getfullscreen(QPixmap fullscreen)
{
    this->img = fullscreen;
    this->update();
}
void ShotWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        this->startPoint = event->pos();
        this->update();
    }
}
void ShotWidget::mouseMoveEvent(QMouseEvent *event)
{   
    if(isDrawing == false)
        return; 
    this->endPoint = event->pos();
    this->update();
}
void ShotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (startPoint == endPoint)
        return;
    if (isDrawing == false)
        return;
    this->endPoint = event->pos();
    QRect rect = QRect(startPoint, endPoint).normalized();
    double rateW = (double)img.width() / this->width();
    double rateH = (double)img.height() / this->height();
    QRect sourceRect{
        (int)(rect.x() * rateW),
        (int)(rect.y() * rateH),
        (int)(rect.width() * rateW),
        (int)(rect.height() * rateH)
    };
    QPixmap selectRegion = img.copy(sourceRect);
    emit ShotTaken(selectRegion);
    this->update();
}
void ShotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(this->rect(), img);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
    QRect rect = QRect(startPoint, endPoint).normalized();
    double rateW = (double)img.width() / this->width();
    double rateH = (double)img.height() / this->height();
    QRect sourceRect{
        (int)(rect.x() * rateW),
        (int)(rect.y() * rateH),
        (int)(rect.width() * rateW),
        (int)(rect.height() * rateH)
    };
    painter.drawPixmap(rect, img, sourceRect);
    painter.setPen(QPen(Qt::red, 0.1));
    painter.drawRect(rect);
}
