#ifndef REQUESTWIDGETBASE_H
#define REQUESTWIDGETBASE_H

#include <QWidget>

class RequestWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit RequestWidgetBase(QWidget *parent = nullptr);

public:
    virtual ~RequestWidgetBase() = default;

public:
    virtual auto is_empty() noexcept -> bool = 0;
    virtual auto clear() noexcept -> void = 0;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    virtual auto validate() -> void = 0;
    virtual auto to_json() -> std::string = 0;

signals:

};

#endif // REQUESTWIDGETBASE_H
