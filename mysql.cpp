#include "mysql.h"
#include <QDebug>

My_Sql::My_Sql()
{
}


#include<QCoreApplication>
#include<QFile>
#include<QSqlError>

void My_Sql::initsql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    //db.setHostName("127.0.0.1");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
   //db.setPassword("szgy@GX*zd#xt_80km!");
    db.setPassword("123456");


   // QString sqlname= QCoreApplication::applicationDirPath();

    //qDebug()<<" sql path:"<< sqlname;

//   QString sqlname = QCoreApplication::applicationDirPath()+ "/" +"stuaaaaa.sql";

    //QString sqlname = "./stuaaaaa.sql";
    QString sqlname = "test444";

   qDebug()<<" sql path:"<< sqlname;

#if 0
   QFile sqlnamefile(sqlname);

   if(sqlnamefile.exists())
   {
        qDebug() << "database file exist!";
   }
   else
   {
        qDebug() << "error database file NOT　exist!";
   }
#endif

    db.setDatabaseName(sqlname);
    if(db.open())
     {
         qDebug() << "database is established!";
         createtable();
         return;
     }
     else
     {
         qDebug() << "database open error!" << db.lastError();
         return;
    }
}

void My_Sql::createtable()
{
    query = new QSqlQuery;
    /*用户表*/
    query->exec("create table user(name VARCHAR(30) UNIQUE NOT NULL, passward VARCHAR(30), identify VARCHAR(30) PRIMARY KEY)");
    /*学生信息表*/
    query->exec("create table studentInfo(stuaccount VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL, stuname VARCHAR(30), stusex ENUM('男', '女') NOT NULL, stuage INT, identify VARCHAR(30) UNIQUE NOT NULL, tel VARCHAR(20), enroll_time DATE, leave_time DATE, scondition enum('学习', '结业', '退学'), stu_text TEXT)");
    /*体检信息表*/
    query->exec("create table healthInfo(stuaccount VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL, stuname VARCHAR(30) NOT NULL, height FLOAT, weight FLOAT, differentiate ENUM('正常', '色弱', '色盲'), left_sight FLOAT, right_sight FLOAT, left_ear ENUM('正常','偏弱'), right_ear ENUM('正常','偏弱'), legs ENUM('正常','不相等'), pressure ENUM('正常','偏高','偏低'), history VARCHAR(50), h_text TEXT)");
    /*考试课程表*/
    query->exec("create table courseInfo(classid INT PRIMARY KEY UNIQUE NOT NULL AUTO_INCREMENT, classname VARCHAR(30), beforeclassid INT)");
    /*考试成绩表*/
    query->exec("create table gradeInfo(stuaccount VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL, classid INT NOT NULL, lasttime date, times INT default 1, grade float default 0, constraint grade_acc_fk FOREIGN KEY(stuaccount) REFERENCES studentInfo(stuaccount), constraint grade_cid_fk FOREIGN KEY(classid) REFERENCES courseInfo(classid))");
    /*领证时间表*/
    query->exec("create table licenseInfo(stuaccount VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL, stuname VARCHAR(30), recv_time date, recv_name VARCHAR(30), l_text TEXT);");

    /*创建root用户*/
    query->exec("insert into user value('root', 'root', 'root')");

    /*学生表姓名索引*/
    query->exec("create index int_name ON studentInfo(stuname)");
    /*学生表学习状态索引*/
    query->exec("alter table studentInfo ADD INDEX ind_con(scondition)");
    /*体检表姓名索引*/
    query->exec("create index ind_hname ON healthInfo(stuname)");
    /*领证表姓名索引*/
    query->exec("alter table licenseInfo ADD INDEX ind_lname(stuname)");
    /*领证表领证人索引*/
    query->exec("alter table licenseInfo ADD INDEX ind_recvname(recv_name)");

    /*设计视图*/
    query->exec("create VIEW grade_view AS SELECT g.stuaccount, s.stuname, c.classname, g.lasttime, g.times, g.grade FROM studentinfo s, courseinfo c, gradeinfo g WHERE g.stuaccount = s.stuaccount AND g.classid = c.classid");

    /*设计insert触发器*/
    query->exec("CREATE TRIGGER license_stu AFTER INSERT ON licenseInfo  FOR EACH ROW BEGIN UPDATE studentInfo SET leave_time=NEW.recv_time, scondition = '结业' WHERE stuaccount = NEW.stuaccount; END");

    /*设计update触发器*/
    query->exec("CREATE TRIGGER update_name AFTER UPDATE ON studentinfo FOR EACH ROW BEGIN UPDATE healthinfo SET stuname = NEW.stuname WHERE stuaccount = NEW.stuaccount;END");

    /*设计delete触发器*/
    query->exec("CREATE TRIGGER delete_stu AFTER DELETE ON studentinfo FOR EACH ROW BEGIN DELETE FROM gradeinfo WHERE stuaccount = OLD.stuaccount; DELETE FROM healthinfo WHERE stuaccount = OLD.stuaccount; DELETE FROM licenseinfo WHERE stuaccount = OLD.stuaccount;END");
}

bool My_Sql::inituser(QString name, QString identify, QString passward)
{
    query = new QSqlQuery;
    QString str = QString("insert into user value('%1', '%2', '%3')").arg(name).arg(identify).arg(passward);
    bool ret = query->exec(str);
    return ret;
}

bool My_Sql::loguser(QString name, QString passward)
{
    query = new QSqlQuery;
    QString str = QString("select * from user where name = '%1' and passward = '%2'").arg(name).arg(passward);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
//    MainWindow* MW = MainWindow::getIntence();
//    MW->passward = query->value(1).toString();
    return true;
}

