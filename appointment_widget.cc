#include "appointment_widget.h"
#include "ui_appointment_widget.h"

#include <QFileDialog>

#include <appointy_exception.h>

#include <../service_creator/util.h>

AppointmentWidget::AppointmentWidget(QWidget *parent) :
    RequestWidgetBase {parent},
    ui(new Ui::AppointmentWidget),
    appointment_request_widget {nullptr}
{
    ui->setupUi(this);
}

AppointmentWidget::~AppointmentWidget()
{
    delete ui;
}

void AppointmentWidget::clear() noexcept
{
    ui->id->setText("");
    ui->date->setText("");
    ui->start->setText("");
    ui->end->setText("");
    appointment_request_widget = nullptr;
    appointment_request = nullptr;
}

void AppointmentWidget::validate() const
{
    try
    {
        string_to_date(ui->date->text().toStdString());
    }
    catch (const appointy::Exception &)
    {
        throw appointy::Exception {"The date's format is wrong"};
    }

    try
    {
        string_to_time(ui->start->text().toStdString());
    }
    catch (const appointy::Exception &)
    {
        throw appointy::Exception {"The start time's format is wrong"};
    }

    try
    {
        string_to_time(ui->end->text().toStdString());
    }
    catch (const appointy::Exception &)
    {
        throw appointy::Exception {"The end time's format is wrong"};
    }

    if(!appointment_request_widget && !appointment_request)
    {
        throw appointy::Exception {"No request loaded or created"};
    }

    if(appointment_request_widget)
    {
        appointment_request_widget->validate();
    }
}

nlohmann::json AppointmentWidget::to_json() const
{
    validate();
    auto json = "{}"_json;
    json["id"] = ui->id->text().toStdString();
    json["date"] = string_to_date(ui->date->text().toStdString()).to_json();
    json["start"] = string_to_time(ui->start->text().toStdString()).to_json();
    json["end"] = string_to_time(ui->end->text().toStdString()).to_json();
    if(appointment_request)
    {
        json["request"] = appointment_request->to_json();
    }
    if(appointment_request_widget)
    {
        json["request"] = appointment_request_widget->to_json();
    }

    return json;
}

void AppointmentWidget::on_load_request_btn_clicked()
{
    auto fod = QFileDialog {};
    fod.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fod.setNameFilter("JSON files (*.json)");
    fod.setFileMode(QFileDialog::FileMode::ExistingFile);
    fod.setViewMode(QFileDialog::ViewMode::Detail);
    if(fod.exec())
    {
        auto selected_file = fod.selectedFiles().value(0);
        if(selected_file.isEmpty())
        {
            show_error_with_ok("No file selected", "You have to select a file to be able to load it");
        }
    }
}
