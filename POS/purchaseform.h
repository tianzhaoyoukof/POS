#ifndef PURCHASEFORM_H
#define PURCHASEFORM_H

/*进货窗体，被加在POSSystem上
 *2015-11-23 创建
 */
#include "addfirm_supplier.h"

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class PurchaseForm;
}

class PurchaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseForm(QWidget *parent = 0);
    ~PurchaseForm();

private slots:
    void on_addKind_Btn_clicked();

    void on_delKind_Btn_clicked();

    void on_addFirm_Btn_clicked();

    void on_delFirm_Btn_clicked();

    void on_addSupplier_Btn_clicked();

    void on_delSupplier_Btn_clicked();

    void on_sure_Btn_clicked();

    void on_sure_Btn_3_clicked();

    void on_sure_Btn_4_clicked();

    void on_sort_Btn_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::PurchaseForm *ui;
    AddFirm_Supplier* m_addFirm;//
    QSqlQueryModel* m_queryModel;   //查询的结果模式
private:
    /*记录全部商品的数量*/
    int m_goods;
    /*记录商品的种类*/
    int m_goodsKind;
    /*产商的数量*/
    int m_firm;
    /*供应商的数量*/
    int m_supplier;
    /*销售单的数量*/
    int m_stock;
    /*添加的是不是厂商，是不是厂商就是供应商*/
    bool m_Firm_or_Supplier;
private:
    void init();
    /*删除厂商或者供应商*/
    void delFirm_or_Supplier();
    /*处理销售单的开始函数*/
    void initStock();
public slots:
    /*添加供应商或者厂商*/
    void add(QString name,QString address,QString phone);
};

#endif // PURCHASEFORM_H
