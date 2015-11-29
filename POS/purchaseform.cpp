#include "purchaseform.h"
#include "ui_purchaseform.h"

#include <QDateTime>
#include <QInputDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
PurchaseForm::PurchaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseForm),
    m_addFirm(NULL),
    m_goods(0),
    m_goodsKind(0),
    m_firm(0),
    m_supplier(0),
    m_stock(0),
    m_Firm_or_Supplier(false),
    m_queryModel(NULL)

{
    ui->setupUi(this);
    init();
}

PurchaseForm::~PurchaseForm()
{
    delete m_addFirm;
    delete m_queryModel;
    delete ui;
}

void PurchaseForm::init()
{
    /*新建对话框并隐藏*/
    m_addFirm = new AddFirm_Supplier();
    m_addFirm->hide();
    connect(m_addFirm,SIGNAL(add(QString,QString,QString)),this,SLOT(add(QString,QString,QString)));

    QSqlQuery query;
    QString temp = "";
    /*从Goods表中读取总共的商品有多少，数量记录在m_goods*/
    query.exec("select count(*) from Goods");
    query.next();
    m_goods = query.value(0).toInt();
    ++ m_goods;
    //qDebug()<<"m_goods:"<<m_goods;

    /*设置日期为今天日期*/
    ui->sDate_DEdit->setDateTime(QDateTime::currentDateTime());
    ui->eDate_DEdit->setDateTime(QDateTime::currentDateTime());

    /*从GoodsKind表中读取总共的商品分类有多少，加在 ui->kind_LWidget 中，
     * 种类数量记录在m_goodsKind*/
    query.exec("select Name from GoodsKind");
    while(query.next())
    {
        temp = query.value(0).toString();
        ui->kind_LWidget->addItem(temp);
        ++ m_goodsKind;
    }
    ++ m_goodsKind;
    //qDebug()<<"goodsKind :"<<m_goodsKind;


    /*从Firm表中读取总共的厂商有多少，加在 ui->firm_LWidget 中，
     * 厂商数量记录在m_firm*/
    query.exec("select Name from Firm");
    while(query.next())
    {
        temp = query.value(0).toString();
        ui->firm_LWidget->addItem(temp);
        ++ m_firm;
    }
    ++ m_firm;
    //qDebug()<<"m_firm :"<<m_firm;

    /*从Supplier表中读取总共的供应商有多少，加在 ui->supplier_LWidget 中，
     * 供应商数量记录在 m_supplier*/
    query.exec("select Name from Supplier");
    while(query.next())
    {
        temp = query.value(0).toString();
        ui->supplier_LWidget->addItem(temp);
        ++ m_supplier;
    }
    ++ m_supplier;
    //qDebug()<<"m_supplier :"<<m_supplier;

    /*从Stock表中读取总共的进货单有多少
     * 供应商数量记录在 m_supplier*/
    query.exec("select ID from Stock");
    if(query.first())
    {
        ++ m_stock;
        while(query.next())
        {
            ++ m_stock;
        }
    }
    ++ m_stock;
    m_queryModel = new QSqlQueryModel();
    initStock();
}

