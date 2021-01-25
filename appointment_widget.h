#ifndef APPOINTMENT_WIDGET_H
#define APPOINTMENT_WIDGET_H

#include "request_widget_base.h"

namespace Ui {
class AppointmentWidget;
}

class AppointmentWidget : public RequestWidgetBase
{
    Q_OBJECT

public:
    explicit AppointmentWidget(QWidget *parent = nullptr);
    ~AppointmentWidget() override;

public:
    auto is_empty() noexcept -> bool override;
    auto clear() noexcept -> void override;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    auto validate() -> void override;
    auto to_json() -> std::string override;

private:
    Ui::AppointmentWidget *ui;
};

#endif // APPOINTMENT_WIDGET_H
