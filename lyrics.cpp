#include "lyrics.h"
#include "ui_lyrics.h"

Lyrics::Lyrics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lyrics)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); //去掉窗口周边的功能按钮

    //设置隐藏按钮图标
    ui->hideBtn->setIcon(QIcon(":/images/hide.png"));
    ui->hideBtn->setIconSize(QSize(30,30));

    //设置动画效果
    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry", this);
    animation->setDuration(300);
    animation->setStartValue(QRect(0, 0, this->width(), this->height()));
    animation->setEndValue(QRect(0, this->height(), this->width(), this->height()));
    //设置动画触发时机
    connect(ui->hideBtn, &QPushButton::clicked, this, [=]{
       animation->start();
    });
    connect(animation, &QPropertyAnimation::finished, this, [=]{
       this->hide();
    });
}

Lyrics::~Lyrics()
{
    delete ui;
}

bool Lyrics::parseLrcFile(const QString &lrcFile)
{
    QFile file(lrcFile);
    //以只读的方式打开文件
    if(!file.open(QIODevice::ReadOnly)) //打开失败
    {
        qDebug() << "文件打开失败: " << lrcFile;
        return false;
    }

    //清空上一首歌曲的歌词
    lrcList.clear();

    //没到末尾就继续
    while(!file.atEnd())
    {
        //每次解析一行
        //歌词格式可能如下:
        //[0:12.42]xxxxxxxxxxxx
        //[0:12.34.00]xxxxxxxxxx
        QString lrcLine = file.readLine();

        //根据分隔符确定时间和歌词
        int pos = lrcLine.indexOf(']', 0);
        QString lrcTime = lrcLine.mid(1, pos - 1);
        QString lrcText = lrcLine.mid(pos + 1, lrcLine.size() - pos - 2);

        //计算时间单位毫秒
        qint64 duration = 0;
        int start = 0;
        int end = lrcTime.indexOf(':', start);
        duration += lrcTime.mid(start, end - start).toInt() * 60 * 1000; //分->毫秒
        start = end + 1;

        end = lrcTime.indexOf('.', start);
        duration += lrcTime.mid(start, end - start).toInt() * 1000; //秒->毫秒
        start = end + 1;

        end = lrcTime.indexOf('.', start);
        duration += lrcTime.mid(start, end - start).toInt(); //毫秒

        //设置到lrcList中
        lrcList.push_back({duration, lrcText});
    }

//    //测试歌词是否获取准确
//    for(const auto& e : lrcList)
//    {
//        qDebug() << e.first << ":" << e.second;
//    }
    return true;
}

void Lyrics::showLrcWord(qint64 position)
{
    int index = getCurDurationLrc(position);
    if(index == -1) //表示暂无歌词
    {
        ui->topLine1->setText("");
        ui->topLine2->setText("");
        ui->topLine3->setText("");
        ui->midLine->setText("暂无歌词");
        ui->downLine1->setText("");
        ui->downLine1->setText("");
        ui->downLine1->setText("");
    }
    else
    {
        ui->topLine1->setText(getCurIndexLrc(index - 3));
        ui->topLine2->setText(getCurIndexLrc(index - 2));
        ui->topLine3->setText(getCurIndexLrc(index - 1));
        ui->midLine->setText(getCurIndexLrc(index));
        ui->downLine1->setText(getCurIndexLrc(index + 1));
        ui->downLine2->setText(getCurIndexLrc(index + 2));
        ui->downLine3->setText(getCurIndexLrc(index + 3));
    }
}

int Lyrics::getCurDurationLrc(qint64 duration)
{
    //判空,为空表示没有歌词
    if(lrcList.isEmpty())
    {
        return -1;
    }

    if(duration <= lrcList[0].first)
    {
        return 0;
    }

    for(int i = 1; i < lrcList.size(); ++i)
    {
        //介于两句歌词之间,显示上一句的歌词
        if(duration >= lrcList[i - 1].first && duration < lrcList[i].first)
        {
            return i - 1;
        }
    }
    return lrcList.size() - 1;
}

QString Lyrics::getCurIndexLrc(int index)
{
    //判断下标是否合法
    if(index < 0 || index >= lrcList.size())
    {
        return "";
    }
    return lrcList[index].second;
}
