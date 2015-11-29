#ifndef PERSONFORM_H
#define PERSONFORM_H

/*人员管理的窗口
 *2015-11-24 创建
 */

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class PersonForm;
}

class PersonForm : public QWidget
{
    Q_OBJECT

public:
    explicit PersonForm(QWidget *parent = 0);
    ~PersonForm();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_delVIP_Btn_clicked();

    void on_sort_Btn_clicked();

    void on_changeDept_Btn_clicked();

    void on_addPreson_Btn_clicked();

    void on_changePass_Btn_clicked();

    void on_delPerson_Btn_clicked();

    void on_person_TView_clicked(const QModelIndex &index);

private:
    Ui::PersonForm *ui;
    QSqlQueryModel* m_queryModel;   //查询的函数
private:
    /*记录现在已经注册的员工的数量*/
    int m_numLoginUser;
private:
    void init();
    /*初始化用户界面*/
    void initUser();
    /*初始化会员界面*/
    void initVIP();
};

#endif // PERSONFORM_H
