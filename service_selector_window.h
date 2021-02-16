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

private:
    Ui::ServiceSelectorWindow *ui;
};

#endif // SERVICE_SELECTOR_WINDOW_H
