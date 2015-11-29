#ifndef VIPREGISTER_H
#define VIPREGISTER_H

#include <QWidget>

/*VIP 注册的类，用于注册会员
 *2015-11-17 创建
 */

namespace Ui {
class VIPRegister;
}

class VIPRegister : public QWidget
{
    Q_OBJECT

public:
    explicit VIPRegister(QWidget *parent = 0);
    ~VIPRegister();

private slots:
    void on_close_Btn_clicked();

    void on_register_Btn_clicked();

private:
    Ui::VIPRegister *ui;
private:
    /*目前已有的会员数最高卡号*/
    int m_maxVIPNo;
private:
    void initVIP();
};

#endif // VIPREGISTER_H
