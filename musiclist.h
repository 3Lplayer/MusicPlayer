#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QVector>
#include <QMimeDatabase>
#include <QMimeType>
#include <QSet>
#include "music.h"

class MusicList
{
public:
    MusicList();

    void setMusicList(const QList<QUrl>& paths); //设置音乐列表
    Music* getMusicByID(const QString& id); //获取指定id的歌曲的music
    void saveToDatabase(); //将歌曲数据保存到数据库
    void loadFromDatabase(); //将数据库中保存的歌曲数据加载到当前的musicList中

public:
    QVector<Music> musicList;
    QSet<QString> musicPath; //存储歌曲路径,用于判断歌曲是否重复插入
};

#endif // MUSICLIST_H
