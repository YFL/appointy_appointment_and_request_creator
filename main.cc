#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("border: 1px solid red;");
    w.show();
    return a.exec();
}
