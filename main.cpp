#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QColor>
#include <QProcess>
#include <QTimer>
#include <QDesktopServices>
#include <QDebug> // For debugging
#include <QDateTime> // For generating unique file names
#include <QUrl>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 创建一个定时器，每隔一段时间检测屏幕白屏
    QTimer timer;
    timer.setInterval(1000); // 设置定时器间隔为1秒
    //跟踪程序是否启动
    bool programStarted = false;
    QObject::connect(&timer, &QTimer::timeout, [&](){
        if (!programStarted) {
            // 获取屏幕截图
            QPixmap screenshot = screen->grabWindow(0);

            // Save the screenshot with a unique filename
            //        QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png";
            //        screenshot.save(fileName, "PNG");
            //        qDebug() << "保存成功";
            // 遍历截图中的像素
            QImage image = screenshot.toImage();
            int totalPixels = image.width() * image.height();
            int whitePixels = 0;

            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    // 获取像素的颜色
                    QColor pixelColor = image.pixelColor(x, y);
                    // 判断颜色是否足够接近白色
                    if (pixelColor.red() > 220 && pixelColor.green() > 220 && pixelColor.blue() > 220) {
                        whitePixels++;
                    }
                }
            }
            float whiteRatio = static_cast<float>(whitePixels) / totalPixels;
            qDebug() << "白色像素占比：" << whiteRatio;
            // 如果屏幕是白屏，原神！启动！
            if (whiteRatio > 0.9) {
                qDebug() << "原神！启动！";
                QString program = "D:/game/Genshin Impact/Genshin Impact Game/YuanShen.exe";
                QStringList arguments;
                bool started = QProcess::startDetached(program, arguments);
                if (started) {
                    qDebug() << "Successfully started the process";
                    programStarted = true;
                    timer.stop();
                } else {
                    qDebug() << "Failed to start the process";
                }
            }
        }

    });

    // 启动定时器
    timer.start();

    int appResult = app.exec();

    // Clean up
    timer.stop(); // Stop the timer before exiting
    return appResult;
}
