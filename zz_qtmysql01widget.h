#ifndef ZZ_QTMYSQL01WIDGET_H
#define ZZ_QTMYSQL01WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class zz_Qtmysql01Widget; }
QT_END_NAMESPACE

class zz_Qtmysql01Widget : public QWidget
{
    Q_OBJECT

public:
    zz_Qtmysql01Widget(QWidget *parent = nullptr);
    ~zz_Qtmysql01Widget();


private slots:
    void on_pushButton_MySQLstart_clicked();

private:
    Ui::zz_Qtmysql01Widget *ui;

    void mysql_init001();
    void mysql_call001();
};
#endif // ZZ_QTMYSQL01WIDGET_H
