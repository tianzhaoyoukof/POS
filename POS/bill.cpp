#include "bill.h"
#include "ui_bill.h"
#include <QDebug>
Bill::Bill(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill)
{
    ui->setupUi(this);
    this->setWindowTitle("账单");
}

Bill::~Bill()
{
    delete ui;
}

void Bill::printName(QString name, QString tradeNo)
{
    ui->person_LEdit->setText(name);
    ui->trade_LEdit->setText(tradeNo);
}

void Bill::printCOUNTandPRICE(int count, double price)
{
    ui->count_LEdit->setText(QString::number(count));
    ui->price_LEdit->setText(QString::number(price));
}

void Bill::printGoods(QVector<QString> &n, QVector<QString> &c, QVector<QString> &p)
{
    int size = n.size();
    ui->name_LWidget->clear();
    ui->count_LWidget->clear();
    ui->price_LWidget->clear();
    ui->name_LWidget->addItem("名称");
    ui->count_LWidget->addItem("数量");
    ui->price_LWidget->addItem("单价");
    for(int i = 0 ; i < size ; i++)
    {
        ui->name_LWidget->addItem(n[i]);
        ui->count_LWidget->addItem(c[i]);
        ui->price_LWidget->addItem(p[i]);
    }

}

void Bill::setDate(const QString &date)
{
    ui->date_LEdit_3->setText(date);
}

void Bill::setVIPNo(const QString &date)
{
    ui->VIPNo_LEdit_4->setText(date);
}

void Bill::setPay(QString pay)
{
    ui->pay_LEdit->setText(pay);
}

void Bill::initBill()
{

}

void Bill::on_close_Btn_clicked()
{
    this->hide();
    emit resetForm();
}
