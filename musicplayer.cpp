#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    initUI();
    initMedia();
    initDatabase();
    loadMusicData();
    initSignalSlot();
    srand(time(nullptr)); //设置随机数种子

    ui->goodRecommendList->initRecommendUI(randomImage(), 1);
    ui->recommendSongsList->initRecommendUI(randomImage(), 2);

    //设置页面的标题和封面和类型
    ui->likePage->setTitle("我喜欢");
    ui->likePage->setFrontCover(":/images/ilikePage.png");
    ui->likePage->setType(PageType::LIKE);
    ui->localPage->setTitle("本地音乐");
    ui->localPage->setFrontCover(":/images/localPage.png");
    ui->localPage->setType(PageType::LOCAL);
    ui->recentPage->setTitle("最近播放");
    ui->recentPage->setFrontCover(":/images/recentPage.png");
    ui->recentPage->setType(PageType::RECENT);
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

void MusicPlayer::initUI()
{
    this->setWindowFlag(Qt::FramelessWindowHint); //取消窗口标题栏
    this->setWindowIcon(QIcon(":/images/miniLogo.png")); //设置最小化显示图标

    //设置最小化系统托盘
    QSystemTrayIcon* systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/images/miniLogo.png"));
    systemTrayIcon->show();
    //设置右键的菜单
    QMenu* menu = new QMenu();
    menu->addAction("显示", this, &QWidget::showNormal);
    menu->addAction("退出", this, &MusicPlayer::quit);
    systemTrayIcon->setContextMenu(menu);

//    //设置窗口阴影效果(设置阴影效果会导致歌词展示动画重影)
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setColor("black"); //设置阴影颜色
//    shadowEffect->setOffset(0,0); //设置阴影偏移量
//    shadowEffect->setBlurRadius(10); //设置阴影扩散半径
//    this->setGraphicsEffect(shadowEffect);

    //设置自定义控件button的图标和文本
    ui->recommend->setIcon(":/images/recommend.png");
    ui->recommend->setText("推荐");
    ui->recommend->setID(0);
    ui->radio->setIcon(":/images/radio.png");
    ui->radio->setText("电台");
    ui->radio->setID(1);
    ui->musicHall->setIcon(":/images/musicHall.png");
    ui->musicHall->setText("音乐馆");
    ui->musicHall->setID(2);
    ui->like->setIcon(":/images/like.png");
    ui->like->setText("我喜欢");
    ui->like->setID(3);
    ui->local->setIcon(":/images/local.png");
    ui->local->setText("本地音乐");
    ui->local->setID(4);
    ui->recent->setIcon(":/images/recent.png");
    ui->recent->setText("最近播放");
    ui->recent->setID(5);

    ui->play->setIcon(QIcon(":/images/play.png")); //设置播放按钮的默认图片
    ui->play->setIconSize(QSize(50,50));

    ui->playMode->setIcon(QIcon(":/images/loop.png")); //设置播放模式按钮的默认图片
    ui->playMode->setIconSize(QSize(25,25));
    ui->playMode->setToolTip("列表循环");

    volumeControl = new VolumeControl(this);
    musicList = new MusicList();

    ui->lyrics->setIcon(QIcon(":/images/wordShow.png"));
    ui->lyrics->setIconSize(QSize(30,30));

    lyricsPage = new Lyrics(this);
    lyricsPage->hide(); //默认隐藏

    //设置歌词页面动画效果
    lyricsAnimation = new QPropertyAnimation(lyricsPage, "geometry", this);
    lyricsAnimation->setDuration(300);
    lyricsAnimation->setStartValue(QRect(0, lyricsPage->height(), lyricsPage->width(), lyricsPage->height()));
    lyricsAnimation->setEndValue(QRect(0, 0, lyricsPage->width(), lyricsPage->height()));
}

void MusicPlayer::initMedia()
{
    //实例化媒体相关对象
    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    page = ui->localPage;

    //设置播放列表的播放方式
    playList->setPlaybackMode(QMediaPlaylist::Loop); //循环播放

    //设置播放曲目列表
    player->setPlaylist(playList);

    //设置默认播放音量
    player->setVolume(20);

}

