#ifndef IDRADIOBUTTON_H
#define IDRADIOBUTTON_H

#include <QRadioButton>

class IDRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    IDRadioButton(const QString &text, uint32_t id, QWidget *parent = nullptr);

public:
    auto id() const -> uint32_t;

private:
    uint32_t _id;
};

#endif // IDRADIOBUTTON_H
