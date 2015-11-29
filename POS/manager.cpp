#include "manager.h"
#include <QDebug>
//#include "purchaseform.h"


Manager::Manager():
    m_useLogin(NULL),
    m_posSystem(NULL)
{
    this->init();
}

Manager::~Manager()
{
    delete m_useLogin;
    delete m_posSystem;
}

void Manager::init()
{
    m_useLogin = new UseLogin();
    m_posSystem = new POSSystem();

    m_useLogin->show();
    m_posSystem->hide();
    m_posSystem->changeManager(true);

    connect(m_useLogin,SIGNAL(logined(int)),this,SLOT(loginSuccess(int)));
    connect(m_posSystem,SIGNAL(logouted()),this,SLOT(logoutSuccess()));

    connect(m_useLogin,SIGNAL(setUserName(QString)),m_posSystem,SLOT(setUserName(QString)));
}

void Manager::loginSuccess(int t)
{
    m_useLogin->hide();
    m_posSystem = new POSSystem();
    m_posSystem->show();
    connect(m_posSystem,SIGNAL(logouted()),this,SLOT(logoutSuccess()));
    connect(m_useLogin,SIGNAL(setUserName(QString)),m_posSystem,SLOT(setUserName(QString)));
    if(t == 0)
        m_posSystem->changeManager(true);
    else
        m_posSystem->changeManager(false);
    //qDebug()<<"LoginSuccess";
}

void Manager::logoutSuccess()
{
    m_useLogin->show();
    delete m_posSystem;
    //m_posSystem->hide();
}
