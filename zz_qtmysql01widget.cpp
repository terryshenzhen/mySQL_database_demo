#include "zz_qtmysql01widget.h"
#include "ui_zz_qtmysql01widget.h"

zz_Qtmysql01Widget::zz_Qtmysql01Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::zz_Qtmysql01Widget)
{
    mysql_init001();
    ui->setupUi(this);
}

zz_Qtmysql01Widget::~zz_Qtmysql01Widget()
{
    delete ui;
}

#include<QMessageBox>
#include "mysql.h"

void zz_Qtmysql01Widget::mysql_init001()
{
    My_Sql MS;
    MS.initsql();

}

void zz_Qtmysql01Widget::mysql_call001()
{
    QString  name("root");
    QString  passward("root");

    My_Sql* mysql = new My_Sql;
    bool ret = mysql->loguser(name, passward);
    if(ret == false)
    {
        QMessageBox::information(this, tr("消息"), tr("账号密码不匹配!"), QMessageBox::Ok);
        return;
    }
    QMessageBox::information(this, tr("消息"), tr("登录成功!"), QMessageBox::Ok);
    return;
}




void zz_Qtmysql01Widget::on_pushButton_MySQLstart_clicked()
{
    mysql_call001();
}

