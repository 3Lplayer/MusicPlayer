#include "button.h"
#include "ui_button.h"

button::button(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::button)
{
    ui->setupUi(this);

    line1Animation = new QPropertyAnimation(ui->line1, "geometry", this); //参数: 针对哪个对象设置动画效果,动画效果类型,父类
    line1Animation->setDuration(1200); //设置每轮动画的持续时间,单位毫秒
    line1Animation->setKeyValueAt(0, QRect(4, 25, 0, 0)); //设置起始关键帧,从最低点开始,QRect参数: 坐标x,坐标y,宽,高
    line1Animation->setKeyValueAt(0.5, QRect(4, 10, 2, 15)); //设置中间关键帧,从最低点到最高点
    line1Animation->setKeyValueAt(1, QRect(4, 25, 0, 0)); //设置结束关键帧,到最低点结束一轮
    line1Animation->setLoopCount(-1); //设置动画执行次数
    line1Animation->start();

    line2Animation = new QPropertyAnimation(ui->line2, "geometry", this); //参数: 针对哪个对象设置动画效果,动画效果类型,父类
    line2Animation->setDuration(1300); //设置每轮动画的持续时间,单位毫秒
    line2Animation->setKeyValueAt(0, QRect(10, 25, 0, 0)); //设置起始关键帧,从最低点开始,QRect参数: 坐标x,坐标y,宽,高
    line2Animation->setKeyValueAt(0.5, QRect(10, 10, 2, 15)); //设置中间关键帧,从最低点到最高点
    line2Animation->setKeyValueAt(1, QRect(10, 25, 0, 0)); //设置结束关键帧,到最低点结束一轮
    line2Animation->setLoopCount(-1); //设置动画执行次数
    line2Animation->start();

    line3Animation = new QPropertyAnimation(ui->line3, "geometry", this); //参数: 针对哪个对象设置动画效果,动画效果类型,父类
    line3Animation->setDuration(1400); //设置每轮动画的持续时间,单位毫秒
    line3Animation->setKeyValueAt(0, QRect(16, 25, 0, 0)); //设置起始关键帧,从最低点开始,QRect参数: 坐标x,坐标y,宽,高
    line3Animation->setKeyValueAt(0.5, QRect(16, 10, 2, 15)); //设置中间关键帧,从最低点到最高点
    line3Animation->setKeyValueAt(1, QRect(16, 25, 0, 0)); //设置结束关键帧,到最低点结束一轮
    line3Animation->setLoopCount(-1); //设置动画执行次数
    line3Animation->start();

    line4Animation = new QPropertyAnimation(ui->line4, "geometry", this); //参数: 针对哪个对象设置动画效果,动画效果类型,父类
    line4Animation->setDuration(1500); //设置每轮动画的持续时间,单位毫秒
    line4Animation->setKeyValueAt(0, QRect(22, 25, 0, 0)); //设置起始关键帧,从最低点开始,QRect参数: 坐标x,坐标y,宽,高
    line4Animation->setKeyValueAt(0.5, QRect(22, 10, 2, 15)); //设置中间关键帧,从最低点到最高点
    line4Animation->setKeyValueAt(1, QRect(22, 25, 0, 0)); //设置结束关键帧,到最低点结束一轮
    line4Animation->setLoopCount(-1); //设置动画执行次数
    line4Animation->start();



    ui->danceLine->hide(); //默认隐藏
}

button::~button()
{
    delete ui;
}

void button::setIcon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}

void button::setText(const QString &text)
{
    ui->text->setText(text);
}

void button::setID(int id)
{
    this->id = id;
}

int button::getID()
{
    return id;
}

void button::restoreBackgroundColor()
{
    ui->body->setStyleSheet(" #body { background-color: rgb(240,240,240); border-radius: 5px; } #body:hover { background-color: rgba(101,208,109,0.2); } ");
    ui->danceLine->hide();
}

void button::mousePressEvent(QMouseEvent *event)
{
    //当鼠标按下时改变背景颜色,表示被选中
    ui->body->setStyleSheet(" #body { background-color: rgba(101,208,109,0.65); border-radius: 5px; } ");
    ui->danceLine->show(); //按下选中后才出现动画效果

    //发送按下后的切换信号
    emit clicked(id);
}
