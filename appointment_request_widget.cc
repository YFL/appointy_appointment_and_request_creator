#include "appointment_request_widget.h"
#include "ui_appointment_request_widget.h"

AppointmentRequestWidget::AppointmentRequestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppointmentRequestWidget)
{
    ui->setupUi(this);
}

AppointmentRequestWidget::~AppointmentRequestWidget()
{
    delete ui;
}
