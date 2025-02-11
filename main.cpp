#include "musicplayer.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //通过共享内存是否存在来表示该程序只能运行一份
    QSharedMemory memory("MusicPlayer"); //实例化一份共享内容
    if(memory.attach()) //进入函数表示该程序已经在运行了
    {
        QMessageBox::information(nullptr, "温馨提示", "当前程序正在运行.");
        return 0;
    }
    //走到这表示程序尚未运行
    memory.create(1);

    MusicPlayer w;
    w.show();
    return a.exec();
}
