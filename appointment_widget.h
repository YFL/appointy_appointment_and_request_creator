#ifndef APPOINTMENT_WIDGET_H
#define APPOINTMENT_WIDGET_H

#include <appointment_request.h>
#include <appointment_request_widget.h>
#include <request_widget_base.h>

namespace Ui {
class AppointmentWidget;
}

class AppointmentWidget : public RequestWidgetBase
{
    Q_OBJECT

public:
    explicit AppointmentWidget(QWidget *parent = nullptr);
    virtual ~AppointmentWidget();

public:
    virtual auto clear() noexcept -> void;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    virtual auto validate() const -> void;
    virtual auto to_json() const -> nlohmann::json;

private slots:
    void on_load_request_btn_clicked();

private:
    Ui::AppointmentWidget *ui;

private:
    std::unique_ptr<AppointmentRequestWidget> appointment_request_widget;
    std::unique_ptr<appointy::AppointmentRequest> appointment_request;
};

#endif // APPOINTMENT_WIDGET_H
