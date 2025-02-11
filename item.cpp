#include "item.h"
#include "ui_item.h"

item::item(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::item)
{
    ui->setupUi(this);
    ui->btn->installEventFilter(this); //安装
}

item::~item()
{
    delete ui;
}

void item::setInfo(const QString &info)
{
    ui->info->setText(info);
}

void item::setImage(const QString &path)
{
    // QString style = "background-image: url(" + path + "); background-size: contain;";
    // ui->image->setStyleSheet(style);

    QPixmap pixmap(path);
     ui->image->setPixmap(pixmap.scaled(ui->image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

bool item::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->btn)
    {
        if(event->type() == QEvent::Enter) //鼠标进入
        {
            QPropertyAnimation* animation = new QPropertyAnimation(ui->body, "geometry");
            animation->setDuration(200);
            animation->setStartValue(QRect(9, 9, ui->body->width(), ui->body->height()));
            animation->setEndValue(QRect(9, 0, ui->body->width(), ui->body->height()));
            animation->start();
            //处理动画执行完的回收工作
            connect(animation, &QPropertyAnimation::finished, this, [=](){
                delete animation;
            });
        }
        else if(event->type() == QEvent::Leave) //鼠标离开
        {
            QPropertyAnimation* animation = new QPropertyAnimation(ui->body, "geometry");
            animation->setDuration(200);
            animation->setStartValue(QRect(9, 0, ui->body->width(), ui->body->height()));
            animation->setEndValue(QRect(9, 9, ui->body->width(), ui->body->height()));
            animation->start();
            connect(animation, &QPropertyAnimation::finished, this, [=](){
                delete animation;
            });
        }
    }
    return QObject::eventFilter(watched, event);
}
