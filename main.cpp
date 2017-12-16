#include "MainWindow.h"
#include "Db.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    auto db = new Db("main.db");
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    w->show();
    delete db;
    return a.exec();
}
