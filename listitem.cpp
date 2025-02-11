#include "listitem.h"
#include "ui_listitem.h"

ListItem::ListItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListItem)
    , isLike(false)
{
    ui->setupUi(this);
    connect(ui->likeBtn, &QPushButton::clicked, this, &ListItem::onLikeBtnClicked);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::setName(const QString &name)
{
    ui->name->setText(name);
}

void ListItem::setSinger(const QString &singer)
{
    ui->singer->setText(singer);
}

void ListItem::setAlbum(const QString &album)
{
    ui->album->setText(album);
}

void ListItem::setIsLike(bool isLike)
{
    this->isLike = isLike;
    //根据是否喜欢设置对应的图标
    if(isLike) //喜欢
    {
        ui->likeBtn->setIcon(QIcon(":/images/selectLike.png"));
        ui->likeBtn->setIconSize(QSize(25,25));
    }
    else //不喜欢
    {
        ui->likeBtn->setIcon(QIcon(":/images/unselectLike.png"));
        ui->likeBtn->setIconSize(QSize(25,25));
    }
}

void ListItem::onLikeBtnClicked()
{
    isLike = !isLike;
    setIsLike(isLike);

    emit isLikeChange(isLike);
}
