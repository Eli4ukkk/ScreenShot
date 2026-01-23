#include "Widget.h"
#include <QScreen>
#include <QGuiApplication>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QPainter>
#include <QColor>

Widget::Widget(QWidget *parent) : QWidget(parent) // 调用构造函数 传递指针
{
    this->resize(400, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    // 初始化控件
    imageLabel = new QLabel(this);
    btnCapture = new QPushButton("Capture(截图)", this);
    btnSave = new QPushButton("Save(保存)", this);
    // 将控件添加到布局中
    layout->addWidget(imageLabel);
    layout->addWidget(btnCapture);
    layout->addWidget(btnSave);
    // 连接按钮点击信号到相应的槽函数
    connect(btnCapture, &QPushButton::clicked, this, &Widget::captureScreen);
    connect(btnSave, &QPushButton::clicked, this, &Widget::saveToFile);
}
// 析构函数
Widget::~Widget(){};
// 实现函数功能
void Widget::captureScreen()
{
    this->hide();
    QTimer::singleShot(500, [this]()->void
    {
        QScreen* screen = QGuiApplication::primaryScreen();
        if(screen){
            pixmap = screen->grabWindow(0);
            this->showMask(); // 调用刚刚写的函数
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        // this->show(); 在showMask里实现了 
    });
}
void Widget::saveToFile()
{
    if (!pixmap.isNull())
    {
        QString path = QFileDialog::getSaveFileName(this, "保存图片", ".");
        // 方法 1:
        // bool success = pixmap.save(path);
        // if(success){
        //     pixmap.save(path);
        //     QMessageBox::information(this, "成功", "成功保存！");
        // }
        // else{
        //     QMessageBox::warning(this, "失败", "截图保存失败！");
        //     return;
        // }
        // 方法 2：
        if (path.length() > 0)
        {
            pixmap.save(path);
            QMessageBox::information(this, "成功", "成功保存！");
        }
        else
        {
            QMessageBox::warning(this, "失败", "截图保存失败！");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请先截图！");
        return;
    }
}
//LESSON_9 SHOWMASK
void Widget::showMask(){
    
    // A.屏窗口
    // 1.创建临时窗口
    QLabel *mask = new QLabel();
    // 2.无边框 + 置顶
    mask->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // 3.设置为全屏大小
    mask->setGeometry(QGuiApplication::primaryScreen()->geometry());
    
    // B.让背景变暗
    // 1.复制一份原图作为画布
    QPixmap darkPixmap = pixmap;
    // 2.创建painter处理图片
    QPainter painter(&darkPixmap);   
    painter.fillRect(darkPixmap.rect(), QColor(0,0,0,100));// 范围：整张图的大小 (0,0 到 宽,高) 颜色为黑色 透明度为100(0-255) QColor(R,G,B,透明度)
    painter.end(); // 保存修改
    // 3.显示图像和窗口
    mask->setPixmap(darkPixmap);
    mask->show();
    // 4.防止卡死 清理内存 再把主窗口显示
    QTimer::singleShot(3000, [mask,this]()->void{
        mask->close();
        delete mask;
        this->show();
    });
}