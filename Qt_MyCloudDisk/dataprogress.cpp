#include "dataprogress.h"
#include "ui_dataprogress.h"

dataprogress::dataprogress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataprogress)
{
    ui->setupUi(this);
}

dataprogress::~dataprogress()
{
    delete ui;
}
//设置文件名字
void dataprogress::setFileName(QString name)
{
    ui->label->setText(name + " : ");
    ui->progressBar->setMinimum(0); //最小值
    ui->progressBar->setValue(0);
}
//设置进度条的当前值value, 最大值max
void dataprogress::setProgress(int value, int max)
{
    ui->progressBar->setValue(value);
    ui->progressBar->setMaximum(max);
}
