#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>


QString Login::m_typePath = FILETYPEDIR;

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // 初始化
    // 网络请求（http）类
    m_manager = Common::getNetManager();

    // 此处无需指定父窗口
    m_mainWin = new MainWindow;
    // 窗口图标
    this->setWindowIcon(QIcon(":/MyImages/image/logo.png"));
    this->setWindowTitle("喵云盘");
    m_mainWin->setWindowIcon(QIcon(":/MyImages/image/logo.png"));
    m_mainWin->setWindowTitle("喵云盘");

    //窗口移到屏幕中间
    //m_cm.moveToCenter(this);
    //默认当前窗口为登录窗口
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //去边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    //设置当前窗口所有的字体
    this->setFont(QFont("宋体", 11, QFont::Bold, false));

    // 密码
    ui->login_pwd->setEchoMode(QLineEdit::Password);
    ui->reg_pwd->setEchoMode(QLineEdit::Password);
    ui->reg_pwd2->setEchoMode(QLineEdit::Password);
    // 当前显示的窗口
    ui->stackedWidget->setCurrentIndex(0);
    ui->login_user->setFocus();
    // 数据的格式提示
    ui->login_user->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_user->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_name->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->login_pwd->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_pwd->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_pwd2->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    // 读取配置文件信息，并初始化
    readCfg();
    // 加载图片信息 - 显示文件列表的时候用，在此初始化
    m_cm.getFileTypeList();

    //titlewidget信号处理
    connect(ui->title_wg, & TitleWg::showSetWg, [=]{
       ui->stackedWidget->setCurrentWidget(ui->set_page);
       ui->serv_addr->setFocus();
    });

    connect(ui->title_wg, &TitleWg::closeWindow, [=]
    {
        //判断当前stackedW显示的页面
        if(ui->stackedWidget->currentWidget() == ui->set_page)
        {
            //切换到登录
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //清空控件数据
            ui->serv_addr->clear();
            ui->serv_port->clear();
            ui->login_user->setFocus();

        }
        else if(ui->stackedWidget->currentWidget() == ui->reg_page)
        {
            //切换到登录
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //清空控件数据
            ui->reg_user->clear();
            ui->reg_name->clear();
            ui->reg_pwd->clear();
            ui->reg_pwd2->clear();
            ui->reg_phone->clear();
            ui->reg_mail->clear();
            ui->reg_user->setFocus();
        }
        else
        {
            this->close();
        }
    });

    //切换到注册窗口
    connect(ui->toolButton, &QToolButton::clicked, [=]{
        ui->stackedWidget->setCurrentWidget(ui->reg_page);
        ui->reg_user->setFocus();
    });

    //切换用户，重新登录
    connect(m_mainWin, &MainWindow::changeUser, [=]()
    {
        m_mainWin->hide();
        this->show();
    });

    //确定按钮与回车键绑定
//    ui->login_btn->setShortcut(Qt::Key_Enter);
//    ui->login_btn->setShortcut(Qt::Key_Return);
//    ui->set_btn->setShortcut(Qt::Key_Enter);
//    ui->set_btn->setShortcut(Qt::Key_Return);
//    ui->reg_btn->setShortcut(Qt::Key_Enter);
//    ui->reg_btn->setShortcut(Qt::Key_Return);

}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *event)
{
    //给窗口画背景图
    QPainter p(this);
    QPixmap pixmap(":/MyImages/image/bg.jpg");
    p.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

//注册
void Login::on_reg_btn_clicked()
{
    //取数据
    QString ip = ui->serv_addr->text();
    QString port = ui->serv_port->text();
    //控件数据
    QString user = ui->reg_user->text();
    QString name = ui->reg_name->text();
    QString pwd = ui->reg_pwd->text();
    QString pwd2 = ui->reg_pwd2->text();
    QString phone = ui->reg_phone->text();
    QString mail = ui->reg_mail->text();
    //校验
    //user数据校验
    QRegExp exp(USER_REG);
    if(!exp.exactMatch(user))
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        ui->reg_user->clear();
        //控件设置焦点
        ui->reg_user->setFocus();
        return;
    }

    //pwd数据校验
    exp.setPattern(PASSWD_REG);
    if(!exp.exactMatch(pwd))
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->reg_pwd->clear();
        //控件设置焦点
        ui->reg_pwd->setFocus();
        return;
    }

    //判断前后密码是否相同
    if(QString::compare(pwd, pwd2) != 0)
    {
        QMessageBox::warning(this, "警告", "前后密码不一致");
        ui->reg_pwd->clear();
        ui->reg_pwd2->clear();
        //控件设置焦点
        ui->reg_pwd2->setFocus();
        return;
    }

    //pwd数据校验
    exp.setPattern(PHONE_REG);
    if(!exp.exactMatch(phone))
    {
        QMessageBox::warning(this, "警告", "手机格式不正确");
        ui->reg_phone->clear();
        //控件设置焦点
        ui->reg_phone->setFocus();
        return;
    }

    //pwd数据校验
    exp.setPattern(EMAIL_REG);
    if(!exp.exactMatch(mail))
    {
        QMessageBox::warning(this, "警告", "邮箱格式不正确");
        ui->reg_mail->clear();
        //控件设置焦点
        ui->reg_mail->setFocus();
        return;
    }
    //需要知道发给server端什么数据
    //将用户的注册信息转换成json对象
    QByteArray postData = getRegJson(user, name, pwd, phone, mail);

    //发送http请求协议
    //QNetworkAccessManager* manager = Common::getNetManager();
    //http请求头协议
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.size());

    //url
    QString url = QString("http://%1:%2/reg").arg(ip).arg(port);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = m_manager->post(request, postData);

    //接受server返回的数据
    connect(reply, &QNetworkReply::readyRead, [=]()
    {
        //读返回的数据
        //成功 {"code":"002"}
        //该用户存在 {"code":"003"}
        //失败 {"code":"004"}
        QByteArray jsonData = reply->readAll();

        //解析json字符串
        QString status = m_cm.getCode(jsonData);
        if(status == "002")
        {
            //success
            QMessageBox::information(this, "提示", "恭喜注册成功！");

            //将当前注册的用户信息填写到登录的输入框中
            ui->login_user->setText(user);
            ui->login_pwd->setText(pwd);

            //当前注册信息清除
            ui->reg_user->clear();
            ui->reg_name->clear();
            ui->reg_pwd->clear();
            ui->reg_pwd2->clear();
            ui->reg_phone->clear();
            ui->reg_mail->clear();

            //跳转到登录页面
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }
        else if(status == "003")
        {
            //用户已存在
            QMessageBox::warning(this, "警告", "对不起，当前用户已存在");
            return;
        }
        else if(status == "004")
        {
            //fail
            QMessageBox::critical(this, "警告", "注册失败");
            return;
        }
    });

}

