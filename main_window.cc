#include "main_window.h"
#include "ui_main_window.h"

#include <QEventLoop>

#include <service_configurator_widget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , service_configurator { new ServiceConfiguratorWidget {this}}
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_Appointment_triggered()
{

}

void MainWindow::on_save_btn_clicked()
{

}

void MainWindow::on_actionNew_AppointmentRequest_triggered()
{
    service_configurator->show();
}
