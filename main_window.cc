#include "main_window.h"
#include "ui_main_window.h"

#include <QEventLoop>

#include <service_configurator_widget.h>

#include <appointment_request_widget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_BookingRequest_triggered()
{

}

void MainWindow::on_save_btn_clicked()
{

}

void MainWindow::on_actionNew_AppointmentRequest_triggered()
{
    change_and_show_request_widget(new AppointmentRequestWidget {this});
}

auto MainWindow::change_and_show_request_widget(QWidget *widget) noexcept -> void
{
    auto children = ui->request_widget->children();
    if(children.size() == 1)
    {
        delete children.at(0);
    }
    widget->setParent(ui->request_widget);
    widget->show();
}
