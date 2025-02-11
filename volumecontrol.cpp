#include "volumecontrol.h"
#include "ui_volumecontrol.h"

VolumeControl::VolumeControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VolumeControl)
{
    ui->setupUi(this);

    //Qt::Popup: 设置窗口为按下出现,点击其他部分退出
    //Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint: 用于设置不显示最底下的QWidet
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    this->setAttribute(Qt::WA_TranslucentBackground); //设置最底下的QWidget为透明的

    //设置控件的阴影效果
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor("black"); //设置阴影颜色
    shadowEffect->setBlurRadius(10); //设置阴影显示半径范围
    shadowEffect->setOffset(0, 0); //设置阴影显示位置
    this->setGraphicsEffect(shadowEffect);

    //设置静音按钮
    ui->muteBtn->setIcon(QIcon(":/images/muteBtn.png"));
    ui->muteBtn->setIconSize(QSize(25,25));

    ui->volume->setText("20%"); //默认音量

    //设置moveLine的默认位置
    QRect rect = ui->moveLine->geometry();
    ui->moveLine->setGeometry(rect.x(), 185 - 37 + 15, rect.width(), 37); //37是总长度的20%,15是距离moveLine距离顶部的间距

    //移动btn的位置,-ui->btn->height()/2是因为要让btn的圆心对齐moveLine的顶部
    ui->btn->move(ui->btn->x(), ui->moveLine->y() - ui->btn->height() / 2);
    connect(ui->muteBtn, &QPushButton::clicked, this, &VolumeControl::onMuteBtnClicked);

    ui->widget->installEventFilter(this);
}

VolumeControl::~VolumeControl()
{
    delete ui;
}

void VolumeControl::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen); //设置无边框
    painter.setBrush(QBrush(Qt::white));

    QPolygon polyGon;
    //确定三角形的三个点
    QPoint a(20,280);
    QPoint b(40,280);
    QPoint c(30,290);
    polyGon.append(a);
    polyGon.append(b);
    polyGon.append(c);

    painter.drawPolygon(polyGon); //绘制三角形
}

bool VolumeControl::eventFilter(QObject *watched, QEvent *event)
{
    //判断事件是否在指定区域内发生的
    if(ui->widget == watched)
    {
        //判断触发事件是否是指定事件
        if(event->type() == QEvent::MouseButtonPress) //鼠标按下
        {
            calcVolumeValue();
        }
        else if(event->type() == QEvent::MouseButtonRelease) //鼠标释放
        {
            emit volumeValueChange(volumeValue); //发送音量改变信号
        }
        else if(event->type() == QEvent::MouseMove) //鼠标移动
        {
            calcVolumeValue();
            emit volumeValueChange(volumeValue); //发送音量改变信号
        }
        return true; //表示以上事件我自己处理
    }
    return QObject::eventFilter(watched, event); //如果不是以上事件交由父类处理
}

void VolumeControl::onMuteBtnClicked()
{
    isMute = !isMute; //更新是否静音的值
    //切换图标
    if(isMute) //是静音
    {
        ui->muteBtn->setIcon(QIcon(":/images/mute.png"));
        ui->muteBtn->setIconSize(QSize(25,25));
    }
    else //非静音
    {
        ui->muteBtn->setIcon(QIcon(":/images/muteBtn.png"));
        ui->muteBtn->setIconSize(QSize(25,25));
    }

    emit isMuteChange(isMute); //给主界面发送是否静音变化通知
}

void VolumeControl::calcVolumeValue()
{
    //获取鼠标按下在widget里的高度
    int height = ui->widget->mapFromGlobal(QCursor().pos()).y();

    //判断高度是否合法,不合法更正
    if(height < 15)
    {
        height = 15;
    }
    if(height > 200)
    {
        height = 200;
    }

    //调整音量条的位置
    ui->moveLine->setGeometry(ui->moveLine->x(), height, ui->moveLine->width(), 200 - height);

    //移动小球
    ui->btn->move(ui->btn->x(), height - ui->btn->height() / 2);

    //计算当前音量值
    volumeValue = (ui->moveLine->height() / (float)185) * 100;

    //设置百分比
    ui->volume->setText(QString::number(volumeValue) + "%");
}

















