#ifndef GOODSMESSAGE_H
#define GOODSMESSAGE_H

#include <QWidget>
#include <QVector>

#include <QTableView>

/*展示商品信息的类，以SellForm为父窗口
 *2015-11-18 创建
 */

namespace Ui {
class GoodsMessage;
}

class GoodsMessage : public QWidget
{
    Q_OBJECT

public:
    explicit GoodsMessage(QWidget *parent = 0);
    ~GoodsMessage();

private slots:
    void on_search_Btn_clicked();

private:
    Ui::GoodsMessage *ui;

    QVector<QTableView*> m_toolBOXWidget;      //展示的窗口可变数组指针
private:
    void initWidget();
    /*搜索全部商品信息*/
    void selectAllGoods();

    /*组成搜索商品的语句*/
    void makeSelectQuery(QString& temp);
    void makeSelectQuery(QString &temp,const QString& goods,const QString& lie,const QString& lie1);
};

#endif // GOODSMESSAGE_H
