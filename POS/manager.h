#ifndef MANAGER_H
#define MANAGER_H

#include "uselogin.h"
#include "possystem.h"

/*管理的类，用于接受登录和注销登录的界面切换
 *2015-11-13 创建
 */

class Manager : public QObject
{
    Q_OBJECT
public:
    Manager();
    ~Manager();
private:
    UseLogin* m_useLogin;        //用户登录界面
    POSSystem* m_posSystem;      //主界面

    void init();                //初始化该类
public slots:
    void loginSuccess(int t);         //登陆成功,0：管理员
    void logoutSuccess();        //登出成功
};

#endif // MANAGER_H
