#include "id_radio_button.h"

IDRadioButton::IDRadioButton(const QString &text, uint32_t id, QWidget *parent) :
    QRadioButton {text, parent},
    _id {id}
{

}

auto IDRadioButton::id() const -> uint32_t
{
    return _id;
}
