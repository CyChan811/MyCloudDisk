#ifndef DATAPROGRESS_H
#define DATAPROGRESS_H

#include <QWidget>


namespace Ui {
class dataprogress;
}

//上传、下载进度控件
class dataprogress : public QWidget
{
    Q_OBJECT

public:
    explicit dataprogress(QWidget *parent = nullptr);
    ~dataprogress();

    void setFileName(QString name = "测试"); //设置文件名字
    void setProgress(int value = 0, int max = 100); //设置进度条的当前值value, 最大值max

private:
    Ui::dataprogress *ui;
};

#endif // DATAPROGRESS_H
