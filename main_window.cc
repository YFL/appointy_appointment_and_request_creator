#include "main_window.h"
#include "ui_main_window.h"

#include <QEventLoop>

#include <appointy_exception.h>
#include <../service_creator/util.h>

#include <appointment_request_widget.h>
#include <service_configurator_widget.h>

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

void MainWindow::on_preview_btn_clicked()
{
    auto *widget = dynamic_cast<RequestWidgetBase *>(ui->request_widget->children().at(0));
    if(widget)
    {
        try
        {
            ui->text_browser->setText(widget->to_json().dump().c_str());
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("An error occurred during the retrieval of the preview", e.what());
        }
    }
}
