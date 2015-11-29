#include "stockform.h"
#include "ui_stockform.h"

#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

StockForm::StockForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockForm),
    m_queryModel(NULL),
    m_stock(0)
{
    ui->setupUi(this);
    init();
}

StockForm::~StockForm()
{
    delete ui;
}

void StockForm::init()
{
    QString temp = "select Name from GoodsKind";
    QSqlQuery query;
    query.exec(temp);
    ui->kind_CBox->addItem("");
    while(query.next())
    {
        QString name = query.value(0).toString();
        ui->kind_CBox->addItem(name);
    }

    m_queryModel = new QSqlQueryModel();

    /*读取进货单数*/
    query.exec("select ID from Stock");
    while(query.next())
    {
        ++ m_stock;
    }
    ++ m_stock;

    m_queryModel = new QSqlQueryModel();

    lowNumber();

    initMSG();
}

void StockForm::initMSG()
{
    QString temp = "select Name as '商品名称',Bar as '条形码',IPrice as '进货价',RPrice as '零售价',WPrice as '批发价',";
    temp += "PPrice as '促销价',SDate as '促销起日期',";
    temp += "EDate as '促销止日期',Number as '库存数量',LNumber as '报警数量',Saleable as '允许销售',";
    temp += "(select Name from Firm where ID = A.FirmID) as '厂商',";
    temp += "(select Name from Supplier where ID = A.SupplierID) as '供货商',";
    temp += "(select Name from GoodsKind where ID = A.kind) as '分类' from Goods A";
    /*分类窗口的增加限制*/
    if(ui->kind_CBox->currentIndex() != 0)
    {
        QString name = ui->kind_CBox->currentText().simplified();
        temp += " where (select Name from GoodsKind where ID = A.kind)='" + name + "'";
    }
    if(ui->msg_LEdit->text().simplified() != "")
    {
        if(ui->kind_CBox->currentIndex() == 0)
        {
            temp += " where";
        }
        else
        {
            temp += " and";
        }
        temp += " (A.Name like '%" + ui->msg_LEdit->text().simplified() + "%' or";
        temp += " A.Bar like '%" + ui->msg_LEdit->text().simplified() + "%' or";
        temp += " (select Name from Firm where ID = A.FirmID) like '%" + ui->msg_LEdit->text().simplified() + "%' or";
        temp += " (select Name from Supplier where ID = A.SupplierID) like '%" + ui->msg_LEdit->text().simplified() + "%')";
    }

    m_queryModel->setQuery(temp);
    ui->MSG_TView->setModel(m_queryModel);
}

void StockForm::lowNumber()
{
    QSqlQuery query;
    QString temp = "select Name from Goods where Number<LNumber";
    query.exec(temp);
    while(query.next())
    {
        QString name = query.value(0).toString();
        ui->lnum_LWidget->addItem(name);
    }
}

void StockForm::on_search_Btn_clicked()
{
    initMSG();
}

void StockForm::on_lnum_LWidget_currentRowChanged(int currentRow)
{
    if(currentRow == -1)
        return ;
    QSqlQuery query;
    QString temp;
    QString name = ui->lnum_LWidget->item(ui->lnum_LWidget->currentIndex().row())->text().simplified();
    temp = "select INNumber,(LNumber-Number) from Goods where Name='" + name + "'";
    query.exec(temp);
    query.next();
    //ui->inum_SBox->setValue(query.value(1).toInt());
    ui->inum_SBox->setMinimum(query.value(1).toInt());
    ui->inum_SBox->setSingleStep(query.value(0).toInt());
}

void StockForm::on_purchase_Btn_clicked()
{
    if(ui->lnum_LWidget->currentIndex().row() == -1)
        return ;
    QString temp = "是否进货:\n" + ui->lnum_LWidget->item(ui->lnum_LWidget->currentIndex().row())->text().simplified() + " * ";
    temp += ui->inum_SBox->text();

    switch(QMessageBox::question(this,tr("进货"),temp,
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*向Goods表中更新数据，增加商品数量*/
    QSqlQuery query;
    temp = "update Goods set Number=Number+" + ui->inum_SBox->text();
    temp += " where Name='" + ui->lnum_LWidget->item(ui->lnum_LWidget->currentIndex().row())->text().simplified() + "'";qDebug()<<temp;
    query.exec(temp);

    temp = "select ID,IPrice from Goods where Name='" + ui->lnum_LWidget->item(ui->lnum_LWidget->currentIndex().row())->text().simplified() + "'";
    query.exec(temp);
    query.next();
    QString ID = query.value(0).toString().simplified();
    QString unit = query.value(1).toString().simplified();

    /*将新进货的信息 Stock 表中，并将 m_stock 增加*/
    temp = "insert Stock Values(" + QString::number(m_stock ++) + "," + ID + ",";
    temp += ui->inum_SBox->text() + "," + unit + ",";
    temp += QString::number(ui->inum_SBox->value() * unit.toDouble()) + ",'";
    temp += QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
    query.exec(temp);
    temp = "进货\n" + ui->lnum_LWidget->item(ui->lnum_LWidget->currentIndex().row())->text().simplified() + " * " + ui->inum_SBox->text();
    QMessageBox::information(this,"进货成功",temp);

    ui->lnum_LWidget->takeItem(ui->lnum_LWidget->currentIndex().row());
}
