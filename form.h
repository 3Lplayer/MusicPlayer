#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "item.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void initRecommendUI(const QJsonArray& array, int row);

private slots:
    void on_rightBtn_clicked();

    void on_leftBtn_clicked();

private:
    void addItem(); //添加推荐

private:
    Ui::Form *ui;
    int row = 1; //默认展示的行数
    int col = 4; //默认展示的列数

    int curIndex = 0; //当前展示的是所有图片的第几组
    int total; //一共有多少组

    QJsonArray jsonArray; //存储图片封面与信息
};

#endif // FORM_H
