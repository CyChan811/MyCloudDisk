#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // ��ʾ������
    void showMainWindow();
    // �����ź�
    void managerSignals();
    // ���µ�½
    void loginAgain();

protected:
    void paintEvent(QPaintEvent *event);

signals:
    // �л��û���ť�ź�
    void changeUser();
    void setUser(QString user);

private:
    Ui::MainWindow *ui;

    Common m_common;
};
#endif // MAINWINDOW_H