//服务器设置
void Login::on_set_btn_clicked()
{
    //获取控件数据
    QString ip = ui->serv_addr->text();
    QString port = ui->serv_port->text();
    //IP数据校验
    QRegExp exp(IP_REG);
    if(!exp.exactMatch(ip))
    {
        QMessageBox::warning(this, "警告", "IP格式不正确");
        ui->serv_addr->clear();
        //控件设置焦点
        ui->serv_addr->setFocus();
        return;
    }
    //端口校验
    exp.setPattern(PORT_REG);
    if(!exp.exactMatch(port))
    {
        QMessageBox::warning(this, "警告", "端口格式不正确");
        ui->serv_port->clear();
        //控件设置焦点
        ui->serv_port->setFocus();
        return;
    }
    QMessageBox::information(this, "提示", "网络设置成功");
    //配置完成后跳转到登录界面
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //保存到配置文件
    Login::m_cm.writeWebInfo(ip, port);
}


//将用户的注册信息转换成json对象
QByteArray Login::getRegJson(QString user, QString name, QString pwd, QString phone, QString mail)
{
/*
{
    userName:xxxx,
    nickName:xxxx,
    firstPwd:xxxx,
    phone:xxxx,
    email:xxxx
}
*/
    QJsonObject obj;
    obj.insert("userName", user);
    obj.insert("nickName", name);
    obj.insert("firstPwd", pwd);
    obj.insert("phone", phone);
    obj.insert("email", mail);

    //obj->jsondoc
    QJsonDocument doc = QJsonDocument(obj);
    //返回json字符串
    return doc.toJson();
}

QByteArray Login::setLoginJson(QString user, QString pwd)
{
    QMap<QString, QVariant> login;
    login.insert("user", user);
    login.insert("pwd", pwd);

    /*json格式如下
    {
        user:xxx,
        pwd:xxxx
    }*/

    QJsonDocument doc = QJsonDocument::fromVariant(login);
    if(doc.isNull())
    {
        cout << "jsonDocument.isNull()";
        return "";
    }

    return doc.toJson();
}

// 得到服务器回复的登陆状态， 状态码返回值为 "000", 或 "001"，还有登陆section
QStringList Login::getLoginStatus(QByteArray json)
{
    QJsonParseError error;
    QStringList list;

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(error.error == QJsonParseError::NoError)
    {
        if(doc.isNull() | doc.isEmpty())
        {
            cout << "doc.isNUll || doc.isEmpty";
            return list;
        }

        if(doc.isObject())
        {
            //取到最外层这个大对象
            QJsonObject obj = doc.object();
            cout << "服务器返回的数据" << obj;
            //状态码
            list.append(obj.value("code").toString());
            //登录token
            list.append(obj.value("token").toString());
        }
    }
    else
    {
        cout << "err = " << error.errorString();
    }
    return list;
}

//回车键确定函数
void Login::keyPressEvent(QKeyEvent *event)
{
    if(ui->stackedWidget->currentWidget() == ui->reg_page)
    {
        if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            this->on_reg_btn_clicked();
    }
    else if(ui->stackedWidget->currentWidget() == ui->login_page)
    {
        if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            this->on_login_btn_clicked();
    }
    else if(ui->stackedWidget->currentWidget() == ui->set_page)
    {
        if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            this->on_set_btn_clicked();
    }
}