void PurchaseForm::delFirm_or_Supplier()
{
    /*检测当前鼠标指定的位置*/
    if(m_Firm_or_Supplier)  //如果是厂商
    {
        if(ui->firm_LWidget->currentRow() == -1)
            return ;
    }
    else   //如果是供应商
    {
        if(ui->supplier_LWidget->currentRow() == -1)
            return ;
    }

    /*检查该  供应商 或者 厂商  是不是有商品，有商品则不能删除*/
    QSqlQuery query;
    QString temp;
    QString name;
    if(m_Firm_or_Supplier)
    {
        name = ui->firm_LWidget->item(ui->firm_LWidget->currentRow())->text();
        name = name.simplified();
        temp = "select count(*) from Firm join Goods ";
        temp += "on(Firm.ID = Goods.FirmID) and Firm.Name='";
        temp += name + "' group by Goods.FirmID";
    }
    else
    {
        name = ui->supplier_LWidget->item(ui->supplier_LWidget->currentRow())->text();
        name = name.simplified();
        temp = "select count(*) from Supplier join Goods ";
        temp += "on(Supplier.ID = Goods.SupplierID) and Supplier.Name='";
        temp += name + "' group by Goods.SupplierID";
    }
    query.exec(temp);
    if(query.first())
    {
        if(m_Firm_or_Supplier)
            temp = "该厂商有其他商品存在";
        else
            temp = "该供应商有其他商品存在";
        QMessageBox::information(this,"删除不成功",temp);
        return ;
    }

    /*记录那个删除的分类的ID*/
    if(m_Firm_or_Supplier)
        temp = "select ID from Firm where Name='" + name + "'";
    else
        temp = "select ID from Supplier where Name='" + name + "'";
    query.exec(temp);
    query.next();
    QString ID = query.value(0).toString();
    //qDebug()<<ID;

    /*向Supplier表中删除一项，还要向 ui->kind_LWidget 删除一项，m_supplier减少*/
    /*或者*/
    /*向Firm表中删除一项，还要向 ui->firm_LWidget 删除一项，m_firm减少*/
    if(m_Firm_or_Supplier)
    {
        ui->firm_LWidget->takeItem(ui->firm_LWidget->currentRow());
        temp = "delete from Firm where Name='" + name + "'";
        -- m_firm;
    }
    else
    {
        ui->supplier_LWidget->takeItem(ui->supplier_LWidget->currentRow());
        temp = "delete from Supplier where Name='" + name + "'";
        -- m_supplier;
    }
    query.exec(temp);
    //qDebug()<<m_goodsKind;

    /*ID调整，减少*/
    if(m_Firm_or_Supplier)
        temp = "update Firm set ID=ID-1 where ID>" + ID;
    else
        temp = "update Supplier set ID=ID-1 where ID>" + ID;
    query.exec(temp);

}

void PurchaseForm::initStock()
{
    m_queryModel->setQuery("select Goods.Name as '商品名',Stock.Number as '进货数量',Stock.UnitPrice as '单价',Stock.AllPrice as '总价' from Stock join Goods on Goods.ID=Stock.GoodsID;");
    ui->stock_TView->setModel(m_queryModel);
}

void PurchaseForm::add(QString name, QString address, QString phone)
{
    QSqlQuery query;
    QString temp;
    if(m_Firm_or_Supplier)  //如果是厂商
    {
        temp = "select Name from Firm";
    }
    else   //如果是供应商
    {
        temp = "select Name from Supplier";
    }

    /*检查 厂商 或 供应商 是否已经存在，存在，则返回*/
    query.exec(temp);
    while(query.next())
    {
        temp = query.value(0).toString();
        temp = temp.simplified();
        if(temp == name)
        {
            QMessageBox::information(this,"添加厂商不成功","该厂商已经存在");
            return ;
        }
    }

    /*向Firm表中写入一项，还要向 ui->firm_LWidget 添加一项，m_firm增加*/
    /*或者*/
    /*向Supplier表中写入一项，还要向 ui->supplier_LWidget 添加一项，m_supplier增加*/
    if(m_Firm_or_Supplier)  //如果是厂商
    {
        ui->firm_LWidget->addItem(name);
        temp = "insert Firm Values(";
        temp += QString::number(m_firm ++);
    }
    else   //如果是供应商
    {
        ui->supplier_LWidget->addItem(name);
        temp = "insert Supplier Values(";
        temp += QString::number(m_supplier ++);
    }
    temp += ",'" + name + "','" + address + "','" + phone + "')";
    query.exec(temp);
}

