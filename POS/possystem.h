#ifndef POSSYSTEM_H
#define POSSYSTEM_H

/*POS前台和后台管理系统的主界面，前台供员工使用，后台管理由管理员使用，一些按钮POS前台无法使用
 *2015-11-13 创建
 */

#include <QMainWindow>
#include "sellform.h"
#include "vipregister.h"
#include "purchaseform.h"
#include "stockform.h"
#include "sellmanform.h"
#include "personform.h"

namespace Ui {
class POSSystem;
}

class POSSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit POSSystem(QWidget *parent = 0);
    ~POSSystem();

    /*变成管理员模式，一些按钮可以点击*/
    void changeManager(bool b);

public slots:
    /*设置用户名*/
    void setUserName(QString name);

private slots:
    void on_actionExit_triggered();

    void on_retail_Btn_clicked();

    void on_VIP_Btn_clicked();

    void on_wholesale_Btn_clicked();

    void on_returnGood_Btn_clicked();

    void on_purchase_Btn_clicked();

    void on_sell_Btn_clicked();

    void on_stock_Btn_clicked();

    void on_person_Btn_clicked();

private:
    Ui::POSSystem *ui;

    SellForm *m_sellform;           //销售窗口
    VIPRegister* m_vipRegister;     //VIP注册窗口
    PurchaseForm* m_purchaseForm;   //进货窗口
    SellManForm* m_sellManForm;     //销售管理窗口
    StockForm* m_stockForm;         //库存管理窗口
    PersonForm* m_personForm;       //人员管理窗口

    void init();                 //初始化
signals:
    /*登出界面*/
    void logouted();
};

#endif // POSSYSTEM_H
