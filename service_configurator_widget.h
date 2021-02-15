#ifndef SERVICE_CONFIGURATOR_WIDGET_H
#define SERVICE_CONFIGURATOR_WIDGET_H

#include <vector>

#include <QMainWindow>

#include <service.h>
#include <answer.h>

#include <question_display_widget.h>

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

    void on_next_btn_clicked();

    void on_prev_btn_clicked();

private:
    std::unique_ptr<appointy::Service> _service;
    std::vector<std::shared_ptr<appointy::Answer>> _answers;
    std::vector<QuestionDisplayWidget *> question_widgets;
    size_t current_question_index;

private:
    Ui::ServiceConfiguratorWidget *ui;
};

#endif // SERVICE_CONFIGURATOR_WIDGET_H
