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
    //绘图事件函数
    void paintEvent(QPaintEvent *event);

private slots:

    void on_reg_btn_clicked();

    void on_set_btn_clicked();

    void on_login_btn_clicked();


private:
    //读取配置文件信息
    //QString getCfgValue(QString title, QString key, QString path = CONFFILE);
    //保存配置文件信息
    //void saveWebInfo(QString ip, QString port, QString path = CONFFILE);

    //注册信息->json
    QByteArray getRegJson(QString user, QString name, QString pwd, QString phone, QString mail);

    //登录用户需要使用的json数据包
    QByteArray setLoginJson(QString user, QString pwd);

    //得到服务器回复的登录状态，状态码返回值为"000"，或"001"， 还有登录section
    QStringList getLoginStatus(QByteArray json);

    //回车键确定函数
    void keyPressEvent(QKeyEvent *event);

    //初始化登录界面的函数

    // 读取配置信息，设置默认登录状态，默认设置信息
    void readCfg();


private:
    static QString      m_typePath;
    Ui::Login *ui;
    Common m_cm;
    // 处理网络请求类对象
    QNetworkAccessManager* m_manager;
    // 主窗口指针
    MainWindow* m_mainWin;
};

#endif // LOGIN_H
