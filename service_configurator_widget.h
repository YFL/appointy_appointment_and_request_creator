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
    auto validate() const -> void;

    /**
     * @brief returns a json object of the form: {service_id : <object>, answers: <array of answer objects>}
     * @return a json object of the form: {service_id : <object>, answers: <array of answer objects>}
     * @throws appointy::Exception with a meaningful message if the answers' validation fails
     */
    auto to_json() -> nlohmann::json;

public:
    auto service_id() const noexcept -> nlohmann::json;
    auto answers() const -> std::vector<std::shared_ptr<appointy::Answer>>;

public:
    auto service() const noexcept -> appointy::Service;

private slots:
    void on_next_btn_clicked();

    void on_prev_btn_clicked();

    void on_actionReset_triggered();

    void on_actionSave_as_triggered();

private:
    auto create_question_widgets_and_show_first_question_if_any() noexcept -> void;
    auto check_answers() const noexcept -> std::optional<unsigned long>;

private:
    appointy::Service _service;
    std::vector<QuestionDisplayWidget *> question_widgets;
    size_t current_question_index;

private:
    Ui::ServiceConfiguratorWidget *ui;
};

#endif // SERVICE_CONFIGURATOR_WIDGET_H
