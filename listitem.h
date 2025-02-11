#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    //设置各标签
    void setName(const QString& name);
    void setSinger(const QString& singer);
    void setAlbum(const QString& album);
    void setIsLike(bool isLike);

protected:
    void onLikeBtnClicked();

private:
    Ui::ListItem *ui;
    bool isLike; //判断是否是喜欢音乐

signals:
    void isLikeChange(bool); //是否喜欢改变
};

#endif // LISTITEM_H
