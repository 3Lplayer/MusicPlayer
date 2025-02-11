#include "playprogressbar.h"
#include "ui_playprogressbar.h"

PlayProgressBar::PlayProgressBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayProgressBar)
{
    ui->setupUi(this);
}

PlayProgressBar::~PlayProgressBar()
{
    delete ui;
}

void PlayProgressBar::mouseMoveEvent(QMouseEvent *event)
{
    QRect progressBarRect = QRect(0, 0, geometry().width(), geometry().height()); //获取当前进度条区域的矩形范围
    //只有当鼠标在progressBarRect区域内按住移动才有效
    if(progressBarRect.contains(event->pos()))
    {
        if(event->buttons() == Qt::LeftButton)
        {
            curPos = event->pos().x(); //更新当前的进度(x坐标)
            if(curPos < 0)
            {
                curPos = 0;
            }
            else if(curPos > ui->totalLine->width())
            {
                curPos = ui->totalLine->width();
            }
            updateProgress(curPos);
        }
    }
}

void PlayProgressBar::mousePressEvent(QMouseEvent *event)
{
    curPos = event->pos().x(); //更新当前的进度(x坐标)
    updateProgress(curPos);
}

void PlayProgressBar::mouseReleaseEvent(QMouseEvent *event)
{
    QRect progressBarRect = QRect(0, 0, geometry().width(), geometry().height()); //获取当前进度条区域的矩形范围
    //只有当鼠标在progressBarRect区域内释放才有效
    if(progressBarRect.contains(event->pos()))
    {
        curPos = event->pos().x(); //更新当前的进度(x坐标)
        updateProgress(curPos);
    }

    emit updateDuration((float)curPos/ui->totalLine->width());
}

void PlayProgressBar::updateProgress(int curPos)
{
    ui->moveLine->setGeometry(ui->moveLine->x(), ui->moveLine->y(), curPos, ui->moveLine->height());
}

void PlayProgressBar::setPosition(float position)
{
    curPos = ui->totalLine->width() * position;
    updateProgress(curPos);
}
