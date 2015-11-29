#include "uselogin.h"
#include "manager.h"
#include "purchaseform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Manager u;
    //PurchaseForm h;
    //h.show();

    return a.exec();
}
