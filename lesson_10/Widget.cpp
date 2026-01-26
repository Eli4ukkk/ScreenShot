#include "Widget.h"
#include <QScreen>
#include <QGuiApplication>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QPainter>
#include <QColor>
#include "ScreenWidget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) // 调用构造函数 传递指针
{
    this->resize(400, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    // 初始化控件
    imageLabel = new QLabel(this);
    btnNewCapture = new QPushButton("NewCapture(新建截图)", this);
    btnSave = new QPushButton("Save(保存)", this);
    // 将控件添加到布局中
    layout->addWidget(imageLabel);
    layout->addWidget(btnNewCapture);
    layout->addWidget(btnSave);
    // 连接按钮点击信号到相应的槽函数
    connect(btnNewCapture, &QPushButton::clicked, this, &Widget::captureScreen);
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
        // 1. 获取屏幕对象
        pixmap = QGuiApplication::primaryScreen()->grabWindow(0);

        // 2. 截取原始像素图
        ScreenWidget *screenWidget = new ScreenWidget(nullptr); // 创建截图窗口 nullptr表示无父窗口 不然hide会连子窗口一起隐藏

        // 3. 创建窗口并传递处理好的图片
        screenWidget->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除 防止内存泄漏
        screenWidget->setScreen(this->pixmap); // 立即传递截图
        screenWidget->showFullScreen(); // 全屏显示截图窗口
        // 4. 连接信号
        connect(screenWidget, &ScreenWidget::screenShotTaken, this, [this](QPixmap screen){ // 捕获截图信号 selectedPixmap (关键点)
            this->pixmap = screen;
            this->imageLabel->setPixmap(this->pixmap.scaled(this->imageLabel->size(), Qt::KeepAspectRatio));
            this->show(); // 显示主窗口
        });
    });
}
void Widget::saveToFile()
{
    if (!pixmap.isNull())
    {
        QString path = QFileDialog::getSaveFileName(this, "保存图片", ".", "Images (*.png *.jpg)");
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
//在LESSON_10中删除 彻底被取代了
// void Widget::showMask(){}
