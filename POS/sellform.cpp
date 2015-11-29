#include "sellform.h"
#include "ui_sellform.h"

#include <QTableWidgetItem>
#include<QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include <QMessageBox>

SellForm::SellForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellForm),
    m_numSell(0),
    m_numTrade(0),
    m_sellerName("")
{
    ui->setupUi(this);
    initSellFrom();
}

SellForm::~SellForm()
{
    delete ui;
}

void SellForm::setKind(int k)
{
    m_kind = k;
    resetForm();
    ui->no_LEdit->setText("");
    ui->num_SBox->setValue(1);
    ui->beizhu_LEdit->setText("");
    switch(k)
    {
        case 0:
        {
            QStringList header;
            header<<"商品名称"<<"数量"<<"单价"<<"小计"<<"备注";
            ui->info_Wig->setHorizontalHeaderLabels(header);
            break;
        }
        case 1:
        {
            QStringList header;
            header<<"商品名称"<<"数量"<<"批发价"<<"小计"<<"备注";
            ui->info_Wig->setHorizontalHeaderLabels(header);
            break;
        }
        case 2:
        {
            QStringList header;
            header<<"商品名称"<<"数量"<<"退货价"<<"小计"<<"备注";
            ui->info_Wig->setHorizontalHeaderLabels(header);
            break;
        }
        default:
        {
            break;
        }
    }
}

void SellForm::setSellerName(QString name)
{
    m_sellerName = name;
}

void SellForm::initSellFrom()
{
    m_goodsMessage = new GoodsMessage();
    m_goodsMessage->hide();
    m_bill = new Bill(this);

    m_bill->setWindowFlags(Qt::Dialog);
    //m_bill->setWindowModified(true);
    m_bill->hide();
    //m_bill->setWindowFlags(Qt::FramelessWindowHint);

    m_showVIPCard = new ShowVIPCard(this);
    m_showVIPCard->setWindowFlags(Qt::Dialog);
    //m_showVIPCard->setWindowModified(true);
    //m_showVIPCard->show();
    //m_showVIPCard->setWindowModified(false);

    m_showVIPCard->hide();

    connect(m_showVIPCard,SIGNAL(showed(QString,double)),this,SLOT(setVIPWindow(QString,double)));
    connect(m_bill,SIGNAL(resetForm()),this,SLOT(resetForm()));

    m_count = 0;
    m_price = 0.00;

    /*合计和数量的字体归0*/
    ui->count_LEdit->setText(QString::number(m_count));
    ui->price_LEdit->setText(QString::number(m_price,'g',3));

    /*统计销售单和交易单*/
    QSqlQuery query;
    query.exec("select max(ID) from Trade");
    query.next();
    if(query.value(0).toString() != "")
    {
        m_numTrade = query.value(0).toInt();
    }
    ++ m_numTrade;

    query.exec("select max(ID) from Sell");
    query.next();
    if(query.value(0).toString() != "")
    {
        m_numSell = query.value(0).toInt();
    }
    ++ m_numSell;

    qDebug()<<"sell:"<<m_numSell<<"   trade:"<<m_numTrade;
}

void SellForm::writeToSell(const QVector<int> &goodsId, const QVector<QString> &name,
                           const QVector<QString> &count, const QVector<QString> &price)
{
    QSqlQuery query;
    int size = name.size();
    for(int i = 0 ; i < size ; ++i)
    {

        /*在Goods表中减去相应的数量*/
        QString temp = "update Goods set Number=Number";
        temp += (m_kind == 2)?("+"):("-");
        temp += count[i] +" where Name='" + name[i] + "'";
        //qDebug()<<temp;
        query.exec(temp);

        /*将销售写进Sell表里面*/
        temp = "insert Sell values(" + QString::number(m_numSell) + ",";
        temp += QString::number(goodsId[i]) + ",";
        if(m_kind == 2)
            temp += "-";
        temp += count[i] + ",";
        if(m_kind == 2)
            temp += QString::number(-1 * count[i].toDouble() * price[i].toDouble());
        else
            temp += QString::number(count[i].toDouble() * price[i].toDouble());
        temp +=  ",'" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";
        //qDebug()<<temp;
        query.exec(temp);

        ++ m_numSell;
    }
}

void SellForm::writeToTrade(const QString& vipno)
{
    /*如果是退货*/
    if(m_kind == 2)
        m_price *= -1;

    QString temp = "insert Trade values(" + QString::number(m_numTrade) + ",'";
    temp += m_sellerName + "'," + QString::number(m_price) + ",'" + vipno + "','";
    temp += QDateTime::currentDateTime().toString("yyyy-MM-dd") + "')";

    QSqlQuery query;
    query.exec(temp);

    ++ m_numTrade;
}

