#ifndef REQUESTWIDGETBASE_H
#define REQUESTWIDGETBASE_H

#include <QWidget>

#include <json.hpp>

class RequestWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit RequestWidgetBase(QWidget *parent = nullptr);

public:
    virtual ~RequestWidgetBase() = default;

public:
    //virtual auto is_empty() const noexcept -> bool = 0;
    virtual auto clear() noexcept -> void = 0;
    /**
     * @brief validate must throw an appointy::Exception with a meaningful message if contents of the widget are not valid
     */
    virtual auto validate() const -> void = 0;
    virtual auto to_json() const -> nlohmann::json = 0;

signals:

};

#endif // REQUESTWIDGETBASE_H
