#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication> // 需要包含这个来使用 primaryScreen
#include <QFileDialog>
#include <QMessageBox>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget w;
    w.setWindowTitle("Lesson 6: Save to File");
    w.resize(800, 600);
    
    // 使用 this 指针或 &w 指定父对象是好习惯，方便内存管理
    QVBoxLayout *layout = new QVBoxLayout(&w); 

    QLabel *imageLabel = new QLabel("Preview Region", &w);
    imageLabel->setAlignment(Qt::AlignCenter);
    // 给Label设置个边框，这样你看得见它在哪里
    imageLabel->setStyleSheet("QLabel { background-color: #eee; border: 1px solid #ccc; }");

    QPushButton *btnCapture = new QPushButton("截图 (Capture)");
    
    // 【问题1回答】：这里不写 &w 可以吗？
    // 答案：完全可以。
    // 原因：当你下面调用 layout->addWidget(btnSave) 时，
    // layout 会自动把 btnSave 的父对象设置为 layout 所属的窗口 (即 w)。
    // 所以这里的 &w 写不写效果一样，只要你把它加进布局里了。
    QPushButton *btnSave = new QPushButton("保存 (Save)");

    layout->addWidget(imageLabel);
    layout->addWidget(btnCapture);
    layout->addWidget(btnSave);

    // 用于存储截图的变量
    QPixmap currentScreenshot; 

    // --- 截图功能 ---
    QObject::connect(btnCapture, &QPushButton::clicked, &w, [&]() {
        w.hide(); // 隐藏窗口以便截取全屏
        
        // 延时500ms等待窗口完全消失
        QTimer::singleShot(500, [&]() {
            QScreen *screen = QGuiApplication::primaryScreen();
            if (!screen) return;
            
            // 截取全屏 (WinID 0 代表桌面)
            currentScreenshot = screen->grabWindow(0);
            
            // 缩放以便在 Label 中预览 (保持比例)
            QPixmap scaled = currentScreenshot.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(scaled);
            
            w.show(); // 恢复窗口显示
        });
    });

    // --- 保存功能 ---
    // 【错误修正】：这里之前写的是 click (槽函数)，信号必须是 clicked
    QObject::connect(btnSave, &QPushButton::clicked, &w, [&]() {
        if (currentScreenshot.isNull()) {
            // 【问题2回答】：&w 用来做什么？
            // 答案：指定父窗口。
            // 效果：1. 消息框会显示在 w 的正中间。
            //       2. 这里会产生"模态"效果，即你在关掉消息框之前，无法点击 w 窗口的其他部分。
            QMessageBox::warning(&w, "提示", "请先截图 (Please capture first)");
            return; 
        }

        // 【问题3回答】：解读参数
        // 参数1 (&w): 父窗口 (同上，对话框居中并模态)。
        // 参数2 ("Save Image"): 对话框左上角的标题。
        // 参数3 ("."): 默认打开的路径。"." 代表当前程序运行的目录。
        // 参数4 ("Images..."): 文件类型过滤器。用户只能看到和保存这些格式。
        QString filename = QFileDialog::getSaveFileName(&w, "Save Image", ".", "Images (*.png *.jpg)");

        if (filename.length() > 0) {
            // save 函数会根据后缀名自动判断格式
            bool success = currentScreenshot.save(filename);
            if (success) {
                QMessageBox::information(&w, "成功", "保存成功!");
            } else {
                QMessageBox::warning(&w, "失败", "截图保存失败");
            }
        }
    });
    
    w.show();
    return app.exec();
}