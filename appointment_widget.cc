#include "appointment_widget.h"
#include "ui_appointment_widget.h"

AppointmentWidget::AppointmentWidget(QWidget *parent) :
    RequestWidgetBase(parent),
    ui(new Ui::AppointmentWidget)
{
    ui->setupUi(this);
}

AppointmentWidget::~AppointmentWidget()
{
    delete ui;
}
