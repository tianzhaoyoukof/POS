#ifndef SELLFORM_H
#define SELLFORM_H

#include <QWidget>
#include <QVector>
#include "goodsmessage.h"
#include "bill.h"
#include "showvipcard.h"

/*商品批发，商品零售，商品退货的窗体，被加在POSSystem上
 *2015-11-13 创建
 */

namespace Ui {
class SellForm;
}

class SellForm : public QWidget
{
    Q_OBJECT

public:
    explicit SellForm(QWidget *parent = 0);
    ~SellForm();
    /*设置界面类型*/
    void setKind(int k);
    /*设置用户名*/
    void setSellerName(QString name);

private slots:
    void on_show_Btn_clicked();

    void on_add_Btn_clicked();

    void on_delete_Btn_clicked();

    void on_submit_Btn_clicked();

private:
    Ui::SellForm *ui;
    GoodsMessage* m_goodsMessage;   //显示商品信息的窗口
    Bill* m_bill;                   //账单的窗口
    ShowVIPCard* m_showVIPCard;     //输入会员卡号的窗口
private:
    /*销售员名字*/
    QString m_sellerName;
    /*总价格*/
    double m_price;
    /*总数量*/
    int m_count;
    /*界面，0 零售   1 批发   2 退货*/
    int m_kind;
    /*Sell销售单的数量*/
    int m_numSell;
    /*Trade交易单的数量*/
    int m_numTrade;
private:
    void initSellFrom();    //初始化
    /*将商品销售写到商品销售表*/
    void writeToSell(const QVector<int>& goodsId,const QVector<QString>& name ,
                     const QVector<QString>& count,const QVector<QString>& price);
    /*将交易写到交易表*/
    void writeToTrade(const QString& vipno);
public slots:
    void setVIPWindow(QString no,double money);
    /*重新设窗口*/
    void resetForm();
};

#endif // SELLFORM_H
