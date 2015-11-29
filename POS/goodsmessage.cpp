#include "goodsmessage.h"
#include "ui_goodsmessage.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>

GoodsMessage::GoodsMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoodsMessage),
    m_toolBOXWidget()
{
    ui->setupUi(this);
    initWidget();
}

GoodsMessage::~GoodsMessage()
{
    delete ui;
}

void GoodsMessage::initWidget()
{
    /*初始化分类*/
    QSqlQuery query;
    query.exec("select Name from GoodsKind");
    int coun = 0;
    ui->toolBox->removeItem(0);
    QTableView* view0 = new QTableView();
    m_toolBOXWidget.push_back(view0);
    ui->toolBox->insertItem(++ coun,view0,"全部商品"); //添加全部商品的View
    selectAllGoods();

    while(query.next())
    {
        QString s = query.value(0).toString();
        s = s.simplified();
        QString s1 = "select Name as '名称',Bar as '条形码',RPrice as '零售价'";
        s1 += ",WPrice as '批发价',PPrice as '促销价',SDate as '促销起日期',";
        s1 += "EDate as '促销止日期',Number as '库存数量',Saleable as '允许销售',";
        s1 += "FirmID as '厂商编号',SupplierID as '	供货商编号'";
        s1 += "from Goods where Kind=" + QString::number(coun);
        QSqlQueryModel* m1 = new QSqlQueryModel();
        m1->setQuery(s1);

        QTableView* view = new QTableView();
        m_toolBOXWidget.push_back(view);
        m_toolBOXWidget[coun]->setModel(m1);
        ui->toolBox->insertItem(++ coun,view,s); //添加某个商品分类的View
        //ui->toolBox->insertItem(++ coun,ui->tableView,s);
    }
}

void GoodsMessage::selectAllGoods()
{
    QSqlQueryModel* m = new QSqlQueryModel();
    QString temp = "";
    makeSelectQuery(temp);
    m->setQuery(temp);
    m_toolBOXWidget[0]->setModel(m);
}

void GoodsMessage::makeSelectQuery(QString &temp)
{
   temp = "select Name as '名称',Bar as '条形码',RPrice as '零售价'";
   temp += ",WPrice as '批发价',PPrice as '促销价',SDate as '促销起日期',";
   temp += "EDate as '促销止日期',Number as '库存数量',Saleable as '允许销售',";
   temp += "FirmID as '厂商编号',SupplierID as '	供货商编号',";
   temp += "(select Name from GoodsKind where ID = A.kind) as '分类' from Goods A";
}

void GoodsMessage::makeSelectQuery(QString &temp,
                                   const QString &goods,
                                   const QString &lie,
                                   const QString &lie1)
{
    makeSelectQuery(temp);
    temp += " where " + lie + " like '%" + goods + "%' or ";
    temp += lie1 + " like '%" + goods + "%'";
}

void GoodsMessage::on_search_Btn_clicked()
{
    /*设置最开始的页为当前页*/
    ui->toolBox->setCurrentIndex(0);

    /*空的搜索关键字，返回全部商品信息*/
    if(ui->key_LEdit->text() == "")
    {
        selectAllGoods();
        return ;
    }


    /*可以进行搜索，在商品名称和条形码中搜索*/
    QSqlQueryModel* m = new QSqlQueryModel();
    QString temp = "";
    makeSelectQuery(temp,ui->key_LEdit->text(),"Name","Bar");
    //qDebug()<<temp;
    m->setQuery(temp);
    m_toolBOXWidget[0]->setModel(m);
}
