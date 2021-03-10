#include "service_selector_window.h"
#include "ui_service_selector_window.h"

#include <appointy_exception.h>

ServiceSelectorWindow::ServiceSelectorWindow(const std::vector<appointy::Service> &services, QWidget *parent) :
    QMainWindow(parent),
    _services {services},
    ui(new Ui::ServiceSelectorWindow)
{
    ui->setupUi(this);
    // disables close button
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    for(auto i = qulonglong {0}; i < _services.size(); i++)
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

void ServiceSelectorWindow::validate() const
{
    if(!_service)
    {
        throw appointy::Exception {"No service selected"};
    }
}

appointy::Service ServiceSelectorWindow::service() const
{
    validate();

    return *_service;
}

void ServiceSelectorWindow::on_select_btn_clicked()
{
    auto selected_items = ui->services->selectedItems();
    auto selected_item = selected_items[0];
    _service = &_services[selected_item->data(Qt::UserRole).toULongLong()];
    close();
}
