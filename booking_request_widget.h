#ifndef BOOKING_REQUEST_WIDGET_H
#define BOOKING_REQUEST_WIDGET_H

#include "request_widget_base.h"

namespace Ui {
class BookingRequestWidget;
}

class BookingRequestWidget : public RequestWidgetBase
{
    Q_OBJECT

public:
    explicit BookingRequestWidget(QWidget *parent = nullptr);
    ~BookingRequestWidget() override;

public:
    //auto is_empty() const noexcept -> bool override;
    auto clear() noexcept -> void override;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    auto validate() const -> void override;
    auto to_json() const -> nlohmann::json override;

private:
    Ui::BookingRequestWidget *ui;
};

#endif // BOOKING_REQUEST_WIDGET_H