#if 0
bool My_Sql::addstu(stuInfo *stu)
{
    query = new QSqlQuery;
    QString str = QString("insert into studentInfo value('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8', '%9', '%10')")
            .arg(stu->account).arg(stu->name).arg(stu->sex).arg(stu->age).arg(stu->identify).arg(stu->tel).arg(stu->enroll_time)
            .arg(stu->leave_time).arg(stu->scondition).arg(stu->text);
    bool ret = query->exec(str);
    return ret;
}



bool My_Sql::updatestu(my_stu *stu)
{
    query = new QSqlQuery;
    QString str = QString("select * from studentInfo where stuaccount = '%1'").arg(stu->account);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("update studentInfo set stuname = '%1', stusex = '%2', stuage = %3, identify = '%4', tel = '%5', enroll_time = '%6', leave_time = '%7', scondition = '%8', stu_text = '%9' where stuaccount = '%10'")
            .arg(stu->name).arg(stu->sex).arg(stu->age).arg(stu->identify).arg(stu->tel).arg(stu->enroll_time).arg(stu->leave_time).arg(stu->scondition).arg(stu->text).arg(stu->account);
    qDebug()<<str;
    bool ret = query->exec(str);
    return ret;
}

bool My_Sql::addheal(my_heal *heal)
{
    query = new QSqlQuery;
    QString str = QString("insert into healthInfo value('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13')")
            .arg(heal->stuaccount).arg(heal->stuname).arg(heal->height).arg(heal->weight).arg(heal->differentiate).arg(heal->left_sight).arg(heal->right_sight)
            .arg(heal->left_ear).arg(heal->right_ear).arg(heal->legs).arg(heal->pressure).arg(heal->history).arg(heal->h_text);
    bool ret = query->exec(str);
    return ret;
}

bool My_Sql::updateheal(my_heal *heal)
{
    query = new QSqlQuery;
    QString str = QString("select * from healthInfo where stuaccount = '%1'").arg(heal->stuaccount);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("update healthInfo set stuname = '%1', height = '%2', weight = '%3', differentiate = '%4', left_sight = '%5', right_sight = '%6', left_ear = '%7', right_ear = '%8', legs = '%9', pressure = '%10', history = '%11', h_text = '%12' where stuaccount = '%13'")
            .arg(heal->stuname).arg(heal->height).arg(heal->weight).arg(heal->differentiate).arg(heal->left_sight).arg(heal->right_sight)
            .arg(heal->left_ear).arg(heal->right_ear).arg(heal->legs).arg(heal->pressure).arg(heal->history).arg(heal->h_text).arg(heal->stuaccount);
    qDebug()<<str;
    bool ret = query->exec(str);
    qDebug()<<ret;
    return ret;
}



bool My_Sql::addgrade(my_grade *grad)
{
    query = new QSqlQuery;
    QString str = QString("insert into gradeInfo value('%1', %2, %3, %4, %5)")
            .arg(grad->stuaccount).arg(grad->classid).arg(grad->lasttime).arg(grad->times).arg(grad->grade);
    qDebug()<<str;
    bool ret = query->exec(str);
    return ret;
}

bool My_Sql::updategrade(my_grade *grad)
{
    query = new QSqlQuery;
    QString str = QString("select * from gradeInfo where stuaccount = '%1'").arg(grad->stuaccount);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("update gradeInfo set classid = '%1', lasttime = %2, times = %3, grade = %4 where stuaccount = '%5'")
            .arg(grad->classid).arg(grad->lasttime).arg(grad->times).arg(grad->grade).arg(grad->stuaccount);

    qDebug()<<str;
    bool ret = query->exec(str);
    return ret;
}



bool My_Sql::addlicen(my_licen *lice)
{
    query = new QSqlQuery;
    QString str = QString("insert into licenseinfo value('%1', '%2', '%3', '%4', '%5')")
            .arg(lice->stuaccount).arg(lice->stuname).arg(lice->recv_time).arg(lice->recv_name).arg(lice->l_text);

    bool ret = query->exec(str);
    return ret;
}

bool My_Sql::updatelicen(my_licen *lice)
{
    query = new QSqlQuery;
    QString str = QString("select * from licenseinfo where stuaccount = '%1'").arg(lice->stuaccount);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("update licenseinfo set stuname = '%1', recv_time = '%2', recv_name = '%3', l_text = '%4' where stuaccount = '%5'")
            .arg(lice->stuname).arg(lice->recv_time).arg(lice->recv_name).arg(lice->l_text).arg(lice->stuaccount);

    bool ret = query->exec(str);
    return ret;
}
#endif

bool My_Sql::deletegrade(QString account)
{
    query = new QSqlQuery;
    QString str = QString("select * from gradeInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("delete from gradeInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    return true;
}

bool My_Sql::deletelicen(QString account)
{
    query = new QSqlQuery;
    QString str = QString("select * from licenseinfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("delete from licenseinfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    return true;
}

bool My_Sql::deleteheal(QString account)
{
    query = new QSqlQuery;
    QString str = QString("select * from healthInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("delete from healthInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    return true;
}




bool My_Sql::deletestu(QString account)
{
    query = new QSqlQuery;
    QString str = QString("select * from studentInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    query->last();
    int record = query->at() + 1;
    if(record == 0)
    {
        return false;
    }
    str = QString("delete from studentInfo where stuaccount = '%1'").arg(account);
    query->exec(str);
    return true;
}
