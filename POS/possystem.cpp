#include "possystem.h"
#include "ui_possystem.h"
#include "form.h"
#include <QDebug>

POSSystem::POSSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::POSSystem),
    m_sellform(NULL),
    m_vipRegister(NULL),
    m_purchaseForm(NULL),
    m_sellManForm(NULL),
    m_stockForm(NULL),
    m_personForm(NULL)
{
    ui->setupUi(this);
    init();
}

POSSystem::~POSSystem()
{
    delete m_sellform;
    delete m_vipRegister;
    if(m_purchaseForm != NULL)
        delete m_purchaseForm;
    if(m_sellManForm != NULL)
        delete m_sellManForm;
    if(m_stockForm != NULL)
        delete m_stockForm;
    if(m_personForm != NULL)
        delete m_personForm;
    delete ui;
}

void POSSystem::changeManager(bool b)
{
    ui->retail_Btn->setEnabled(!b);
    ui->wholesale_Btn->setEnabled(!b);
    ui->returnGood_Btn->setEnabled(!b);
    ui->VIP_Btn->setEnabled(!b);
    ui->purchase_Btn->setEnabled(b);
    ui->sell_Btn->setEnabled(b);
    ui->stock_Btn->setEnabled(b);
    ui->person_Btn->setEnabled(b);
}

void POSSystem::setUserName(QString name)
{
    m_sellform->setSellerName(name);
    qDebug()<<"Name"<<name;
    m_sellform->setSellerName(name);
    ui->user_Label->setText(name);
}

void POSSystem::init()
{
    //m_purchaseForm = new PurchaseForm(ui->w);
    m_sellform = new SellForm(ui->w);
    m_vipRegister = new VIPRegister(ui->w);
    m_sellform->setHidden(true);
    m_vipRegister->setHidden(true);
    m_stockForm = new StockForm(ui->w);
    m_stockForm->hide();
    //m_vipRegister->setParent(ui->w);

}

void POSSystem::on_actionExit_triggered()
{
    emit logouted();
}

void POSSystem::on_retail_Btn_clicked()
{
    m_sellform->setKind(0);
    m_sellform->setHidden(false);
    m_vipRegister->setHidden(true);
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
}

void POSSystem::on_VIP_Btn_clicked()
{
    m_sellform->setHidden(true);
    m_vipRegister->setHidden(false);
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
}

void POSSystem::on_wholesale_Btn_clicked()
{
    m_sellform->setKind(1);
    m_sellform->setHidden(false);
    m_vipRegister->setHidden(true);
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
}

void POSSystem::on_returnGood_Btn_clicked()
{
    m_sellform->setKind(2);
    m_sellform->setHidden(false);
    m_vipRegister->setHidden(true);
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
}

void POSSystem::on_purchase_Btn_clicked()
{
    if(m_purchaseForm == NULL)
    {
        m_purchaseForm = new PurchaseForm(ui->w);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
    m_purchaseForm->setHidden(false);
    m_vipRegister->hide();
    m_sellform->hide();
}

void POSSystem::on_sell_Btn_clicked()
{
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm == NULL)
    {
        m_sellManForm = new SellManForm(ui->w);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
    m_sellManForm->setHidden(false);
    m_vipRegister->hide();
    m_sellform->hide();
}

void POSSystem::on_stock_Btn_clicked()
{
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm == NULL)
    {
        m_stockForm = new StockForm(ui->w);
    }
    if(m_personForm != NULL)
    {
        m_personForm->setHidden(true);
    }
    m_stockForm->setHidden(false);
    m_vipRegister->hide();
    m_sellform->hide();
}

void POSSystem::on_person_Btn_clicked()
{
    if(m_purchaseForm != NULL)
    {
        m_purchaseForm->setHidden(true);
    }
    if(m_sellManForm != NULL)
    {
        m_sellManForm->setHidden(true);
    }
    if(m_stockForm != NULL)
    {
        m_stockForm->setHidden(true);
    }
    if(m_personForm == NULL)
    {
        m_personForm = new PersonForm(ui->w);
    }
    m_personForm->setHidden(false);
    m_vipRegister->hide();
    m_sellform->hide();
}
