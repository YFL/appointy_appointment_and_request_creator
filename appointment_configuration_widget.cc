#include "appointment_configuration_widget.h"
#include "ui_appointment_configuration_widget.h"

#include <QFileDialog>

#include <fstream>

#include "../service_creator/util.h"
#include <appointment_configuration.h>
#include <appointy_exception.h>
#include <io_ops.h>
#include <request_handling.h>

#include <service_configurator_widget.h>
#include <service_selector_window.h>

AppointmentConfigurationWidget::AppointmentConfigurationWidget(QWidget *parent) :
    RequestWidgetBase(parent),
    ui(new Ui::AppointmentConfigurationWidget),
    _service_config_widget {nullptr},
    _service_selector_window {nullptr}
{
    ui->setupUi(this);
}

AppointmentConfigurationWidget::~AppointmentConfigurationWidget()
{
    delete ui;
}

//auto AppointmentRequestWidget::is_empty() const noexcept -> bool
//{
//    return
//        ui->first_date->text().isEmpty() && ui->interval_end->text().isEmpty() &&
//        ui->interval_start->text().isEmpty() && ui->last_date->text().isEmpty() &&
//        ui->service_id->text().isEmpty();
//}

auto AppointmentConfigurationWidget::clear() noexcept -> void
{
    ui->first_date->setText({""});
    ui->interval_end->setText({""});
    ui->interval_start->setText({""});
    ui->last_date->setText({""});
    delete _service_config_widget;
    delete _service_selector_window;
    ui->select_service_btn->setEnabled(false);
    ui->configure_service_btn->setEnabled(false);
}

auto AppointmentConfigurationWidget::validate() const -> void
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

auto AppointmentConfigurationWidget::to_json() const -> nlohmann::json
{
    validate();
    return appointy::AppointmentConfiguration{
        string_to_date(ui->first_date->text().toStdString()),
        string_to_date(ui->last_date->text().toStdString()),
        string_to_time(ui->interval_start->text().toStdString()),
        string_to_time(ui->interval_end->text().toStdString()),
        {
            _service_config_widget->service_id(),
            _service_config_widget->configuration()
        }
    }.to_json();
}

void AppointmentConfigurationWidget::on_load_services_btn_clicked()
{
    auto fod = QFileDialog {};
    fod.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fod.setDefaultSuffix("json");
    //fsd.setDirectory("./");
    fod.setFileMode(QFileDialog::FileMode::ExistingFile);
    fod.setNameFilter("JSON files (*.json)");
    fod.setViewMode(QFileDialog::ViewMode::Detail);
    if(fod.exec())
    {
        auto selected_files = fod.selectedFiles();
        auto selected_file = selected_files[0];
        try
        {
            _service_selector_window = new ServiceSelectorWindow {appointy::load_services_from_json(selected_file.toStdString()), this};
            ui->select_service_btn->setEnabled(true);
            ui->configure_service_btn->setEnabled(false);
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("Error occurred while loading services from JSON file", e.what());
        }
    }
}

void AppointmentConfigurationWidget::on_select_service_btn_clicked()
{
    _service_selector_window->show();
    ui->configure_service_btn->setEnabled(true);
}

void AppointmentConfigurationWidget::on_configure_service_btn_clicked()
{
    if(!_service_config_widget || _service_config_widget->service() != _service_selector_window->service())
    {
        _service_config_widget = new ServiceConfiguratorWidget {_service_selector_window->service(), this};
    }

    _service_config_widget->show();
}

void AppointmentConfigurationWidget::on_request_duration_btn_clicked()
{
    if(!_service_config_widget)
    {
        show_error_with_ok("The service wasn't configured", "Press the Configure Service button to configure the service. If it isn't enabled, you have to load and select a service first");
        return;
    }
    try
    {
        _service_config_widget->validate();
    }
    catch(const appointy::Exception &e)
    {
        show_error_with_ok("The service wasn't configured properly", e.what());
        return;
    }

    ui->estimated_duration_label->setText(("Estimated duration: " + appointy::compute_estimated_duration_of_config(appointy::ServiceConfiguration {_service_config_widget->service_id(), _service_config_widget->configuration()}, "mongodb://localhost", "appointy_db").completion_time.to_string()).c_str());
    ui->estimated_duration_label->adjustSize();
}
