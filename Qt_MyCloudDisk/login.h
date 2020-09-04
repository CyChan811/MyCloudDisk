#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "common/common.h"
#include "common/logininfoinstance.h"
#include "mainwindow.h"
#include "common/des.h"


namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
protected:
    //��ͼ�¼�����
    void paintEvent(QPaintEvent *event);

private slots:

    void on_reg_btn_clicked();

    void on_set_btn_clicked();

    void on_login_btn_clicked();


private:
    //��ȡ�����ļ���Ϣ
    //QString getCfgValue(QString title, QString key, QString path = CONFFILE);
    //���������ļ���Ϣ
    //void saveWebInfo(QString ip, QString port, QString path = CONFFILE);

    //ע����Ϣ->json
    QByteArray getRegJson(QString user, QString name, QString pwd, QString phone, QString mail);

    //��¼�û���Ҫʹ�õ�json���ݰ�
    QByteArray setLoginJson(QString user, QString pwd);

    //�õ��������ظ��ĵ�¼״̬��״̬�뷵��ֵΪ"000"����"001"�� ���е�¼section
    QStringList getLoginStatus(QByteArray json);

    //�س���ȷ������
    void keyPressEvent(QKeyEvent *event);

    //��ʼ����¼����ĺ���

    // ��ȡ������Ϣ������Ĭ�ϵ�¼״̬��Ĭ��������Ϣ
    void readCfg();


private:
    static QString      m_typePath;
    Ui::Login *ui;
    Common m_cm;
    // �����������������
    QNetworkAccessManager* m_manager;
    // ������ָ��
    MainWindow* m_mainWin;
};

#endif // LOGIN_H