void PurchaseForm::on_addKind_Btn_clicked()
{
    /*输入种类的名称，名称为空则返回*/
    QString s = QInputDialog::getText(this,"输入新分类","请输入新分类");
    if(s == "")
       return ;

    /*检查种类是否已经存在，存在，则返回*/
    QSqlQuery query;
    QString temp;
    query.exec("select Name from GoodsKind");
    while(query.next())
    {
        temp = query.value(0).toString();
        temp = temp.simplified();
        if(s == temp)
        {
            QMessageBox::information(this,"添加不成功","该分类已经存在");
            return ;
        }
    }


    /*向GoodsKind表中写入一项，还要向 ui->kind_LWidget 添加一项，m_goodsKind增加*/
    temp = "insert GoodsKind Values(" + QString::number(m_goodsKind);
    temp += ",'" + s + "')";
    query.exec(temp);
    ui->kind_LWidget->addItem(s);
    ++ m_goodsKind;
}

void PurchaseForm::on_delKind_Btn_clicked()
{
    /*检测当前鼠标指定的位置*/
    if(ui->kind_LWidget->currentRow() == -1)
        return ;
    //qDebug()<<ui->kind_LWidget->currentRow();

    /*检查该分类是不是有商品，有商品则不能删除*/
    QSqlQuery query;
    QString temp;
    QString name;
    name = ui->kind_LWidget->item(ui->kind_LWidget->currentRow())->text();
    name = name.simplified();
    temp = "select count(*) from GoodsKind join Goods ";
    temp += "on(GoodsKind.ID = Goods.Kind) and GoodsKind.Name = '";
    temp += name + "' group by Goods.Kind;";
    //qDebug()<<temp;
    query.exec(temp);
    if(query.first())
    {
        QMessageBox::information(this,"删除不成功","该分类有其他商品存在");
        return ;
    }

    /*记录那个删除的分类的ID*/
    temp = "select ID from GoodsKind where Name='" + name + "'";
    //qDebug()<<temp;
    query.exec(temp);
    query.next();
    QString ID = query.value(0).toString();
    //qDebug()<<ID;

    /*向GoodsKind表中删除一项，还要向 ui->kind_LWidget 删除一项，m_goodsKind减少*/
    ui->kind_LWidget->takeItem(ui->kind_LWidget->currentRow());
    temp = "delete from GoodsKind where Name='" + name + "'";
    //qDebug()<<temp;
    query.exec(temp);
    -- m_goodsKind;
    //qDebug()<<m_goodsKind;

    /*ID调整，减少*/
    temp = "update GoodsKind set ID=ID-1 where ID>" + ID;
    //qDebug()<<temp;
    query.exec(temp);
}

void PurchaseForm::on_addFirm_Btn_clicked()
{
    /*检查输入，结果在add(QString name, QString address, QString phone)中判断*/
    m_addFirm->show();
    m_addFirm->setWindowTitle("添加厂商");
    m_Firm_or_Supplier = true;
}

void PurchaseForm::on_delFirm_Btn_clicked()
{
    m_Firm_or_Supplier = true;
    delFirm_or_Supplier();
}

void PurchaseForm::on_addSupplier_Btn_clicked()
{
    /*检查输入，结果在add(QString name, QString address, QString phone)中判断*/
    m_addFirm->show();
    m_addFirm->setWindowTitle("添加供应商");
    m_Firm_or_Supplier = false;
}

void PurchaseForm::on_delSupplier_Btn_clicked()
{
    m_Firm_or_Supplier = false;
    delFirm_or_Supplier();
}

