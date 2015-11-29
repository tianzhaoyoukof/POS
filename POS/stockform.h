#ifndef STOCKFORM_H
#define STOCKFORM_H

/*库存管理的窗口
 *2015-11-24 创建
 */

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class StockForm;
}

class StockForm : public QWidget
{
    Q_OBJECT

public:
    explicit StockForm(QWidget *parent = 0);
    ~StockForm();

private slots:
    void on_search_Btn_clicked();
    void on_lnum_LWidget_currentRowChanged(int currentRow);

    void on_purchase_Btn_clicked();

private:
    Ui::StockForm *ui;
    QSqlQueryModel* m_queryModel;
private:
    int m_stock;    //进货单数量
private:
    void init();
    /*初始化商品信息表*/
    void initMSG();
    /*数量不足报警*/
    void lowNumber();
};

#endif // STOCKFORM_H
