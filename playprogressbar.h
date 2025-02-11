#ifndef PLAYPROGRESSBAR_H
#define PLAYPROGRESSBAR_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class PlayProgressBar;
}

class PlayProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit PlayProgressBar(QWidget *parent = nullptr);
    ~PlayProgressBar();

    void setPosition(float position); //设置进度条当前的进度

protected:
    void mouseMoveEvent(QMouseEvent *event) override; //重写鼠标移动事件
    void mousePressEvent(QMouseEvent *event) override; //重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent* event) override; //重写鼠标释放事件

    void updateProgress(int curPos); //更新歌曲播放进度

signals:
    void updateDuration(float position); //当进度条发生改变时当前播放时长也要随着改变

private:
    Ui::PlayProgressBar *ui;
    int curPos; //记录当前的进度
};

#endif // PLAYPROGRESSBAR_H
