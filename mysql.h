#ifndef MYSQL_H
#define MYSQL_H

#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>


class My_Sql
{
    QSqlQuery *query;
public:
    My_Sql();
    void initsql();                                                         //初始化连接数据库
    void createtable();                                                     //建表、索引、触发器
    bool inituser(QString name, QString identify, QString passward);        //注册用户
    bool loguser(QString name, QString passward);                           //用户登录/忘记密码

        bool deletestu(QString account);                                        //删除学生信息
    bool deleteheal(QString account);                                       //删除体检信息
    bool deletegrade(QString account);                                      //删除成绩信息
    bool deletelicen(QString account);                                      //删除领证信息

#if 0
    bool addstu(my_stu* stu);                                               //添加学生信息

    bool updatestu(my_stu* stu);                                            //修改学生信息
    bool addheal(my_heal* heal);                                            //添加体检信息
    bool updateheal(my_heal* heal);                                         //修改体检信息

    bool addgrade(my_grade* grad);                                          //添加成绩信息
    bool updategrade(my_grade* grad);                                       //修改成绩信息

    bool addlicen(my_licen* lice);                                          //添加领证信息
    bool updatelicen(my_licen* lice);                                       //修改领证信息
#endif

};

#endif // MYSQL_H
