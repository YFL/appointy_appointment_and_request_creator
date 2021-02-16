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

auto AppointmentWidget::is_empty() noexcept -> bool
{
    return ui->date->text().isEmpty() && ui->end->text().isEmpty() &&
            ui->start->text().isEmpty();// && ui->
}

auto AppointmentWidget::clear() noexcept -> void
{

}

auto AppointmentWidget::validate() -> void
{

}

auto AppointmentWidget::to_json() -> std::string
{
    return {};
}
