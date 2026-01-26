#include <QApplication> // < >系统路径
#include "Widget.h" // " "当前文件夹

int main(int argc, char* argv[]){
    QApplication app(argc, argv);//创建应用对象

    Widget w;
    w.show();
    // 普通的窗口Widget是没有exec()的
    return app.exec();
}