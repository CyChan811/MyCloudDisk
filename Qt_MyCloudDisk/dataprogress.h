#ifndef DATAPROGRESS_H
#define DATAPROGRESS_H

#include <QWidget>


namespace Ui {
class dataprogress;
}

//�ϴ������ؽ��ȿؼ�
class dataprogress : public QWidget
{
    Q_OBJECT

public:
    explicit dataprogress(QWidget *parent = nullptr);
    ~dataprogress();

    void setFileName(QString name = "����"); //�����ļ�����
    void setProgress(int value = 0, int max = 100); //���ý������ĵ�ǰֵvalue, ���ֵmax

private:
    Ui::dataprogress *ui;
};

#endif // DATAPROGRESS_H
