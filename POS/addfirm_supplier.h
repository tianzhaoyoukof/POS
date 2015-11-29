#ifndef ADDFIRM_SUPPLIER_H
#define ADDFIRM_SUPPLIER_H
/* 添加供应商或厂商的窗口
 * 2015-11-23 创建
 */
#include <QDialog>

namespace Ui {
class AddFirm_Supplier;
}

class AddFirm_Supplier : public QDialog
{
    Q_OBJECT

public:
    explicit AddFirm_Supplier(QWidget *parent = 0);;
    ~AddFirm_Supplier();

private slots:
    void on_add_Btn_clicked();

private:
    Ui::AddFirm_Supplier *ui;
private:
    void init();
signals:
    void add(QString name ,QString address,QString phone);
};

#endif // ADDFIRM_SUPPLIER_H
