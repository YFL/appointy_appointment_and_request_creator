#include "service_selector_window.h"
#include "ui_service_selector_window.h"

ServiceSelectorWindow::ServiceSelectorWindow(const std::vector<appointy::Service> &services, QWidget *parent) :
    QMainWindow(parent),
    _services {services},
    ui(new Ui::ServiceSelectorWindow)
{
    ui->setupUi(this);
    for(qulonglong i = 0; i < _services.size(); i++)
    {
        auto *item = new QListWidgetItem {ui->services};
        item->setData(Qt::UserRole, QVariant {i});
        item->setText(_services[i].name.c_str());
        ui->services->addItem(item);
    }
}


ServiceSelectorWindow::~ServiceSelectorWindow()
{
    delete ui;
}

void ServiceSelectorWindow::on_select_btn_clicked()
{
    auto selected_items = ui->services->selectedItems();
    auto selected_item = selected_items[0];
    emit selected(_services[selected_item->data(Qt::UserRole).toULongLong()]);
}
