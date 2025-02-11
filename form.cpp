#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::initRecommendUI(const QJsonArray &array, int row)
{
    jsonArray = array;
    if(row == 2)
    {
        this->row = row;
        this->col = 8;
    }
    else //只显示一行就隐藏最底下的哪一行
    {
        ui->listDown->hide();
    }

    total = jsonArray.size() / col;

    addItem();
}

void Form::addItem()
{
    //每次调用该函数先清空当前已经存在的item
    QList<item*> upList = ui->listUp->findChildren<item*>();
    for(auto e : upList)
    {
        ui->upHLayout->removeWidget(e);
        delete e;
    }

    QList<item*> downList = ui->listDown->findChildren<item*>();
    for(auto e : downList)
    {
        ui->downHLayout->removeWidget(e);
        delete e;
    }

    for(int i = curIndex * col, j = 0; i < col + (curIndex * col); ++i, ++j)
    {
        item* it = new item(this);
        QJsonObject jsonObj(jsonArray[i].toObject());
        it->setImage(jsonObj.value("path").toString());
        it->setInfo(jsonObj.value("info").toString());

        if(row == 2 && j >= col / 2) //有两层,添加第二层
        {
            ui->downHLayout->addWidget(it);
        }
        else //只有一层
        {
            ui->upHLayout->addWidget(it);
        }
    }
}

void Form::on_rightBtn_clicked()
{
    curIndex++;
    if(curIndex >= total) //超过最大组
    {
        curIndex = 0;
    }
    addItem();
}


void Form::on_leftBtn_clicked()
{
    curIndex--;
    if(curIndex < 0) //小于最小组
    {
        curIndex = total - 1;
    }
    addItem();
}

