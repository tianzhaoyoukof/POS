#include "showvipcard.h"
#include "ui_showvipcard.h"

#include <QSqlQuery>
#include <QMessageBox>

ShowVIPCard::ShowVIPCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowVIPCard)
{
    ui->setupUi(this);
    this->setWindowTitle("展示VIP");
}

ShowVIPCard::~ShowVIPCard()
{
    delete ui;
}

void ShowVIPCard::on_sure_Btn_clicked()
{
    if(ui->no_LEdit->text() == "")
        return ;
    QSqlQuery query;
    query.exec("select No,Consume from VIP");
    while(query.next())
    {
        QString temp = query.value(0).toString();
        temp = temp.simplified();
        double d = query.value(1).toDouble();
        if(temp == ui->no_LEdit->text())
        {
            emit showed(ui->no_LEdit->text(),d);
            return ;
        }
    }
    QMessageBox::information(this,"","没有此会员");
}

void ShowVIPCard::on_cancel_Btn_clicked()
{
    emit showed("",0.0);
}
