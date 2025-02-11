#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPainter>

namespace Ui {
class VolumeControl;
}

class VolumeControl : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeControl(QWidget *parent = nullptr);
    ~VolumeControl();

protected:
    void paintEvent(QPaintEvent* event) override; //用于绘制音量调节下面的倒三角
    bool eventFilter(QObject* watched, QEvent* event) override;

    void onMuteBtnClicked(); //静音按钮按下
    void calcVolumeValue(); //计算出当前音量

private:
    Ui::VolumeControl *ui;
    bool isMute = false; //用于判断当前是否静音
    int volumeValue = 20; //音量值,默认20

signals:
    void isMuteChange(bool);
    void volumeValueChange(int);
};

#endif // VOLUMECONTROL_H
