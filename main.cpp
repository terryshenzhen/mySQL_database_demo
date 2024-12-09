#include "zz_qtmysql01widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    zz_Qtmysql01Widget w;
    w.show();
    return a.exec();
}
