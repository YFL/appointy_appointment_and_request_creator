#include "id_check_box.h"

IDCheckBox::IDCheckBox(const QString &text, uint32_t id, QWidget *parent) :
    QCheckBox {text, parent},
    _id {id}
{

}

auto IDCheckBox::id() const -> uint32_t
{
    return _id;
}