void MusicPlayer::initSignalSlot()
{
    connect(ui->recommend, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->radio, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->musicHall, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->like, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->local, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->recent, &button::clicked, this, &MusicPlayer::switchWindow);
    connect(ui->localPage, &Common::updateILikePage, this, &MusicPlayer::updatePageOfTypeLike);
    connect(ui->likePage, &Common::updateILikePage, this, &MusicPlayer::updatePageOfTypeLike);
    connect(ui->recentPage, &Common::updateILikePage, this, &MusicPlayer::updatePageOfTypeLike);
    connect(ui->play, &QPushButton::clicked, this, &MusicPlayer::onPlayClicked);
    connect(ui->playUp, &QPushButton::clicked, this, &MusicPlayer::onPlayUpClicked);
    connect(ui->playDown, &QPushButton::clicked, this, &MusicPlayer::onPlayDownClicked);
    connect(ui->playMode, &QPushButton::clicked, this, &MusicPlayer::onPlayModeClicked);
    connect(ui->likePage, &Common::playAllMusic, this, &MusicPlayer::onPlayAllMusicClicked);
    connect(ui->localPage, &Common::playAllMusic, this, &MusicPlayer::onPlayAllMusicClicked);
    connect(ui->recentPage, &Common::playAllMusic, this, &MusicPlayer::onPlayAllMusicClicked);
    connect(ui->likePage, &Common::doubleClicked, this, &MusicPlayer::onDoubleClicked);
    connect(ui->localPage, &Common::doubleClicked, this, &MusicPlayer::onDoubleClicked);
    connect(ui->recentPage, &Common::doubleClicked, this, &MusicPlayer::onDoubleClicked);
    connect(playList, &QMediaPlaylist::currentIndexChanged, this, &MusicPlayer::onCurIndexToRecent);
    connect(volumeControl, &VolumeControl::isMuteChange, this, &MusicPlayer::onMuteChange);
    connect(volumeControl, &VolumeControl::volumeValueChange, this, &MusicPlayer::onVolumeChange);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPlayer::onDurationChange);
    connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayer::onPositionChange);
    connect(ui->progressBar, &PlayProgressBar::updateDuration, this, &MusicPlayer::onProgressBarChange);
    connect(player, &QMediaPlayer::metaDataAvailableChanged, this, &MusicPlayer::onMetaDataAvailableChanged);
    connect(ui->lyrics, &QPushButton::clicked, this, &MusicPlayer::showLyricsPage);
}

void MusicPlayer::initDatabase()
{
    //设置数据库驱动
    sqlite = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    sqlite.setDatabaseName("MusicPlayer.db");
    //打开数据库
    if(!sqlite.open()) //打开失败
    {
        QMessageBox::critical(this, "错误", "SQLite数据库打开失败.");
        return ;
    }
    qDebug() << "数据库打开成功.";
    //创建存储表
    QString sql = "create table if not exists musicData(\
                   id integer primary key autoincrement,\
                   musicID varchar(64) unique,\
                   name varchar(64),\
                   singer varchar(64),\
                   album varchar(64),\
                   duration bigint,\
                   url varchar(64),\
                   isLike integer,\
                   isRecent integer)";
    QSqlQuery query;
    if(!query.exec(sql))
    {
        QMessageBox::critical(this, "错误","创建数据表失败.");
        return ;
    }
    qDebug() << "数据表创建成功.";
}

void MusicPlayer::loadMusicData()
{
    musicList->loadFromDatabase(); //加载歌曲数据
    //更新页面显示
    ui->likePage->setType(PageType::LIKE);
    ui->likePage->updateSongs(*musicList);
    ui->localPage->setType(PageType::LOCAL);
    ui->localPage->updateSongs(*musicList);
    ui->recentPage->setType(PageType::RECENT);
    ui->recentPage->updateSongs(*musicList);

}

