#ifndef IDCHECKBOX_H
#define IDCHECKBOX_H

#include <QCheckBox>

class IDCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit IDCheckBox(const QString &text, uint32_t id, QWidget *parent = nullptr);

public:
    auto id() const -> uint32_t;

private:
    uint32_t _id;
};

#endif // IDCHECKBOX_H