void PurchaseForm::on_sure_Btn_clicked()
{
    QSqlQuery query;
    QString temp;
    QString name = ui->name_LEdit->text().simplified();
    /*检测商品名称不能为空，且不能存在*/
    if(name == "")
    {
        QMessageBox::information(this,"进货失败","商品名不能为空");
        return ;
    }
    temp = "select Name from Goods where Name='" + name + "'";
    query.exec(temp);
    if(query.first())
    {
        QMessageBox::information(this,"进货失败","商品名已存在");
        return ;
    }

    name = ui->bar_LEdit->text().simplified();
    /*检测条形码不能为空，且不能存在*/
    if(name == "")
    {
        QMessageBox::information(this,"进货失败","条形码不能为空");
        return ;
    }
    temp = "select Name from Goods where Bar='" + name + "'";
    query.exec(temp);
    if(query.first())
    {
        QMessageBox::information(this,"进货失败","条形码已存在");
        return ;
    }

    /*检测起促销日期不能比止促销日期大*/
    if(ui->sDate_DEdit->date() > ui->eDate_DEdit->date())
    {
        QMessageBox::information(this,"进货失败","促销日期错误");
        return ;
    }

    /*检测有选择商品种类，厂商，供应商*/
    if(ui->kind_LWidget->currentRow() == -1)
    {
        QMessageBox::information(this,"进货失败","没有选择商品分类");
        return ;
    }
    if(ui->firm_LWidget->currentRow() == -1)
    {
        QMessageBox::information(this,"进货失败","没有选择厂商号");
        return ;
    }
    if(ui->supplier_LWidget->currentRow() == -1)
    {
        QMessageBox::information(this,"进货失败","没有选择供应商号");
        return ;
    }

    /*向Goods表中插入一条记录，m_goods增加*/
    temp = "insert Goods Values(" + QString::number(m_goods ++) + ",'";
    temp += ui->name_LEdit->text().simplified() + "','";
    temp += ui->bar_LEdit->text().simplified() + "'," + QString::number(ui->iPrice->value());
    temp += "," + QString::number(ui->rPrice->value()) + "," + QString::number(ui->wPrice->value());
    temp += "," + QString::number(ui->wPrice->value()) + ",'" + ui->sDate_DEdit->date().toString("yyyy-MM-dd");
    temp += "','" + ui->eDate_DEdit->date().toString("yyyy-MM-dd") + "'," + ui->num_SBox->text();
    temp += "," + ui->lnum_SBox->text() + "," + ui->innum_SBox->text();
    if(ui->sellable_CBox->isChecked())
        temp += ",'Y',";
    else
        temp += ",'N',";
    temp += QString::number(ui->firm_LWidget->currentRow() + 1) + ",";
    temp += QString::number(ui->supplier_LWidget->currentRow() + 1) + ",";
    temp += QString::number(ui->kind_LWidget->currentRow() + 1) + ")";
    query.exec(temp);

    /*将新进货的信息 Stock 表中，并将 m_stock 增加*/
    temp = "insert Stock Values(" + QString::number(m_stock ++) + "," + QString::number(m_goods - 1) + ",";
    temp += ui->num_SBox->text() + "," + ui->iPrice->text() + ",";
    temp += QString::number(ui->num_SBox->value() * ui->iPrice->value()) + ",'";
    temp += QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
    qDebug()<<temp;
    query.exec(temp);

    /*打印信息*/
    temp = "现有新商品\n" + ui->name_LEdit->text().simplified();
    QMessageBox::information(this,"进货成功",temp);
}

void PurchaseForm::on_sure_Btn_3_clicked()
{
    /*检查商品的名称或条形码是否存在*/
    QString name = ui->barPurchase_LEdit->text().simplified();
    QSqlQuery query;
    QString ID;
    QString unit;
    //qDebug()<<name;
    QString temp = "select Name,ID,IPrice from Goods where Name='" + name + "' or ";
    temp += "Bar='" + name + "'";
    qDebug()<<temp;
    query.exec(temp);
    if(query.first())   //存在该商品
    {
        name = query.value(0).toString().simplified();
        ID = query.value(1).toString().simplified();
        unit = query.value(2).toString().simplified();
    }
    else
        return ;
    //qDebug()<<name<<"  "<<ui->numPurchase_SBox->value();

    /*向Goods表中更新数据，增加商品数量*/
    temp = "update Goods set Number=Number+" + QString::number(ui->numPurchase_SBox->value());
    temp += " where Name='" + name + "'";
    query.exec(temp);

    /*将新进货的信息 Stock 表中，并将 m_stock 增加*/
    temp = "insert Stock Values(" + QString::number(m_stock ++) + "," + ID + ",";
    temp += ui->numPurchase_SBox->text() + "," + unit + ",";
    temp += QString::number(ui->numPurchase_SBox->value() * unit.toDouble()) + ",'";
    temp += QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
    qDebug()<<temp;
    query.exec(temp);

    /*打印信息*/
    temp = "添加\n" + name + " * " + ui->numPurchase_SBox->text();
    QMessageBox::information(this,"进货成功",temp);
}

