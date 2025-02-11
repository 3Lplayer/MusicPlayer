#include "common.h"
#include "ui_common.h"

Common::Common(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Common)
{
    ui->setupUi(this);
    ui->body->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //设置关闭水平滚动条

    //按下播放所有按钮,给主界面发送信号,让主界面处理
    connect(ui->playAllBtn, &QPushButton::clicked, this, [=](){
        emit playAllMusic(type);
    });

    //双击播放列表中的某一首歌曲,给主界面发送信号,让主界面处理
    connect(ui->body, &QListWidget::doubleClicked, this, [=](const QModelIndex& index){
        emit doubleClicked(this, index.row()); //传递本身和双击的行数过去
    });
}

Common::~Common()
{
    delete ui;
}

void Common::setFrontCover(const QString &path)
{
    ui->frontCover->setPixmap(QPixmap(path));
    ui->frontCover->setScaledContents(true); //设置自适应图片尺寸
}

void Common::setTitle(const QString &title)
{
    ui->title->setText(title);
}

void Common::setType(PageType type)
{
    this->type = type;
}

void Common::addSongs(const MusicList &list)
{
    songs.clear();
    //遍历获取相应类型的歌曲
    for(const auto& music : list.musicList)
    {
        //我喜欢类型
        if(music.isLike && type == PageType::LIKE)
        {
            songs.push_back(music.id);
        }
        //本地音乐类型,直接添加
        if(type == PageType::LOCAL)
        {
            songs.push_back(music.id);
        }
        //最近播放类型
        if(music.isRecent && type == PageType::RECENT)
        {
            songs.push_back(music.id);
        }
    }
}

void Common::updateSongs(MusicList &list)
{
    ui->body->clear();
    addSongs(list); //先获取自身的歌曲
    for(const auto& id : songs)
    {
        auto music = list.getMusicByID(id);

        //构造添加歌曲对象
        ListItem* listItem = new ListItem(this);
        listItem->setName(music->name);
        listItem->setSinger(music->singer);
        listItem->setAlbum(music->album);
        listItem->setIsLike(music->isLike);

        QListWidgetItem* item = new QListWidgetItem(ui->body);
        item->setSizeHint(QSize(listItem->width(), listItem->height()));
        ui->body->setItemWidget(item, listItem);

        //接收是否喜欢按钮按下发出的信号
        connect(listItem, &ListItem::isLikeChange, this, [=](bool isLike){
            emit updateILikePage(isLike, id); //发送更新我喜欢页面的歌曲信息
        });
    }
    //update(); //非立即重绘界面,而是将刷新请求存到队列中,排队处理
    repaint(); //立即重绘界面
}

void Common::addMusicToPlaylist(MusicList &musicList, QMediaPlaylist *playlist)
{
    //遍历添加歌曲的url
    for(const auto& id : songs)
    {
        playlist->addMedia(musicList.getMusicByID(id)->url);
    }
}

QString Common::getMusicIDByIndex(int index)
{
    return songs[index];
}

void Common::setFrontCover(QPixmap pixmap)
{
    ui->frontCover->setPixmap(pixmap);
    ui->frontCover->setScaledContents(true); //设置图片自适应尺寸
}
