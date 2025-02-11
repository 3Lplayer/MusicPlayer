#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QPropertyAnimation>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QMenu>
#include "common.h"
#include "volumecontrol.h"
#include "musiclist.h"
#include "lyrics.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicPlayer;
}
QT_END_NAMESPACE

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

    void initUI(); //初始化ui界面
    void initMedia(); //初始化媒体相关类
    void initSignalSlot(); //初始化各按钮的槽函数
    void initDatabase(); //初始化数据库
    void loadMusicData(); //加载歌曲数据
    QJsonArray randomImage(); //随机获取推荐音乐封面图片
    void playTypePageMusic(Common* page, int index); //播放指定页面的指定索引歌曲

private slots:
    void on_quit_clicked();
    void switchWindow(int id); //切换显示界面
    void on_voice_clicked();
    void on_addMusic_clicked();
    void updatePageOfTypeLike(bool isLike, QString id); //更新三个页面里面喜欢按钮的变化
    void onPlayClicked(); //按下播放按钮
    void onPlayUpClicked(); //按下播放上一首按钮
    void onPlayDownClicked(); //按下播放下一首按钮
    void onPlayModeClicked(); //按下切换播放模式按钮
    void onPlayAllMusicClicked(PageType pageType); //根据页面类型播放器所有音乐
    void onDoubleClicked(Common* page, int index); //根据页面类型播放器指定索引的歌曲
    void onCurIndexToRecent(int index); //将当前播放的歌曲添加最近播放页面
    void onMuteChange(bool isMute); //处理是否静音
    void onVolumeChange(int value); //处理音量改变
    void onDurationChange(qint64 duration); //歌曲总时长改变处理
    void onPositionChange(qint64 position); //歌曲进度改变处理
    void onProgressBarChange(float position); //当进度条发生改变时
    void onMetaDataAvailableChanged(bool available); //当元数据发生改变时
    void showLyricsPage(); //显示动态歌词展示界面
    void quit(); //退出程序

    void on_skin_clicked();

    void on_max_clicked();

    void on_min_clicked();

protected:
    //重写鼠标按下和鼠标移动
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Ui::MusicPlayer *ui;
    QPoint distance; //鼠标按下坐标与程序窗口左上角坐标的距离
    VolumeControl* volumeControl; //音量调节窗口
    MusicList* musicList;
    QMediaPlayer* player; //媒体操作
    QMediaPlaylist* playList; //存储播放媒体
    Common* page; //现在播放列表所属那个页面
    qint64 totalDuration; //当前播放歌曲的总时长
    int curIndex; //当前播放歌曲的索引
    Lyrics* lyricsPage;
    QPropertyAnimation* lyricsAnimation; //歌词页面动画效果
    QSqlDatabase sqlite; //数据库操作对象
    bool isMove; //用来检测当前移动窗口是否合法
};
#endif // MUSICPLAYER_H