void PurchaseForm::on_sure_Btn_4_clicked()
{
    /*检查商品的名称或条形码是否存在*/
    QString name = ui->barDEL_LEdit->text().simplified();
    QSqlQuery query;
    //qDebug()<<name;
    QString temp = "select Name from Goods where Name='" + name + "' or ";
    temp += "Bar='" + name + "'";
    //qDebug()<<temp;
    query.exec(temp);
    if(query.first())   //存在该商品
        name = query.value(0).toString().simplified();
    else
        return ;
    //qDebug()<<name;

    switch(QMessageBox::question(this,tr("询问"),tr("是否清除仓库?"),
            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
        break;
    default:
        return ;
    }

    /*记录ID*/
    temp = "select ID from Goods where Name='" + name + "'";
    //qDebug()<<temp;
    query.exec(temp);
    query.next();
    QString ID = query.value(0).toString();
    //qDebug()<<ID;

    /*将Goods表中该行数据删掉，并重新排ID，m_goods减少*/
    temp = "delete from Goods where Name='" + name + "'";
    //qDebug()<<temp;
    query.exec(temp);
    //qDebug()<<m_goodsKind;

    /*ID调整，减少*/
    temp = "update Goods set ID=ID-1 where ID>" + ID;
    query.exec(temp);

    -- m_goods;
    temp = "已清除出仓库\n" + name;
    QMessageBox::information(this,"进货成功",temp);
}

void PurchaseForm::on_sort_Btn_clicked()
{
    //qDebug()<<ui->sortNUM->currentIndex()<<"  "<<ui->sortUNIT->currentIndex()<<"  "<<ui->sortALL->currentIndex();
    QString temp = "select Goods.Name as '商品名',Stock.Number as '进货数量',";
    temp += "Stock.UnitPrice as '单价',Stock.AllPrice as '总价' from Stock join Goods on Goods.ID=Stock.GoodsID ";
    if(ui->sortNUM->currentIndex() == 1)
        temp += "order by Stock.Number ASC";
    else if(ui->sortNUM->currentIndex() == 2)
        temp += "order by Stock.Number DESC";

    if(ui->sortUNIT->currentIndex() != 0)
    {
        if(ui->sortNUM->currentIndex() != 0)
            temp += ",";
        else
            temp += "order by ";
        if(ui->sortUNIT->currentIndex() == 1)
            temp += "Stock.UnitPrice ASC";
        else if(ui->sortUNIT->currentIndex() == 2)
            temp += "Stock.UnitPrice DESC";
    }

    if(ui->sortALL->currentIndex() != 0)
    {
        if(ui->sortNUM->currentIndex() != 0 || ui->sortUNIT->currentIndex() != 0)
            temp += ",";
        else
            temp += "order by ";
        if(ui->sortALL->currentIndex() == 1)
            temp += "Stock.AllPrice ASC";
        else if(ui->sortALL->currentIndex() == 2)
            temp += "Stock.AllPrice DESC";
    }

    //qDebug()<<temp;

    m_queryModel->setQuery(temp);
    ui->stock_TView->setModel(m_queryModel);
}

void PurchaseForm::on_tabWidget_currentChanged(int index)
{
    if(index == 3)
        initStock();
}
