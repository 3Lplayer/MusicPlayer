#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPropertyAnimation>

namespace Ui {
class button;
}

class button : public QWidget
{
    Q_OBJECT

public:
    explicit button(QWidget *parent = nullptr);
    ~button();

    void setIcon(const QString& icon); //设置图标
    void setText(const QString& text); //设置文本
    void setID(int id); //初始化id
    int getID(); //获取id
    void restoreBackgroundColor(); //还原按下后的背景颜色

public:
    void mousePressEvent(QMouseEvent* event) override; //重写鼠标按下事件

private:
    Ui::button *ui;
    int id; //用来表示切换哪个层叠窗口
    //用来设置各标签的动画效果
    QPropertyAnimation* line1Animation;
    QPropertyAnimation* line2Animation;
    QPropertyAnimation* line3Animation;
    QPropertyAnimation* line4Animation;

signals:
    void clicked(int id); //当按下后发送切换信号
};

#endif // BUTTON_H
