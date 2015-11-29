#ifndef SELLMANFORM_H
#define SELLMANFORM_H

/*销售管理的窗口
 *2015-11-24 创建
 */

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class SellManForm;
}

class SellManForm : public QWidget
{
    Q_OBJECT

public:
    explicit SellManForm(QWidget *parent = 0);
    ~SellManForm();

private slots:
    void on_sortSELL_Btn_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_sortTRADE_Btn_clicked();

    void on_delSell_Btn_clicked();

    void on_delTrade_Btn_clicked();

    void on_showAllTrade_Btn_clicked();

private:
    Ui::SellManForm *ui;
    QSqlQueryModel* m_queryModel;   //查询的函数
private:
    void init();
    /*初始化销售单界面*/
    void initSell();
    /*初始化交易单界面*/
    void initTrade();
};

#endif // SELLMANFORM_H
