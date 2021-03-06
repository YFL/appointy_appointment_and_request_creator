#include "main_window.h"
#include "ui_main_window.h"

#include <fstream>

#include <QEventLoop>
#include <QFileDialog>

#include <appointy_exception.h>
#include <../service_creator/util.h>

#include <appointment_widget.h>
#include <appointment_configuration_widget.h>
#include <booking_request_widget.h>
#include <service_configurator_widget.h>

auto strip_unnecessary_from_json_dump(const std::string &string) -> std::string
{
    auto json = std::string {};
    std::copy_if(string.begin(), string.end(), std::back_inserter(json), [](auto x) {return x != '\\';});
    if(json[0] == '"')
    {
        json = json.substr(1, json.size() - 2);
    }

    return json;
}

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
    change_and_show_request_widget(new BookingRequestWidget {this});
}

void MainWindow::on_save_btn_clicked()
{
    if(request_widget())
    {
        auto fsd = QFileDialog {};
        fsd.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
        fsd.setFileMode(QFileDialog::FileMode::AnyFile);
        fsd.setNameFilter("JSON files (*.json)");
        fsd.setDefaultSuffix("json");
        fsd.setViewMode(QFileDialog::ViewMode::Detail);
        if(fsd.exec())
        {
            if(fsd.selectedFiles().size() > 0)
            {
                auto selected_files = fsd.selectedFiles();
                auto selected_file = selected_files[0];
                std::ofstream file {selected_file.toStdString()};
                if(file.is_open())
                {
                    file << strip_unnecessary_from_json_dump(request_widget()->to_json().dump());
                }
                else
                {
                    show_error_with_ok("Couldn't open the file", "");
                }
            }
            else
            {
                show_error_with_ok("No file selected", "You have to select a file, to be able to save");
            }
        }
    }
    else
    {
        show_error_with_ok("No widget", "There is no request widget to save (or more than one)");
    }
}

void MainWindow::on_actionNew_AppointmentConfiguration_triggered()
{
    change_and_show_request_widget(new AppointmentConfigurationWidget {this});
}

auto MainWindow::change_and_show_request_widget(QWidget *widget) noexcept -> void
{
    auto r_widget = request_widget();
    if(r_widget)
    {
        delete r_widget;
    }
    widget->setParent(ui->request_widget);
    widget->show();
}

auto MainWindow::request_widget() const noexcept -> const RequestWidgetBase *
{
    if(ui->request_widget->children().size() == 1)
    {
        return dynamic_cast<RequestWidgetBase *>(ui->request_widget->children().at(0));
    }

    return nullptr;
}

void MainWindow::on_preview_btn_clicked()
{
    auto *widget = request_widget();
    if(widget)
    {
        try
        {
            ui->text_browser->setText(strip_unnecessary_from_json_dump(widget->to_json().dump()).c_str());
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("An error occurred during the retrieval of the preview", e.what());
        }
    }
}

void MainWindow::on_actionNew_Appointment_triggered()
{
    change_and_show_request_widget(new AppointmentWidget {this});
}
