#include "music.h"

Music::Music()
{

}

Music::Music(const QUrl &songUrl)
    : id(QUuid::createUuid().toString()), url(songUrl)
{
    fetchSongInfo();
}

void Music::fetchSongInfo()
{
    QMediaPlayer player;
    player.setMedia(url); //解析url媒体的元数据

    //因为调用setMedia后会立刻返回,并不会等待解析完成后才返回,所以需要手动等待解析完成
    while(!player.isMetaDataAvailable())
    {
        QCoreApplication::processEvents();
    }

    //元数据已经解析完成了,获取元数据信息填充相应字段
    if(player.isMetaDataAvailable())
    {
        name = player.metaData("Title").toString();
        singer = player.metaData("Author").toString();
        album = player.metaData("AlbumTitle").toString();
        time = player.metaData("Duration").toLongLong();

        //处理无法获取name/singer/album的情况
        QString musicInfo = url.fileName(); //获取到完整的歌曲命名信息
        int index = musicInfo.indexOf('-'); //查找musicInfo中是否存在分割歌曲名和歌手的分隔符

        //如果name为空
        if(name.isEmpty())
        {
            //不存在分隔符
            if(index == -1)
            {
                name = musicInfo.mid(0, musicInfo.indexOf('.')).trimmed();
            }
            else
            {
                name = musicInfo.mid(0, index).trimmed();
            }
        }

        //如果singer为空
        if(singer.isEmpty())
        {
            //不存在分隔符
            if(index == -1)
            {
                singer = "未知歌手";
            }
            else
            {
                singer = musicInfo.mid(index + 1, musicInfo.indexOf('.') - index - 1).trimmed();
            }
        }

        //如果album为空
        if(album.isEmpty())
        {
            album = "未知专辑";
        }

        qDebug() << name << "-" << singer << "-" << album << "-" << time;
    }
}

QString Music::getLrcFilePath() const
{
    QString lrcFile = url.toLocalFile();
    lrcFile.replace(".mp3", ".lrc"); //将原本路径中的后缀替换

    return lrcFile;
}

void Music::insertToDatabase()
{
    //先判断需要插入的数据是否已经存在数据库中
    QSqlQuery query;
    //select 1表示是否查询到指定数据,查到返回1,再作为select exists的子查询返回逻辑值
    query.prepare("select exists(select 1 from musicData where musicID = ?)");
    query.addBindValue(id);
    if(!query.exec()) //查询失败
    {
        qDebug() << "查询歌曲数据失败: " << query.lastError().text();
        return ;
    }
    //判断查询的返回值
    if(query.next())
    {
        bool isExists = query.value(0).toBool();
        if(isExists) //表示数据已经存在
        {
            //只需更新当前的isLike和isRecent
            query.prepare("update musicData set isLike = ?, isRecent = ? where musicID = ?");
            query.addBindValue(isLike ? 1 : 0);
            query.addBindValue(isRecent ? 1 : 0);
            query.addBindValue(id);
            if(!query.exec()) //更新失败
            {
                qDebug() << "更新歌曲数据失败: " << query.lastError().text();
                return ;
            }
            qDebug() << "更新歌曲数据成功: " << name;
        }
        else //不存在
        {
            //插入数据
            query.prepare("insert into musicData(musicID, name, singer, album, duration, url, isLike, isRecent) values(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(id);
            query.addBindValue(name);
            query.addBindValue(singer);
            query.addBindValue(album);
            query.addBindValue(time);
            query.addBindValue(url);
            query.addBindValue(isLike ? 1 : 0); //这里数据库中不存逻辑值,用1/0表示真/假
            query.addBindValue(isRecent ? 1 : 0);
            if(!query.exec()) //插入失败
            {
                qDebug() << "插入歌曲数据失败: " << query.lastError().text();
                return ;
            }
            qDebug() << "插入歌曲数据成功: " << name;
        }
    }
}
