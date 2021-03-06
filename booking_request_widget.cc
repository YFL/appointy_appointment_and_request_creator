#include "booking_request_widget.h"
#include "ui_booking_request_widget.h"

BookingRequestWidget::BookingRequestWidget(QWidget *parent) :
    RequestWidgetBase(parent),
    ui(new Ui::BookingRequestWidget)
{
    ui->setupUi(this);
}

BookingRequestWidget::~BookingRequestWidget()
{
    delete ui;
}

//auto AppointmentWidget::is_empty() const noexcept -> bool
//{
//    return ui->date->text().isEmpty() && ui->end->text().isEmpty() &&
//            ui->start->text().isEmpty();// && ui->
//}

auto BookingRequestWidget::clear() noexcept -> void
{

}

auto BookingRequestWidget::validate() const -> void
{

}

auto BookingRequestWidget::to_json() const -> nlohmann::json
{
    return {};
}
