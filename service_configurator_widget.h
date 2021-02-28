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
    explicit ServiceConfiguratorWidget(const appointy::Service &service, QWidget *parent = nullptr);
    ~ServiceConfiguratorWidget();

public:
    auto validate() -> void;

    auto to_json() -> nlohmann::json;

private slots:
    void on_next_btn_clicked();

    void on_prev_btn_clicked();

    void on_service_selected(const appointy::Service &service);

    void on_answer_apply();

    void on_actionReset_triggered();

    void on_actionSave_as_triggered();

    void on_return_and_close_btn_clicked();

private:
    auto create_question_widgets_and_show_first_question_if_any(const appointy::Service &service) noexcept -> void;
    auto check_answers() noexcept -> std::optional<unsigned long>;

private:
    appointy::Service _service;
    std::vector<std::shared_ptr<appointy::Answer>> _answers;
    std::vector<QuestionDisplayWidget *> question_widgets;
    size_t current_question_index;

private:
    Ui::ServiceConfiguratorWidget *ui;
};

#endif // SERVICE_CONFIGURATOR_WIDGET_H
