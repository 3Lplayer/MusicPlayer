#ifndef ITEM_H
#define ITEM_H

#include <QWidget>
#include <QDebug>
#include <QPropertyAnimation>

namespace Ui {
class item;
}

class item : public QWidget
{
    Q_OBJECT

public:
    explicit item(QWidget *parent = nullptr);
    ~item();

    void setInfo(const QString& info); //设置信息
    void setImage(const QString& path); //设置图片封面

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::item *ui;
};

#endif // ITEM_H
