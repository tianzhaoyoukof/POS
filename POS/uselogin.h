#ifndef USELOGIN_H
#define USELOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QString>

/*员工或管理员登陆的类，密码和账号均正确才可登录
 *2015-11-13 创建
 */

namespace Ui {
class UseLogin;
}

class UseLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit UseLogin(QWidget *parent = 0);
    ~UseLogin();

private slots:
    void on_exit_Btn_clicked();

    void on_login_Btn_clicked();

    void on_showPassword_Btn_pressed();

    void on_showPassword_Btn_released();

private:
    Ui::UseLogin *ui;

    QSqlDatabase db;    //数据库

private:
    /*初始化类*/
    void initClass();

private:
    QString m_passwordString;   //密码储存的字符串

signals:
    void logined(int t);
    void setUserName(QString name);
};

#endif // USELOGIN_H
