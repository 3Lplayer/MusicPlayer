#ifndef LYRICS_H
#define LYRICS_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QFile>
#include <QPair>
#include <QDebug>

namespace Ui {
class Lyrics;
}

class Lyrics : public QWidget
{
    Q_OBJECT

public:
    explicit Lyrics(QWidget *parent = nullptr);
    ~Lyrics();

    bool parseLrcFile(const QString& lrcFile); //解析歌曲歌词文件
    void showLrcWord(qint64 position); //显示当前时间的歌词

protected:
    int getCurDurationLrc(qint64 duration); //获取当前进度的歌词的下标
    QString getCurIndexLrc(int index); //获取当前下标对应的歌词

private:
    Ui::Lyrics *ui;
    QVector<QPair<qint64, QString>> lrcList; //存储每一句歌词的时间点和歌词
};

#endif // LYRICS_H
