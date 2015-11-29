#include "personform.h"
#include "ui_personform.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>
#include <QInputDialog>
PersonForm::PersonForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonForm),
    m_queryModel(NULL),
    m_numLoginUser(0)
{
    ui->setupUi(this);
    init();
}

PersonForm::~PersonForm()
{
    delete m_queryModel;
    delete ui;
}

void PersonForm::on_tabWidget_currentChanged(int index)
{
    /*0 初始化员工界面 initUser()
     *1 初始化会员界面 initVIP()
     */
    switch(index)
    {
    case 0:
        initUser();
        break;
    case 1:
        initVIP();
        break;
    }
}

void PersonForm::init()
{
    m_queryModel = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("select Max(ID) from LoginUser");
    query.next();
    m_numLoginUser = query.value(0).toInt();
    ++ m_numLoginUser;
    initUser();

}

void PersonForm::initUser()
{
    m_queryModel->setQuery("select ID as '编号',Name as '用户名',Dept as '权限',Password as '登录密码' from LoginUser where Name!='Admin'");
    ui->person_TView->setModel(m_queryModel);

    ui->Admin_RBtn->setChecked(false);
    ui->Emp_RBtn->setChecked(false);
}

void PersonForm::initVIP()
{
    /*重新从数据库选择，排序改变*/
    QString temp = "select ID as '编号',Name as '会员名',No as '会员卡号',Consume as '消费金额',VIPDate as '注册日期' from VIP ";
    if(ui->sortALL->currentIndex() == 1)
        temp += "order by Consume ASC";
    else if(ui->sortALL->currentIndex() == 2)
        temp += "order by Consume DESC";
    m_queryModel->setQuery(temp);
    ui->VIP_TView->setModel(m_queryModel);
}