void SellForm::resetForm()
{
    //ui->info_Wig->clear();
    int si = ui->info_Wig->rowCount();
    for(int i = si - 1 ; i >= 0 ; --i)
    {
        ui->info_Wig->removeRow(i);
    }

    m_count = 0;
    m_price = 0.0;

    ui->count_LEdit->setText(QString::number(m_count));
    ui->price_LEdit->setText(QString::number(m_price));
}

void SellForm::setVIPWindow(QString no,double money)
{
    m_showVIPCard->hide();

    /*会员卡号*/
    m_bill->setVIPNo(no);
    m_bill->show();
    //m_bill->setWindowFlags(Qt::Dialog);

    if(no != "")        //将销售额写进会员卡号
    {
        /*打折*/
        m_price *= 0.95;

        /*VIP表中插入数据*/
        QString temp = "update VIP set Consume=" + QString::number(money + m_price) + " where No='" + no + "'";
        QSqlQuery query;
        query.exec(temp);
        if(m_kind == 0)     //如果是销售界面
        {
            /*打印商品信息*/
            QVector<QString> name;
            QVector<QString> count;
            QVector<QString> price;
            QVector<int> goodsID;
            temp = "";
            int size = ui->info_Wig->rowCount();
            for(int i = 0 ; i < size ; ++i)
            {
                name.push_back(ui->info_Wig->item(i,0)->text().simplified());
                count.push_back(ui->info_Wig->item(i,1)->text());
                price.push_back(QString::number(ui->info_Wig->item(i,2)->text().toDouble() * 0.95)); //95折

                /*检测商品的数量够不够*/
                temp = "select ID from Goods where Name='" + name[i] + "'";

                query.exec(temp);
                query.next();

                goodsID.push_back(query.value(0).toInt());
            }

            /*将信息打印到交易表和商品表*/
            writeToTrade(no);
            writeToSell(goodsID,name,count,price);
        }

    }
    else if(m_kind == 0)
    {
        /*VIP表中插入数据*/
        QString temp = "";
        QSqlQuery query;

        /*打印商品信息*/
        QVector<QString> name;
        QVector<QString> count;
        QVector<QString> price;
        QVector<int> goodsID;

        int size = ui->info_Wig->rowCount();
        for(int i = 0 ; i < size ; ++i)
        {
            name.push_back(ui->info_Wig->item(i,0)->text().simplified());
            count.push_back(ui->info_Wig->item(i,1)->text());
            price.push_back(QString::number(ui->info_Wig->item(i,2)->text().toDouble() * 0.95)); //95折

            /*检测商品的数量够不够*/
            temp = "select ID from Goods where Name='" + name[i] + "'";

            query.exec(temp);
            query.next();

            goodsID.push_back(query.value(0).toInt());
        }

        /*将信息打印到交易表和商品表*/
        writeToTrade(no);
        writeToSell(goodsID,name,count,price);
    }

    m_bill->setPay(QString::number(m_price));
}

void SellForm::on_show_Btn_clicked()
{
    m_goodsMessage->show();
}

