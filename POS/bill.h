#ifndef BILL_H
#define BILL_H

#include <QWidget>
#include <QVector>

/*账单的类，用于显示收营员结账后的物品单
 *2015-11-19 创建
 */

namespace Ui {
class Bill;
}

class Bill : public QWidget
{
    Q_OBJECT

public:
    explicit Bill(QWidget *parent = 0);
    ~Bill();
public:
    /*打印收银员名字*/
    void printName(QString name,QString tradeNo);
    /*打印总价和数量*/
    void printCOUNTandPRICE(int count,double price);
    /*打印商品信息*/
    void printGoods(QVector<QString>& n,QVector<QString>& c,QVector<QString>& p);
    /*设置日期*/
    void setDate(const QString& date);
    /*设置会员卡号*/
    void setVIPNo(const QString& date);
    /*设置应付*/
    void setPay(QString pay);
private slots:
    void on_close_Btn_clicked();

private:
    Ui::Bill *ui;
private:
    void initBill();
signals:
    void resetForm();
};

#endif // BILL_H
