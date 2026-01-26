#include "ScreenWidget.h"
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent)
{
    this->isDrawing = false;
}

void ScreenWidget::setScreen(QPixmap img){
    this->fullScreen = img;
    this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->startPoint = event->pos();
        this->isDrawing = true;
    }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *event){
    if(isDrawing){
        this->endPoint = event->pos();
        this->update();
    }
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *event){
    if (startPoint == event->pos()) {
        return;
    }
    if(isDrawing){
        endPoint = event->pos();
        isDrawing = false; // 放在逻辑前面防止重入

        // 【核心修复 1】计算缩放比例
        // 比例 = 图片真实宽度 / 窗口当前宽度
        // 例如：图片2560 / 窗口1706 ≈ 1.5
        // this (窗口)：这是用来展示图片的载体/容器。
        // 由于 Qt 的高分屏缩放机制，操作系统告诉 Qt：“这个屏幕只有 1706 x 1066 这么大”。
        // 所以 this->width() 是 1706，this->height() 是 1066。
        double scaleW = (double)fullScreen.width() / this->width(); 
        double scaleH = (double)fullScreen.height() / this->height();

        // 获取鼠标选中的逻辑区域
        QRect logicalRect = QRect(startPoint, endPoint).normalized();

        // 【核心修复 2】将逻辑区域映射回物理图片区域
        QRect physicalRect(
            logicalRect.x() * scaleW,
            logicalRect.y() * scaleH,
            logicalRect.width() * scaleW,
            logicalRect.height() * scaleH
        );

        // 截图
        QPixmap selectedPixmap = fullScreen.copy(physicalRect);

        if(!selectedPixmap.isNull()){
            emit screenShotTaken(selectedPixmap);
            this->close();
        }
        else{
            QMessageBox::warning(this,"警告","截图区域无效");
        }
    }
}

void ScreenWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);

    // 【核心修复 3】确保背景图铺满窗口
    // 无论窗口是逻辑大小还是物理大小，都把图片拉伸适应窗口
    if (!fullScreen.isNull()) {
        painter.drawPixmap(this->rect(), fullScreen);
    }

    // 绘制遮罩（半透明黑色）
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));

    // 如果正在截图，绘制选中区域的高亮原图
    if (isDrawing || !startPoint.isNull()) { // 简单判断是否有点
        QRect rect = QRect(startPoint, endPoint).normalized();
        
        // 计算比例
        double scaleW = (double)fullScreen.width() / this->width();
        double scaleH = (double)fullScreen.height() / this->height();

        // 算出对应的源图片区域
        QRect sourceRect(
            rect.x() * scaleW, 
            rect.y() * scaleH, 
            rect.width() * scaleW, 
            rect.height() * scaleH
        );

        // 这里的 drawPixmap(target, pixmap, source)
        // 意思是：在窗口的 rect 区域，画 fullScreen 图片的 sourceRect 部分
        painter.drawPixmap(rect, fullScreen, sourceRect); //因为本身就是在this上取的（逻辑图），所以两个点不用乘比例了

        // 画蓝色边框
        painter.setPen(QPen(Qt::blue, 2)); // 线宽设为2更清晰
        painter.drawRect(rect);
    }
}