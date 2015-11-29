#include "sellmanform.h"
#include "ui_sellmanform.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>

SellManForm::SellManForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellManForm),
    m_queryModel(NULL)
{
    ui->setupUi(this);
    init();
}

SellManForm::~SellManForm()
{
    delete ui;
}


void SellManForm::init()
{
    m_queryModel = new QSqlQueryModel();
    on_tabWidget_currentChanged(0);
}

void SellManForm::initSell()
{
    QString temp = "select Sell.ID as '编号',Goods.Name as '商品名称',Sell.Number as '销售数量',";
    temp += "Sell.Price as '销售总额' from Sell join Goods on Goods.ID=Sell.GoodsID";

    if(ui->sortSellNum->currentIndex() == 1)
    {
        temp += " order by '销售数量' ASC";
    }
    else if(ui->sortSellNum->currentIndex() == 2)
    {
        temp += " order by '销售数量' DESC";
    }

    if(ui->sortSellPrice->currentIndex() != 0)
    {
        if(ui->sortSellNum->currentIndex() != 0)
            temp += ",'销售总额' ";
        else
            temp += " order by '销售总额' ";
        if(ui->sortSellPrice->currentIndex() == 1)
            temp += "ASC";
        else if(ui->sortSellPrice->currentIndex() == 2)
            temp += "DESC";
    }
    m_queryModel->setQuery(temp);
    ui->sell_TView->setModel(m_queryModel);

}

void SellManForm::initTrade()
{
    QString temp;
    if(ui->sortKind->currentIndex() == 0 && ui->sortType->currentIndex() == 0)
        temp = "select ID as '编号',Name as '销售员',Price as '销售金额',VIPNo as '会员卡号',TDate as '销售日期' from Trade";
    else if(ui->sortKind->currentIndex() == 0)   //总销售金额
    {
        temp = "select ID as '编号',Name as '销售员',Price as '销售金额',VIPNo as '会员卡号',TDate as '销售日期' from Trade";
        switch(ui->sortType->currentIndex())
        {
            case 1:
            {
                temp += " order by Price ASC";
                break;
            }
            case 2:
            {
                temp += " order by Price DESC";
                break;
            }
        }
    }
    else if(ui->sortKind->currentIndex() == 1)  //员工销售金额
    {
        temp = "select Name as '销售员',sum(Price) as '销售总额' from Trade group by Name";
        switch(ui->sortType->currentIndex())
        {
            case 1:
            {
                temp += " order by '销售总额' ASC";
                break;
            }
            case 2:
            {
                temp += " order by '销售总额' DESC";
                break;
            }
        }
    }
    else if(ui->sortKind->currentIndex() == 2)  //会员销售金额
    {
        temp = "select Trade.VIPNo as '会员卡号',sum(Price) as '总消费' from Trade join VIP on Trade.VIPNo=VIP.No group by Trade.VIPNo";
        switch(ui->sortType->currentIndex())
        {
            case 1:
            {
                temp += " order by '总消费' ASC";
                break;
            }
            case 2:
            {
                temp += " order by '总消费' DESC";
                break;
            }
        }
    }
    m_queryModel->setQuery(temp);
    ui->trade_TView->setModel(m_queryModel);

    if(ui->sortKind->currentIndex() == 0)
    {
        ui->showAllTrade_Btn->setEnabled(false);
        ui->delTrade_Btn->setEnabled(true);
    }
    else
    {
        ui->showAllTrade_Btn->setEnabled(true);
        ui->delTrade_Btn->setEnabled(false);
    }
}

void SellManForm::on_sortSELL_Btn_clicked()
{
    initSell();
}

void SellManForm::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case 0:
        initSell();
        break;
    case 1:
        initTrade();
        break;
    }
}

void SellManForm::on_sortTRADE_Btn_clicked()
{
    initTrade();
}

void SellManForm::on_delSell_Btn_clicked()
{
    if(ui->sell_TView->currentIndex().row() == -1)
        return ;

    switch(QMessageBox::question(this,tr("询问"),tr("是否剔除该销售表?"),
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*记录ID*/
    int ID = m_queryModel->record(ui->sell_TView->currentIndex().row()).value(0).toInt();

    QSqlQuery query;
    QString temp = "delete from Sell where ID=" + QString::number(ID);
    query.exec(temp);

    temp = "update Sell set ID=ID-1 where ID>" + QString::number(ID);
    query.exec(temp);

    initSell();

    QMessageBox::information(this,"操作成功","成功剔除改销售表");
}

void SellManForm::on_delTrade_Btn_clicked()
{
    if(ui->trade_TView->currentIndex().row() == -1)
        return ;

    switch(QMessageBox::question(this,tr("询问"),tr("是否剔除该交易表?"),
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*记录ID*/
    int ID = m_queryModel->record(ui->trade_TView->currentIndex().row()).value(0).toInt();
    //qDebug()<<ID;

    QSqlQuery query;
    QString temp = "delete from Trade where ID=" + QString::number(ID);
    query.exec(temp);

    temp = "update Trade set ID=ID-1 where ID>" + QString::number(ID);
    query.exec(temp);

    on_showAllTrade_Btn_clicked();

    QMessageBox::information(this,"操作成功","成功剔除改交易表");
}

void SellManForm::on_showAllTrade_Btn_clicked()
{
    ui->sortKind->setCurrentIndex(0);
    initTrade();
}
