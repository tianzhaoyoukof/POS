#include "uselogin.h"
#include "ui_uselogin.h"
#include <QDebug>

#include<QSqlQueryModel>
#include<QSqlQuery>
#include<QTableView>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>

UseLogin::UseLogin(QWidget *parent) :
    QMainWindow(parent),
    m_passwordString(""),
    ui(new Ui::UseLogin)
{
    ui->setupUi(this);
    this->setWindowTitle("系统登录");
    initClass();
}

UseLogin::~UseLogin()
{
    delete ui;
}

void UseLogin::on_exit_Btn_clicked()
{
    db.close();
    exit(0);
}

void UseLogin::on_login_Btn_clicked()
{
    /*SQL查询*/
    bool flag = false;
    QSqlQuery query;
    query.exec("select * from LoginUser");
    while(query.next())
    {
        QString name = query.value(1).toString();
        name = name.simplified();
        QString pass = query.value(2).toString();
        pass = pass.simplified();
        QString dept = query.value(3).toString();
        dept = dept.simplified();

        if(name == ui->name_LEdit->text().simplified() && pass == ui->password_LEdit->text().simplified())
        {
            if(ui->admin_Radio->isChecked() && dept == "Admin")
            {
                qDebug()<<"登录成功";
                emit logined(0);
                emit setUserName(ui->name_LEdit->text());
                flag = true;
                break;
            }
            else if(ui->emp_Radio->isChecked() && dept == "Emp")
            {
                qDebug()<<"登录成功";
                emit logined(1);
                emit setUserName(ui->name_LEdit->text());
                flag = true;
                break;
            }
        }
    }
    if(!flag)
    {
        QMessageBox::information(this,"错误","账号或密码不对");
    }
}

void UseLogin::initClass()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("pos");
    db.setUserName("as");
    db.setPassword("123456");

    if (!db.open())
        qDebug()<<"数据库打开失败 &_&";
    else
        qDebug()<<"数据库连接成功！ ^_^";

    /*password_LEdit限制条件*/
    QRegExp reg("[a-zA-Z0-9]+$");
    QValidator *qValidator = new QRegExpValidator(reg,ui->password_LEdit);
    ui->password_LEdit->setValidator(qValidator);
    ui->password_LEdit->setEchoMode(QLineEdit::Password);

    /*name_LEdit限制条件*/
    ui->name_LEdit->setValidator(qValidator);
}

void UseLogin::on_showPassword_Btn_pressed()
{
    ui->password_LEdit->setEchoMode(QLineEdit::Normal);
}

void UseLogin::on_showPassword_Btn_released()
{
    ui->password_LEdit->setEchoMode(QLineEdit::Password);
}
