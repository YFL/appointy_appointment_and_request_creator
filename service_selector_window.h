#ifndef SERVICE_SELECTOR_WINDOW_H
#define SERVICE_SELECTOR_WINDOW_H

#include <QMainWindow>

#include <service.h>

namespace Ui {
class ServiceSelectorWindow;
}

class ServiceSelectorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceSelectorWindow(const std::vector<appointy::Service> &services, QWidget *parent = nullptr);
    ~ServiceSelectorWindow();

public:
    auto validate() const -> void;

public:
    auto service() const -> appointy::Service;

private slots:
    void on_select_btn_clicked();

private:
    std::vector<appointy::Service> _services;
    appointy::Service *_service;

private:
    Ui::ServiceSelectorWindow *ui;
};

#endif // SERVICE_SELECTOR_WINDOW_H
