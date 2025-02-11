#ifndef COMMON_H
#define COMMON_H

#include <QWidget>
#include <musiclist.h>
#include <QScrollBar>
#include <QMediaPlaylist>
#include "listitem.h"

namespace Ui {
class Common;
}

//用于标识该页面是哪个类型的
enum PageType
{
    LIKE,
    LOCAL,
    RECENT
};

class Common : public QWidget
{
    Q_OBJECT

public:
    explicit Common(QWidget *parent = nullptr);
    ~Common();

    void setFrontCover(const QString& path); //设置页面的封面图片
    void setTitle(const QString& title); //设置页面标题
    void setType(PageType type); //设置页面类型
    void addSongs(const MusicList& list); //获取音乐列表里面与自身类型一致的音乐
    void updateSongs(MusicList& list); //更新显示在页面上的歌曲
    void addMusicToPlaylist(MusicList& musicList, QMediaPlaylist* playlist); //将自身的歌曲添加到播放列表
    QString getMusicIDByIndex(int index); //获取songs里面指定下标的歌曲id
    void setFrontCover(QPixmap pixmap); //设置播放歌曲封面

private:
    Ui::Common *ui;
    PageType type;
    QVector<QString> songs; //存储每个页面自己的音乐的id

signals:
    void updateILikePage(bool,QString);
    void playAllMusic(PageType);
    void doubleClicked(Common*,int);

};

#endif // COMMON_H
