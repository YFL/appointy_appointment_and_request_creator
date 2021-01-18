#ifndef APPOINTMENT_REQUEST_WIDGET_H
#define APPOINTMENT_REQUEST_WIDGET_H

#include <QWidget>

namespace Ui {
class AppointmentRequestWidget;
}

class AppointmentRequestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentRequestWidget(QWidget *parent = nullptr);
    ~AppointmentRequestWidget();

private:
    Ui::AppointmentRequestWidget *ui;
};

#endif // APPOINTMENT_REQUEST_WIDGET_H
