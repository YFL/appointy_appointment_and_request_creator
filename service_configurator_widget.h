#ifndef SERVICE_CONFIGURATOR_WIDGET_H
#define SERVICE_CONFIGURATOR_WIDGET_H

#include <vector>

#include <QMainWindow>

#include <service.h>
#include <answer.h>

#include <question_display_widget.h>
#include <service_selector_window.h>

namespace Ui {
class ServiceConfiguratorWidget;
}

class ServiceConfiguratorWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceConfiguratorWidget(QWidget *parent = nullptr);
    ~ServiceConfiguratorWidget();

signals:
    void service_config_ready(std::vector<std::shared_ptr<appointy::Answer>>);

private slots:
    void on_next_btn_clicked();

    void on_prev_btn_clicked();

    void on_service_selected(const appointy::Service &service);

    void on_answer_apply();

    void on_actionReset_triggered();

    void on_actionSave_as_triggered();

    void on_return_and_close_btn_clicked();

private:
    auto change_service_and_show_first_question_if_any(const appointy::Service &service) -> void;
    auto check_answers() noexcept -> std::optional<unsigned long>;

private:
    std::unique_ptr<appointy::Service> _service;
    std::vector<std::shared_ptr<appointy::Answer>> _answers;
    std::vector<QuestionDisplayWidget *> question_widgets;
    size_t current_question_index;
    ServiceSelectorWindow *service_selector;

private:
    Ui::ServiceConfiguratorWidget *ui;
};

#endif // SERVICE_CONFIGURATOR_WIDGET_H
