#include "appointment_widget.h"
#include "ui_appointment_widget.h"

#include <fstream>

#include <QFileDialog>

#include <appointy_exception.h>
#include <io_ops.h>
#include <json_parser.h>
#include <request_handling.h>

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
            return;
        }

        try
        {
            auto file_contents = appointy::open_file_to_string(selected_file.toStdString());
            auto json = nlohmann::json::parse(file_contents);
            appointment_request = std::unique_ptr<appointy::AppointmentRequest>(new appointy::AppointmentRequest {appointy::JSON_Parser::parse_appointment_request(json)});
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("Couldn't open file", e.what());
        }
        catch(const nlohmann::detail::parse_error &e)
        {
            show_error_with_ok("Error while parsing string in JSON format", e.what());
        }
    }
}

void AppointmentWidget::on_request_duration_btn_clicked()
{
    if(!appointment_request)
    {
        show_error_with_ok("No request loaded", "You have to load a request first by clicking on the \"Load AppointmentRequest from JSON\" button");
        return;
    }

    ui->estimated_duration_label->setText(("Estimated duration: " + appointy::accept_estimated_duration_request({appointment_request->service_id, appointment_request->answers}, "mongodb://localhost", "appointy_db").to_string()).c_str());
}
