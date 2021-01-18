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

private:
    Ui::AppointmentWidget *ui;
};

#endif // APPOINTMENT_WIDGET_H
