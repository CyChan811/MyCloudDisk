#ifndef TITLEWG_H
#define TITLEWG_H

#include <QWidget>

namespace Ui {
class TitleWg;
}

class TitleWg : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWg(QWidget *parent = nullptr);
    ~TitleWg();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    Ui::TitleWg *ui;
    QPoint m_pt;    //��ֵ����굱ǰλ��-�������Ͻ�
    QWidget* m_parent;

signals:
    void showSetWg();   //�źŲ���Ҫʵ�֣�ֻ��Ҫ��������
    void closeWindow();
};

#endif // TITLEWG_H
