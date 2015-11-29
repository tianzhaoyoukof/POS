#include "addfirm_supplier.h"
#include "ui_addfirm_supplier.h"
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>

AddFirm_Supplier::AddFirm_Supplier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFirm_Supplier)
{
    ui->setupUi(this);
}

AddFirm_Supplier::~AddFirm_Supplier()
{
    delete ui;
}

void AddFirm_Supplier::init()
{
    /*password_LEdit限制条件*/
    QRegExp reg("[0-9]+$");
    QValidator *qValidator = new QRegExpValidator(reg,ui->phone_LEdit);
    ui->phone_LEdit->setValidator(qValidator);
}

void AddFirm_Supplier::on_add_Btn_clicked()
{
    if(ui->name_LEdit->text().simplified() == "")
    {
        QMessageBox::information(this,"添加失败","名字不能为空");
        return ;
    }

    if(ui->adress_LEdit->text().simplified() == "")
    {
        QMessageBox::information(this,"添加失败","地址不能为空");
        return ;
    }

    if(ui->phone_LEdit->text().simplified() == "")
    {
        QMessageBox::information(this,"添加失败","电话不能为空");
        return ;
    }

    emit add(ui->name_LEdit->text().simplified(),ui->adress_LEdit->text().simplified(),
             ui->phone_LEdit->text().simplified());
    this->hide();
}
