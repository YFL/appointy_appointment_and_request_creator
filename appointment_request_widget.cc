#include "appointment_request_widget.h"
#include "ui_appointment_request_widget.h"

#include "../service_creator/util.h"
#include <appointment_request.h>
#include <appointy_exception.h>

AppointmentRequestWidget::AppointmentRequestWidget(QWidget *parent) :
    RequestWidgetBase(parent),
    ui(new Ui::AppointmentRequestWidget)
{
    ui->setupUi(this);
}

AppointmentRequestWidget::~AppointmentRequestWidget()
{
    delete ui;
}

bool AppointmentRequestWidget::is_empty() noexcept
{
    return
            ui->first_date->text().isEmpty() && ui->interval_end->text().isEmpty() &&
            ui->interval_start->text().isEmpty() && ui->last_date->text().isEmpty() &&
            ui->service_id->text().isEmpty();
}

void AppointmentRequestWidget::clear() noexcept
{
    ui->first_date->setText({""});
    ui->interval_end->setText({""});
    ui->interval_start->setText({""});
    ui->last_date->setText({""});
    ui->service_id->setText({""});
}

void AppointmentRequestWidget::validate()
{

    try
    {
        string_to_date(ui->first_date->text().toStdString());
    }
    catch(const appointy::Exception &e)
    {
        throw appointy::Exception {err_msg("First date", e.what())};
    }


    try
    {
        string_to_time(ui->interval_end->text().toStdString());
    }
    catch (const appointy::Exception &e)
    {
        throw appointy::Exception {err_msg("Interval end", e.what())};
    }

    try
    {
        string_to_time(ui->interval_start->text().toStdString());
    }
    catch (const appointy::Exception &e)
    {
        throw appointy::Exception {err_msg("Interval start", e.what())};
    }

    try
    {
        string_to_date(ui->last_date->text().toStdString());
    }
    catch (const appointy::Exception &e)
    {
        throw appointy::Exception {err_msg("Last date", e.what())};
    }

    if(ui->service_id->text().isEmpty())
    {
        throw appointy::Exception {"The " + ui->service_id_label->text().toStdString() + " can not be left empty"};
    }
}

std::string AppointmentRequestWidget::to_json()
{
    validate();
    return appointy::AppointmentRequest {
        string_to_date(ui->first_date->text().toStdString()),
        string_to_date(ui->last_date->text().toStdString()),
        string_to_time(ui->interval_start->text().toStdString()),
        string_to_time(ui->interval_end->text().toStdString()),
        ui->service_id->text().toStdString(),
        {}
    }.to_json().dump();
}
