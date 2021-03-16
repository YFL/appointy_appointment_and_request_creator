#ifndef APPOINTMENT_CONFIGURATION_WIDGET_H
#define APPOINTMENT_CONFIGURATION_WIDGET_H

#include "request_widget_base.h"

#include <answer.h>
#include <service.h>

#include <service_configurator_widget.h>
#include <service_selector_window.h>

namespace Ui {
class AppointmentConfigurationWidget;
}

class AppointmentConfigurationWidget : public RequestWidgetBase
{
    Q_OBJECT

public:
    explicit AppointmentConfigurationWidget(QWidget *parent = nullptr);
    ~AppointmentConfigurationWidget() override;

public:
    //auto is_empty() const noexcept -> bool override;
    auto clear() noexcept -> void override;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    auto validate() const -> void override;
    auto to_json() const -> nlohmann::json override;

private slots:
    void on_load_services_btn_clicked();
    void on_select_service_btn_clicked();
    void on_configure_service_btn_clicked();

    void on_request_duration_btn_clicked();

private:
    Ui::AppointmentConfigurationWidget *ui;

private:
    ServiceConfiguratorWidget *_service_config_widget;
    ServiceSelectorWindow *_service_selector_window;
};

#endif // APPOINTMENT_CONFIGURATION_WIDGET_H