void PersonForm::on_delVIP_Btn_clicked()
{
    /*检查鼠标所在的行数*/
    //qDebug()<<ui->VIP_TView->currentIndex().row();
    if(ui->VIP_TView->currentIndex().row() == -1)
        return ;

    switch(QMessageBox::question(this,tr("询问"),tr("是否注销该会员?"),
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*将ID记录下来,分别从列表和VIP表中删除该会员*/
    int ID = m_queryModel->record(ui->VIP_TView->currentIndex().row()).value(0).toInt();
    //qDebug()<<"name"<<name;
    QString temp = "delete from VIP where ID=" + QString::number(ID);
    QSqlQuery query;
    query.exec(temp);

    /*把大于该ID的所有会员ID-1*/
    temp = "update VIP set ID=ID-1 where ID>" + QString::number(ID);
    query.exec(temp);
    QMessageBox::information(this,"操作成功","该会员已注销");
    on_sort_Btn_clicked();
}

void PersonForm::on_sort_Btn_clicked()
{
    initVIP();
}

void PersonForm::on_changeDept_Btn_clicked()
{
    /*检查鼠标*/
    if(ui->person_TView->currentIndex().row() == -1)
        return ;

    QString name = ui->person_TView->model()->index(ui->person_TView->currentIndex().row(),1).data(0).toString();
    name = name.simplified();

    /*打印确认框*/
    QString temp = "是否将 " + name + " 权限\n更改为 :";
    if(ui->Admin_RBtn->isChecked())
        temp += "管理员?";
    else
        temp += "普通员工?";
    switch(QMessageBox::question(this,tr("询问"),temp,
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*从数据库修改*/
    QSqlQuery query;
    temp = "update LoginUser set Dept=";
    if(ui->Admin_RBtn->isChecked())
        temp += "'Admin'";
    else
        temp += "'Emp'";
    temp += " where Name='" + name + "'";
    query.exec(temp);

    initUser();
    QMessageBox::information(this,"操作成功","改变权限成功");
}

void PersonForm::on_addPreson_Btn_clicked()
{
    /*输入名字*/
    QString name = QInputDialog::getText(this,"增加用户","请输入用户名");
    name = name.simplified();
    if(name.simplified() == "")
    {
        QMessageBox::information(this,"增加用户失败","输入的用户名不能为空");
        return ;
    }
    /*检查名字是否存在*/
    QSqlQuery query;
    QString temp = "select * from LoginUser where Name='" + name + "'";
    query.exec(temp);
    if(query.first())
    {
        QMessageBox::information(this,"增加用户失败","用户名已存在");
        return ;
    }

    /*输入密码*/
    QString oldPass = QInputDialog::getText(this,"增加用户","请输入用户密码");
    if(oldPass.simplified() == "")
    {
        QMessageBox::information(this,"增加用户失败","密码不能为空");
        return ;
    }

    /*新密码确认*/
    QString newPass = QInputDialog::getText(this,"修改密码","请再次输入密码");
    if(oldPass != newPass)
    {
        QMessageBox::information(this,"增加用户失败","两次密码不同");
        return ;
    }

    /*输入权限*/
    QStringList dept;
    dept<<tr("管理员")<<tr("员工");
    QString Dept = QInputDialog::getItem(this,"修改密码","请再次输入密码",dept,0,false);
    //qDebug()<<Dept;

    /*将数据写进数据表*/
    temp = "insert LoginUser Values(" + QString::number(m_numLoginUser ++) + ",'" + name;
    temp += "','" + newPass + "','";
    if(Dept == "管理员")
        temp += "Admin')";
    else
        temp += "Emp')";
    query.exec(temp);

    initUser();
    QMessageBox::information(this,"增加用户成功","增加了用户 \n" + name);
}

void PersonForm::on_changePass_Btn_clicked()
{
    if(ui->person_TView->currentIndex().row() == -1)
        return ;

    /*输入旧密码，并确认*/
    QString oldPass = QInputDialog::getText(this,"修改密码","请输入旧密码");
    if(oldPass.simplified() == "")
        return ;
    QString ID = ui->person_TView->model()->index(ui->person_TView->currentIndex().row(),0).data(0).toString();
    ID = ID.simplified();

    QString temp = "select Password from LoginUser where ID=" + ID;
    QSqlQuery query;
    query.exec(temp);
    query.next();
    QString password = query.value(0).toString();
    password = password.simplified();
    if(oldPass != password)
    {
        QMessageBox::information(this,"修改失败","旧密码输入错误");
        return ;
    }

    /*输入新密码*/
    oldPass = QInputDialog::getText(this,"修改密码","请输入新密码");
    if(oldPass.simplified() == "")
    {
        QMessageBox::information(this,"修改失败","密码不能为空");
    }

    /*新密码确认*/
    QString newPass = QInputDialog::getText(this,"修改密码","请再次输入新密码");
    if(oldPass != newPass)
    {
        QMessageBox::information(this,"修改失败","两次新密码不同");
        return ;
    }

    temp = "update LoginUser set Password='" + newPass + "' where ID=" + ID;
    query.exec(temp);

    initUser();
    QMessageBox::information(this,"修改密码","密码修改成功");
}

void PersonForm::on_delPerson_Btn_clicked()
{
    /*检查鼠标*/
    if(ui->person_TView->currentIndex().row() == -1)
        return ;
    QString name = ui->person_TView->model()->index(ui->person_TView->currentIndex().row(),1).data(0).toString();
    name = name.simplified();

    /*打印确认框*/
    QString temp = "是否删除员工\n " + name + " ?";

    switch(QMessageBox::question(this,tr("询问"),temp,
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*记录ID*/
    QString ID = ui->person_TView->model()->index(ui->person_TView->currentIndex().row(),0).data(0).toString();
    ID = ID.simplified();

    /*从数据库中删除*/
    QSqlQuery query;
    temp = "delete from LoginUser where ID=" + ID;
    query.exec(temp);

    temp = "update LoginUser set ID=ID-1 where ID>" + ID;
    query.exec(temp);

    initUser();
    -- m_numLoginUser;
    QMessageBox::information(this,"操作成功","已删除用户");
}

void PersonForm::on_person_TView_clicked(const QModelIndex &index)
{
    QString temp = ui->person_TView->model()->index(ui->person_TView->currentIndex().row(),2).data(0).toString();
    temp = temp.simplified();
    if(temp == "Admin")
        ui->Admin_RBtn->setChecked(true);
    else
        ui->Emp_RBtn->setChecked(true);
}
