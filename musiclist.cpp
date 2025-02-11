#include "musiclist.h"

MusicList::MusicList() {}

void MusicList::setMusicList(const QList<QUrl> &paths)
{
    for(const auto& e : paths)
    {
        //当前歌曲不存在
        if(!musicPath.contains(e.toLocalFile()))
        {
            musicPath.insert(e.toLocalFile());
            QMimeDatabase mimeDB;
            QMimeType mimeType = mimeDB.mimeTypeForFile(e.toLocalFile());
            QString mime = mimeType.name();
            if(mime == "audio/mpeg" || mime == "audio/flac" || mime == "audio/wav")
            {
                Music music(e);
                musicList.push_back(music);
            }
        }
    }
}

Music* MusicList::getMusicByID(const QString &id)
{
    for(auto& music : musicList)
    {
        if(music.id == id)
        {
            return &music;
        }
    }
    return nullptr;
}

void MusicList::saveToDatabase()
{
    for(auto& music : musicList)
    {
        music.insertToDatabase();
    }
}

void MusicList::loadFromDatabase()
{
    QSqlQuery query;
    query.prepare("select * from musicData");
    if(!query.exec()) //查询数据失败
    {
        qDebug() << "加载歌曲数据失败.";
        return ;
    }

    //遍历查询到的数据
    while(query.next())
    {
        Music music;
        music.id = query.value(1).toString();
        music.name = query.value(2).toString();
        music.singer = query.value(3).toString();
        music.album = query.value(4).toString();
        music.time = query.value(5).toLongLong();
        music.url = query.value(6).toString();
        music.isLike = query.value(7).toInt();
        music.isRecent = query.value(8).toInt();
        musicList.push_back(music); //添加到音乐列表
        musicPath.insert(music.url.toLocalFile());
    }
    qDebug() << "歌曲数据全部加载完毕.";
}
