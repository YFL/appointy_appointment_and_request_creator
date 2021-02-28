#include "appointment_request_widget.h"
#include "ui_appointment_request_widget.h"

#include "../service_creator/util.h"
#include <appointment_request.h>
#include <appointy_exception.h>

#include <service_configurator_widget.h>

AppointmentRequestWidget::AppointmentRequestWidget(QWidget *parent) :
    RequestWidgetBase(parent),
    ui(new Ui::AppointmentRequestWidget),
    _service_config_widget {nullptr},
    _service_selector_window {nullptr}
{
    ui->setupUi(this);
}

AppointmentRequestWidget::~AppointmentRequestWidget()
{
    delete ui;
}

auto AppointmentRequestWidget::is_empty() const noexcept -> bool
{
    return
        ui->first_date->text().isEmpty() && ui->interval_end->text().isEmpty() &&
        ui->interval_start->text().isEmpty() && ui->last_date->text().isEmpty() &&
        ui->service_id->text().isEmpty();
}

auto AppointmentRequestWidget::clear() noexcept -> void
{
    ui->first_date->setText({""});
    ui->interval_end->setText({""});
    ui->interval_start->setText({""});
    ui->last_date->setText({""});
    ui->service_id->setText({""});
}

auto AppointmentRequestWidget::validate() const -> void
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

    if(!_service_selector_window)
    {
        throw appointy::Exception {"No service was selected from the loaded services, if any"};
    }

    _service_selector_window->validate();

    if(!_service_config_widget)
    {
        throw appointy::Exception {"The selected service, if any, wasn't configured"};
    }

    _service_config_widget->validate();
}

auto AppointmentRequestWidget::to_json() const -> nlohmann::json
{
    validate();
    return appointy::AppointmentRequest {
        string_to_date(ui->first_date->text().toStdString()),
        string_to_date(ui->last_date->text().toStdString()),
        string_to_time(ui->interval_start->text().toStdString()),
        string_to_time(ui->interval_end->text().toStdString()),
    }.to_json().dump();
}

void AppointmentRequestWidget::on_configure_service_btn_clicked()
{
    delete _service_config_widget;
    delete _service_selector_window;

    _service_config_widget = new ServiceConfiguratorWidget {{}, this};
}
