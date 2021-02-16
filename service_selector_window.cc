#include "service_selector_window.h"
#include "ui_service_selector_window.h"

ServiceSelectorWindow::ServiceSelectorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServiceSelectorWindow)
{
    ui->setupUi(this);
}

ServiceSelectorWindow::~ServiceSelectorWindow()
{
    delete ui;
}