void SellForm::on_add_Btn_clicked()
{
    if(ui->no_LEdit->text() == "")
    {
        //qDebug()<<"空的编号";
        return ;
    }

    /*查询编号，如果查得到，添加到框中*/
    QSqlQuery query;
    int count = 0;
    QString s = "";
    switch(m_kind)
    {
        case 0:     //商品零售
        {
            s = "select Name,Bar,RPrice from Goods where Bar='"+ui->no_LEdit->text() + "'";
            break;
        }
        case 1:     //商品批发
        {
            s = "select Name,Bar,WPrice from Goods where Bar='"+ui->no_LEdit->text() + "'";
            break;
        }
        case 2:     //商品退货
        {
            s = "select Name,Bar,WPrice from Goods where Bar='"+ui->no_LEdit->text() + "'";
            break;
        }
    }

    //qDebug()<<s;
    query.exec(s);
    while(query.next())
    {
        QString temp = query.value(0).toString();


        /*首先检查是不是存在于销售单中*/
        int flag = -1;
        int size = ui->info_Wig->rowCount();
        //qDebug()<<"现在行数"<<size;
        for(int i = 0 ; i < size ; ++i)
        {
            if(ui->info_Wig->item(i,0)->text() == temp)
            {
                //qDebug()<<"在第"<<i<<"行";
                flag = i;
                break;
            }
        }

        if(flag != -1)        //如果存在于销售单中
        {
            int shuliang = ui->num_SBox->value() +
                    ui->info_Wig->item(flag,1)->text().toInt();
            ui->info_Wig->item(flag,1)->setText(QString::number(shuliang));//数量

            temp = query.value(2).toString();
            double danjia = temp.toDouble();

            double zongjia = danjia * shuliang;
            double xiaoji = ui->info_Wig->item(flag,3)->text().toDouble();
            temp = QString::number(zongjia);
            ui->info_Wig->item(flag,3)->setText(temp);//小计

            /*没有备注就显示备注,有备注就用旧的备注*/
            if(ui->info_Wig->item(flag,4)->text() == "")
            {
                ui->info_Wig->item(flag,4)->setText(ui->beizhu_LEdit->text());//备注
            }

            m_count += ui->num_SBox->value();   //添加数量
            m_price += zongjia - xiaoji ;       //添加总价
        }
        else            //如果不存在于销售单中
        {
            QTableWidgetItem* item = new QTableWidgetItem(temp);
            ui->info_Wig->insertRow(count);
            ui->info_Wig->setItem(count,0,item);//名称
            item->setFlags(Qt::NoItemFlags);

            int shuliang = ui->num_SBox->value();
            item = new QTableWidgetItem(QString::number(ui->num_SBox->value()));
            ui->info_Wig->setItem(count,1,item);//数量
            item->setFlags(Qt::NoItemFlags);

            temp = query.value(2).toString();
            double danjia = temp.toDouble();
            item = new QTableWidgetItem(temp);
            ui->info_Wig->setItem(count,2,item);//单价
            item->setFlags(Qt::NoItemFlags);

            double zongjia = danjia * shuliang;
            temp = QString::number(zongjia);
            item = new QTableWidgetItem(temp);
            ui->info_Wig->setItem(count,3,item);//小计
            item->setFlags(Qt::NoItemFlags);

            temp = ui->beizhu_LEdit->text();
            qDebug()<<temp;
            item = new QTableWidgetItem(temp);
            ui->info_Wig->setItem(count,4,item);//备注
            item->setFlags(Qt::NoItemFlags);

            m_count += shuliang;    //添加数量
            m_price += zongjia ;    //添加总价
        }

        ui->count_LEdit->setText(QString::number(m_count));         //数量的标签
        ui->price_LEdit->setText(QString::number(m_price));   //总计的标签

        ++ count;
    }
}

void SellForm::on_delete_Btn_clicked()
{
    int flag = ui->info_Wig->currentRow();

    if(flag == -1)
        return ;

    /*首先将价格和数量进行减少*/
    int shuliang = ui->info_Wig->item(flag,1)->text().toInt();

    double xiaoji = ui->info_Wig->item(flag,3)->text().toDouble();

    m_count -= shuliang;                //减少数量
    m_price -= xiaoji ;                 //减少总价

    /*减少显示的价格和数量进行*/
    ui->count_LEdit->setText(QString::number(m_count));         //数量的标签
    ui->price_LEdit->setText(QString::number(m_price));   //总计的标签

    ui->info_Wig->removeRow(ui->info_Wig->currentRow());
}

void SellForm::on_submit_Btn_clicked()
{
    /*没有数量，拒绝打印账单*/
    if(m_count == 0)
        return ;

    /*打印名字和账单数量，总价*/
    m_bill->printCOUNTandPRICE(m_count,m_price);
    m_bill->printName(m_sellerName,QString::number(m_numTrade));

    /*打印商品信息*/
    QVector<QString> name;
    QVector<QString> count;
    QVector<QString> price;
    QVector<QString> lessName;  //数量不足的商品名
    QVector<int> goodsID;
    QSqlQuery query;
    QString temp = "";
    int size = ui->info_Wig->rowCount();
    for(int i = 0 ; i < size ; ++i)
    {
        name.push_back(ui->info_Wig->item(i,0)->text().simplified());
        count.push_back(ui->info_Wig->item(i,1)->text());
        price.push_back(ui->info_Wig->item(i,2)->text());

        /*检测商品的数量够不够*/
        temp = "select ID,Number from Goods where Name='" + name[i] + "'";

        query.exec(temp);
        query.next();

        goodsID.push_back(query.value(0).toInt());

        if(query.value(1).toInt() < count[i].toInt())
            lessName.push_back(name[i]);

    }

    /*如果商品数量不足,退货则不需要检测*/
    if(lessName.size() != 0 && m_kind != 2)
    {
        temp = "以下商品数量不足：";

        foreach(QString ch,lessName)
        {
            temp += "\n";
            temp += ch;
        }
        QMessageBox::information(this,"商品数量不足",temp);
        return ;
    }

    /*展示VIP卡号*/
    if(m_kind == 0)     //如果是销售的
    {
        m_showVIPCard->show();
    }
    else                //批发和退货
    {
        setVIPWindow("",0.0);

        /*将信息打印到交易表和商品表*/
        writeToTrade("");
        writeToSell(goodsID,name,count,price);
    }

    m_bill->printGoods(name,count,price);
    m_bill->setDate(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
}
