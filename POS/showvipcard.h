#ifndef SHOWVIPCARD_H
#define SHOWVIPCARD_H

#include <QWidget>

namespace Ui {
class ShowVIPCard;
}

class ShowVIPCard : public QWidget
{
    Q_OBJECT

public:
    explicit ShowVIPCard(QWidget *parent = 0);
    ~ShowVIPCard();

private slots:
    void on_sure_Btn_clicked();

    void on_cancel_Btn_clicked();

private:
    Ui::ShowVIPCard *ui;
signals:
    void showed(QString no,double money);
};

#endif // SHOWVIPCARD_H
