#ifndef MUSIC_H
#define MUSIC_H

#include <QString>
#include <QUrl>
#include <QUuid>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class Music
{
public:
    Music();
    Music(const QUrl& songUrl);

    void fetchSongInfo(); // 获取歌曲元数据
    QString getLrcFilePath() const; //获取歌曲歌词文件的路径
    void insertToDatabase(); //将数据插入到数据库中

public:
    QString id;
    QString name;
    QString singer;
    QString album;
    qint64 time;
    bool isLike = false;
    bool isRecent = false;
    QUrl url;
};

#endif // MUSIC_H