QJsonArray MusicPlayer::randomImage()
{
    QVector<QString> images;
    for(int i = 1; i <= 40; ++i)
    {
        QString name = QString::number(i) + ".png";
        images << name;
    }

    std::random_shuffle(images.begin(), images.end()); //随机打散images中的图片的顺序

    //组织图片路径与信息的映射
    QJsonArray jsonArray;
    for(int i = 0; i < images.size(); ++i)
    {
        QJsonObject jsonObj;
        jsonObj.insert("path", ":/images/" + images[i]);
        jsonObj.insert("info", QString("推荐-%1").arg(i + 1, 3, 10, QChar('0'))); //%1用i来替换,位宽为3位,十进制整数显示,不足三位用0补充
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void MusicPlayer::playTypePageMusic(Common *page, int index)
{
    this->page = page; //更新播放列表所属页面
    //切换播放按钮
    ui->play->setIcon(QIcon(":/images/pause.png")); //设置播放按钮的默认图片
    ui->play->setIconSize(QSize(45,45));

    playList->clear(); //清空原本的播放列表
    page->addMusicToPlaylist(*musicList, playList); //添加page的播放列表
    playList->setCurrentIndex(index); //设置播放曲目从第一首开始
    player->play();
}

void MusicPlayer::on_quit_clicked()
{
    this->hide(); //最小化到系统托盘
}

void MusicPlayer::switchWindow(int id)
{
    //未被按下的按钮背景颜色变回原来的颜色
    QList<button*> list = this->findChildren<button*>(); //找到所有button*类型的控件
    for(auto& item : list) //将id不匹配的按钮的背景颜色变回原来的原色
    {
        if(item->getID() != id)
        {
            item->restoreBackgroundColor();
        }
    }

    //切换当前按下按钮所对应的层叠窗口
    ui->detailsWindow->setCurrentIndex(id);
    isMove = false;
}

void MusicPlayer::mousePressEvent(QMouseEvent *event)
{
    //判断是否是左键按下
    if(event->button() == Qt::LeftButton)
    {
        //event->globalPos(): 鼠标相对于电脑屏幕左上角的坐标
        distance = event->globalPos() - this->geometry().topLeft(); //获取相差距离
    }
    isMove = true;
}

void MusicPlayer::mouseMoveEvent(QMouseEvent *event)
{
    //判断是否是左键移动
    if(event->buttons() == Qt::LeftButton && isMove)
    {
        this->move(event->globalPos() - distance); //窗口移动
    }
}


void MusicPlayer::on_voice_clicked()
{
    //获取音量调节按钮基于整个屏幕的自身的左上角的坐标
    QPoint point = ui->addMusic->mapToGlobal(QPoint(0,0));

    //计算弹出的调节音量控件以整个屏幕为起点的显示坐标
    QPoint showPoint = point - QPoint(volumeControl->width(), volumeControl->height());

    showPoint.setX(showPoint.x() + 5);
    //更新位置
    volumeControl->move(showPoint);
    volumeControl->show();
}


void MusicPlayer::on_addMusic_clicked()
{
    //创建对话框
    QFileDialog dialog(this);
    dialog.setWindowTitle("加载本地音乐"); //设置对话框图标

    //设置默认打开路径
    QDir dir(QDir::currentPath());
    dir.cdUp(); //相当于cd..
    QString path = dir.path() + "/MusicPlayer/music";
    dialog.setDirectory(path);

    //设置打开方式为一次可以选择多个或一个文件
    dialog.setFileMode(QFileDialog::ExistingFiles);

    //设置文件过滤
    //dialog.setNameFilter("音乐文件(*.mp3 *.wav *.flac *.lrc)");
    QStringList mimeTypeFilters;
    mimeTypeFilters << "application/octet-stream";    // All(*)
    dialog.setMimeTypeFilters(mimeTypeFilters);

    //模态显示,意思是在弹出对话框后必须先退出或结束对话框的操作,在对话框存在期间不能操作其他任何区域
    if(QDialog::Accepted == dialog.exec()) //获取返回值
    {
        QList<QUrl> paths = dialog.selectedUrls(); //获取所选文件的路径
        musicList->setMusicList(paths);

        ui->detailsWindow->setCurrentIndex(4); //切换到本地音乐界面
        ui->local->mousePressEvent(nullptr);
        ui->localPage->updateSongs(*musicList); //更新显示歌曲
        ui->localPage->addMusicToPlaylist(*musicList, playList); //添加本地音乐的歌曲到播放列表
    }
}

void MusicPlayer::updatePageOfTypeLike(bool isLike, QString id)
{
    //先修改歌曲列表里面指定歌曲的是否喜欢
    auto music = musicList->getMusicByID(id);
    if(music) //判空
    {
       music->isLike = isLike;
    }

    //更新三个页面里的是否喜欢状态
    ui->likePage->updateSongs(*musicList);
    ui->localPage->updateSongs(*musicList);
    ui->recentPage->updateSongs(*musicList);
}

void MusicPlayer::onPlayClicked()
{
    //根据播放按钮当前的状态判定按下后触发的操作
    if(player->state() == QMediaPlayer::StoppedState || player->state() == QMediaPlayer::PausedState) //最开始的停止状态或暂停状态,按钮按下后都是开始播放
    {
        player->play();
        ui->play->setIcon(QIcon(":/images/pause.png")); //切换图标
        ui->play->setIconSize(QSize(45,45));
    }
    else if(player->state() == QMediaPlayer::PlayingState) //正在播放状态
    {
        player->pause();
        ui->play->setIcon(QIcon(":/images/play.png")); //切换图标
        ui->play->setIconSize(QSize(50,50));
    }
}

void MusicPlayer::onPlayUpClicked()
{
    playList->previous();
}

void MusicPlayer::onPlayDownClicked()
{
    playList->next();
}

void MusicPlayer::onPlayModeClicked()
{
    //播放模式切换顺序: 列表循环->随机播放->单曲循环
    if(playList->playbackMode() == QMediaPlaylist::Loop) //列表循环->随机播放
    {
        playList->setPlaybackMode(QMediaPlaylist::Random);
        ui->playMode->setToolTip("随机播放"); //设置鼠标悬停在按钮上的提示文本
        //切换图标
        ui->playMode->setIcon(QIcon(":/images/random.png"));
        ui->playMode->setIconSize(QSize(25,25));
    }
    else if(playList->playbackMode() == QMediaPlaylist::Random) //随机播放->单曲循环
    {
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->playMode->setToolTip("单曲循环"); //设置鼠标悬停在按钮上的提示文本
        //切换图标
        ui->playMode->setIcon(QIcon(":/images/curIndexLoop.png"));
        ui->playMode->setIconSize(QSize(25,25));
    }
    else if(playList->playbackMode() == QMediaPlaylist::CurrentItemInLoop) //单曲循环->列表循环
    {
        playList->setPlaybackMode(QMediaPlaylist::Loop);
        ui->playMode->setToolTip("列表循环"); //设置鼠标悬停在按钮上的提示文本
        //切换图标
        ui->playMode->setIcon(QIcon(":/images/loop.png"));
        ui->playMode->setIconSize(QSize(25,25));
    }
}

void MusicPlayer::onPlayAllMusicClicked(PageType pageType)
{
    Common* page = ui->localPage;
    //根据页面类型确定页面
    if(pageType == PageType::LIKE)
    {
        page = ui->likePage;
    }
    else if(pageType == PageType::RECENT)
    {
        page = ui->recentPage;
    }

    playTypePageMusic(page, 0);
}

void MusicPlayer::onDoubleClicked(Common *page, int index)
{
    playTypePageMusic(page, index);
}

void MusicPlayer::onCurIndexToRecent(int index)
{
    curIndex = index;
    //获取指定下标的歌曲id
    QString id = page->getMusicIDByIndex(index);
    //根据歌曲id获取指定歌曲
    Music* music = musicList->getMusicByID(id);
    music->isRecent = true; //修改最近播放属性
    ui->recentPage->updateSongs(*musicList); //更新最近播放页面
}

void MusicPlayer::onMuteChange(bool isMute)
{
    player->setMuted(isMute);
}

void MusicPlayer::onVolumeChange(int value)
{
    player->setVolume(value);
}

void MusicPlayer::onDurationChange(qint64 duration)
{
    totalDuration = duration;
    //arg参数: 填充的字符-填充几位-显示进制-不够位数用什么补充
    ui->totalTime->setText(QString("%1:%2").arg(duration/1000/60, 2, 10, QChar('0')).arg(duration/1000%60, 2, 10, QChar('0')));
}

void MusicPlayer::onPositionChange(qint64 position)
{
    ui->curTime->setText(QString("%1:%2").arg(position/1000/60, 2, 10, QChar('0')).arg(position/1000%60, 2, 10, QChar('0'))); //更新当前播放时间
    ui->progressBar->setPosition((float)position/totalDuration);
    lyricsPage->showLrcWord(position); //显示歌词
}

void MusicPlayer::onProgressBarChange(float position)
{
    qint64 duration = totalDuration * position;
    ui->curTime->setText(QString("%1:%2").arg(duration/1000/60, 2, 10, QChar('0')).arg(duration/1000%60, 2, 10, QChar('0')));

    player->setPosition(duration); //设置播放进度到当前进度条同比例位置
}

void MusicPlayer::onMetaDataAvailableChanged(bool available)
{
    QString id = page->getMusicIDByIndex(curIndex); //获取当前索引对应的歌曲id
    const Music* music = musicList->getMusicByID(id); //根据歌曲id获得歌曲指针

    //设置歌曲名和作者
    ui->songName->setText(music->name);
    ui->singer->setText(music->singer);

    //设置歌曲封面
    QVariant variant = player->metaData("ThumbnailImage");
    if(variant.isValid()) //判断是否存在
    {
        QImage image = variant.value<QImage>(); //转换为QImage
        ui->musicAvatar->setPixmap(QPixmap::fromImage(image));
        page->setFrontCover(QPixmap::fromImage(image));
    }
    else //不存在,设置默认图片
    {
        ui->musicAvatar->setPixmap(QString(":/images/defaultAvatar.png"));
        page->setFrontCover(QString(":/images/defaultAvatar.png"));
    }
    ui->musicAvatar->setScaledContents(true); //设置图片自适应尺寸

    //获取歌词文件路径
    QString lrcFilePath = music->getLrcFilePath();
    //解析歌词
    lyricsPage->parseLrcFile(lrcFilePath);
}

void MusicPlayer::showLyricsPage()
{
    lyricsPage->show();
    lyricsAnimation->start();
}

void MusicPlayer::quit()
{
    //持久化歌曲数据
    musicList->saveToDatabase();
    //关闭数据库
    sqlite.close();
    this->close();
}


void MusicPlayer::on_skin_clicked()
{
    QMessageBox::information(this, "温馨提示", "暂不支持换肤功能.");
}

void MusicPlayer::on_max_clicked()
{
    QMessageBox::information(this, "温馨提示", "暂不支持放大功能.");
}

void MusicPlayer::on_min_clicked()
{
    showMinimized();
}
