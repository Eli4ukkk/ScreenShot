#include "Widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);
    fullscreenLabel = new QLabel(this);
    btn_newCapture = new QPushButton("newCapture(截图)", this);
    btn_Save = new QPushButton("Save(保存)", this);
    btn_copy = new QPushButton("Copy to Clipboard(复制到剪贴板)", this);
    // fullscreen = new QPixmap(); q1:为什么fullscreen不用new?
    layout->addWidget(fullscreenLabel); // addWidget()
    layout->addWidget(btn_newCapture);
    layout->addWidget(btn_Save);
    layout->addWidget(btn_copy);
    connect(btn_newCapture, &QPushButton::clicked, this, &Widget::function_newCpature);
    connect(btn_Save, &QPushButton::clicked, this, &Widget::function_Save);
    connect(btn_copy, &QPushButton::clicked, this, &Widget::saveToClipboard);
    // this->setLayout(layout); Line10:已经指定父对象为this,不需要再调用setLayout(),会自己调用
}
Widget::~Widget() {};
void Widget::function_newCpature()
{
    this->hide();
    QTimer::singleShot(500, [this]() -> void
                       {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen == nullptr)
            return;
        fullscreen = screen->grabWindow(0);
        ShotWidget *shotWidget = new ShotWidget(nullptr);
        shotWidget->setAttribute(Qt::WA_DeleteOnClose);
        shotWidget->getfullscreen(fullscreen);
        shotWidget->showFullScreen();
        connect(shotWidget, &ShotWidget::ShotTaken, [this, shotWidget](QPixmap selectRegion) -> void{
            resultRegion = selectRegion;
            fullscreenLabel->setPixmap(selectRegion.scaled(fullscreenLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
            shotWidget->close();
            this->show();
        }); });
}
void Widget::function_Save()
{
    if (!resultRegion.isNull())
    {
        QString path = QFileDialog::getSaveFileName(this, "保存图片", ".");
        if (path.length() > 0)
        {
            resultRegion.save(path);
            QMessageBox::information(this, "Information", "保存成功");
        }
        else
        {
            QMessageBox::warning(this, "Warning", "保存失败");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "请先截图");
        return;
    }
}
void Widget::saveToClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(resultRegion);
}
