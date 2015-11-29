#include "vipregister.h"
#include "ui_vipregister.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDateTime>

VIPRegister::VIPRegister(QWidget *parent) :
    QWidget(parent),
    m_maxVIPNo(20150001),
    ui(new Ui::VIPRegister)
{
    ui->setupUi(this);
    initVIP();
}

VIPRegister::~VIPRegister()
{
    delete ui;
}

void VIPRegister::initVIP()
{
    QSqlQuery query;
    query.exec("select * from VIP");
    while(query.next())
    {
        ++ m_maxVIPNo;   //只需要计算有多少条记录即可
    }
    ui->no_LEdit->setText(QString::number(m_maxVIPNo));
    /*设置为今天日期*/
    ui->date_LEdit->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
}

void VIPRegister::on_close_Btn_clicked()
{
    this->setHidden(true);
}

void VIPRegister::on_register_Btn_clicked()
{
    QString s = ui->name_LEdit->text();
    if(s == "")     //名字为空，注册失败
    {
        QMessageBox::information(this,"错误","会员名字不能为空");
        return ;
    }

    s = QString::number(m_maxVIPNo ++);

    QSqlQuery query;
    s = "insert VIP values(" + QString::number(m_maxVIPNo - 20150001)
            + ",'" + ui->name_LEdit->text()
            + "','"+ s +"',0,'"+ ui->date_LEdit->text() +"');";
    query.exec(s);
    if(!query.isValid())
    {
        QMessageBox::information(this,"提示","注册成功");
        ui->name_LEdit->setText("");
        ui->no_LEdit->setText(QString::number(m_maxVIPNo));
    }
}
