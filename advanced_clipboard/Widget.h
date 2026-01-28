#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QClipboard>
#include "ShotWidget.h"

class Widget : public QWidget
{
Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QLabel *fullscreenLabel;
    QPushButton *btn_newCapture;
    QPushButton *btn_Save;
    QPushButton *btn_copy;
    QPixmap fullscreen;
    QPixmap resultRegion;
    
    void function_Save();
    void function_newCpature();
    void saveToClipboard();
};

#endif // WIDGET_H