void Login::on_login_btn_clicked()
{
    //获取用户登录信息
    QString user = ui->login_user->text();
    QString pwd = ui->login_pwd->text();
    QString ip = ui->serv_addr->text();
    QString port = ui->serv_port->text();

    //数据校验
    QRegExp regexp(USER_REG);
    if(!regexp.exactMatch(user))
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        ui->login_user->clear();
        ui->login_user->setFocus();
        return;
    }

    regexp.setPattern(PASSWD_REG);
    if(!regexp.exactMatch(pwd))
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->login_pwd->clear();
        ui->login_pwd->setFocus();
        return;
    }

    //登录信息写入配置文件cfg.json
    //登录信息加密
    m_cm.writeLoginInfo(user, pwd, ui->remember_pwd->isChecked());
    //设置登录信息json包，密码经过md5加密，getStrMd5()
    QByteArray array = setLoginJson(user, m_cm.getStrMd5(pwd));

    //设置登录的url
    QNetworkRequest request;
    QString url = QString("http://%1:%2/login").arg(ip).arg(port);
    request.setUrl(QUrl(url));

    //请求头信息
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(array.size()));

    //向服务器发送post请求
    QNetworkReply* reply = m_manager->post(request, array);
    cout << "post url:" << url << "post data:" << array;

    //接受服务器发回的http响应消息
    connect(reply, &QNetworkReply::finished, [=]()
    {
       //出错了
        if(reply->error() != QNetworkReply::NoError)
        {
            cout << reply->errorString();
            //释放资源
            reply->deleteLater();
            return;
        }

        //将server回写的数据读出
        QByteArray json = reply->readAll();
        /*
        登录 - 服务器回写的数据包格式
        成功：{"code":"000"}
        失败：{"code":"001"}
        */
        cout << "server return value:" << json;
        QStringList tmpList = getLoginStatus(json);
        if(tmpList.at(0) == "000")
        {
            cout << "登录成功";

            //设置登录信息，显示文件列表界面需要使用这些信息
            LoginInfoInstance *p = LoginInfoInstance::getInstance();    //单例模式
            p->setLoginInfo(user, ip, port, tmpList.at(1));
            cout << p->getUser().toUtf8().data() << "," << p->getIp() << "," << p->getPort() << tmpList.at(1);

            //QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登录成功"));
            //当前窗口隐藏
            this->hide();
            //主界面窗口显示
            m_mainWin->showMainWindow();
        }
        else
        {
            QMessageBox::warning(this, "警告", "用户名或密码不正确");
        }
        reply->deleteLater();   //释放资源
    });

}

// 读取配置信息，设置默认登录状态，默认设置信息
void Login::readCfg()
{
    QString user = m_cm.getCfgValue("login", "user");
    QString pwd = m_cm.getCfgValue("login", "pwd");
    QString remeber = m_cm.getCfgValue("login", "remember");
    int ret = 0;

    if(remeber == "yes")//记住密码
    {
        //密码解密
        unsigned char encPwd[512] = {0};
        int encPwdLen = 0;
        //toLocal8Bit(), 转换为本地字符集，默认windows则为gbk编码，linux为utf-8编码
        QByteArray tmp = QByteArray::fromBase64( pwd.toLocal8Bit());  //先Base64解密
        ret = DesDec( (unsigned char *)tmp.data(), tmp.size(), encPwd, &encPwdLen); //后编码解密
        if(ret != 0)
        {
            cout << "DesDec";
            return;
        }

    #ifdef _WIN32 //如果是windows平台
        //fromLocal8Bit(), 本地字符集转换为utf-8
        ui->login_pwd->setText( QString::fromLocal8Bit( (const char *)encPwd, encPwdLen ) );
    #else //其它平台
        ui->login_pwd->setText( (const char *)encPwd );
    #endif

        ui->remember_pwd->setChecked(true);

    }
    else //没有记住密码
    {
        ui->login_pwd->setText("");
        ui->remember_pwd->setChecked(false);
    }

    //用户解密
    unsigned char encUsr[512] = {0};
    int encUsrLen = 0;
    //toLocal8Bit(), 转换为本地字符集，如果windows则为gbk编码，如果linux则为utf-8编码
    QByteArray tmp = QByteArray::fromBase64( user.toLocal8Bit()); //先Base64解密
    ret = DesDec( (unsigned char *)tmp.data(), tmp.size(), encUsr, &encUsrLen); //再编码解密
    if(ret != 0)
    {
        cout << "DesDec";
        return;
    }

    #ifdef _WIN32 //如果是windows平台
        //fromLocal8Bit(), 本地字符集转换为utf-8
        ui->login_user->setText( QString::fromLocal8Bit( (const char *)encUsr, encUsrLen ) );
    #else //其它平台
        ui->login_user->setText( (const char *)encUsr );
    #endif

    QString ip = m_cm.getCfgValue("web_server", "ip");
    QString port = m_cm.getCfgValue("web_server", "port");
    ui->serv_addr->setText(ip);
    ui->serv_port->setText(port);
}


