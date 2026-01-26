#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QLabel* imageLabel;
    QPushButton* btnCapture;
    QPushButton* btnSave;
    QPixmap pixmap;
    // 声明函数
    void captureScreen();
    void saveToFile();
    void showMask();
};

#endif // WIDGET_H
