#ifndef APPOINTMENT_REQUEST_WIDGET_H
#define APPOINTMENT_REQUEST_WIDGET_H

#include "request_widget_base.h"

#include <answer.h>
#include <service.h>

namespace Ui {
class AppointmentRequestWidget;
}

class AppointmentRequestWidget : public RequestWidgetBase
{
    Q_OBJECT

public:
    explicit AppointmentRequestWidget(QWidget *parent = nullptr);
    ~AppointmentRequestWidget() override;

public:
    auto is_empty() noexcept -> bool override;
    auto clear() noexcept -> void override;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    auto validate() -> void override;
    auto to_json() -> std::string override;

private slots:
    void on_configure_service_btn_clicked();
    void on_service_config_ready(std::vector<appointy::Answer>);

private:
    Ui::AppointmentRequestWidget *ui;

private:
    std::vector<std::optional<appointy::Service>> _services;
    std::optional<unsigned long> _selected_service_index;
    std::vector<appointy::Answer> _answers;
};

#endif // APPOINTMENT_REQUEST_WIDGET_H
