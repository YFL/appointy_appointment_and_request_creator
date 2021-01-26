#ifndef SERVICE_CONFIGURATOR_WIDGET_H
#define SERVICE_CONFIGURATOR_WIDGET_H

#include <vector>

#include <QMainWindow>

#include <service.h>
#include <answer.h>

namespace Ui {
class ServiceConfiguratorWidget;
}

class ServiceConfiguratorWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceConfiguratorWidget(QWidget *parent = nullptr);
    ~ServiceConfiguratorWidget();

private slots:
    void on_actionOpen_triggered();

private:
    std::unique_ptr<appointy::Service> _service;
    std::vector<std::shared_ptr<appointy::Answer>> _answers;

private:
    Ui::ServiceConfiguratorWidget *ui;
};

#endif // SERVICE_CONFIGURATOR_WIDGET_H
