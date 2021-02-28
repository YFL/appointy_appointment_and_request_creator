#include "service_configurator_widget.h"
#include "ui_service_configurator_widget.h"

#include <fstream>
#include <sstream>
#include <string>

#include <QFileDialog>

#include <json_parser.h>
#include <choice_answer.h>

#include <nlohmann/json.hpp>

#include "../service_creator/util.h"

ServiceConfiguratorWidget::ServiceConfiguratorWidget(const appointy::Service &service, QWidget *parent) :
    QMainWindow(parent),
    _service {service},
    ui(new Ui::ServiceConfiguratorWidget)
{
    ui->setupUi(this);
}

ServiceConfiguratorWidget::~ServiceConfiguratorWidget()
{
    delete ui;
}

auto ServiceConfiguratorWidget::validate() -> void
{
    auto index = check_answers();
    if(index)
    {
        throw appointy::Exception {"One of the questions was not answered.\nThe text of the question reads: " + question_widgets[index.value()]->question().text};
    }
}

auto ServiceConfiguratorWidget::to_json() -> nlohmann::json
{
    auto json = "{}"_json;
    json["service_id"] = _service.id;
    json["answers"] = "[]"_json;

    for(auto i = 0ul; i < _answers.size(); i++)
    {
        if(_answers[i] == nullptr)
        {
            throw appointy::Exception {"One of the questions was not answered. The text of the questions reads: " + question_widgets[i]->question().text};
        }
        json["answers"].push_back(_answers[i]->to_json());
    }

    return json;
}

void ServiceConfiguratorWidget::on_next_btn_clicked()
{
    current_question_index++;
    if(current_question_index + 1 == question_widgets.size())
    {
        ui->next_btn->setEnabled(false);
    }
    if(!ui->prev_btn->isEnabled())
    {
        ui->prev_btn->setEnabled(true);
    }
    question_widgets[current_question_index - 1]->hide();
    question_widgets[current_question_index]->show();
}

void ServiceConfiguratorWidget::on_prev_btn_clicked()
{
    current_question_index--;
    if(current_question_index < 1)
    {
        ui->prev_btn->setEnabled(false);
    }
    else if(!ui->next_btn->isEnabled())
    {
        ui->next_btn->setEnabled(true);
    }
    question_widgets[current_question_index + 1]->hide();
    question_widgets[current_question_index]->show();
}

void ServiceConfiguratorWidget::on_service_selected(const appointy::Service &service)
{
    create_question_widgets_and_show_first_question_if_any(service);
}

void ServiceConfiguratorWidget::on_answer_apply()
{
    auto answer = dynamic_cast<QuestionDisplayWidget &>(*question_widgets[current_question_index]).answer();
    if(!answer)
    {
        show_error_with_ok("The answer is not filled out correctly", "");
    }
    else
    {
        _answers[current_question_index] = answer;
    }
}

auto ServiceConfiguratorWidget::create_question_widgets_and_show_first_question_if_any(const appointy::Service &service) noexcept -> void
{
    if(_service.questions.size() > 0)
    {
        _answers.resize(_service.questions.size());
        for(auto &question : _service.questions)
        {
            question_widgets.push_back(new QuestionDisplayWidget {question, ui->question_widget});
            connect(question_widgets.back(), &QuestionDisplayWidget::apply, this, &ServiceConfiguratorWidget::on_answer_apply);
        }
        current_question_index = 0;
        question_widgets[current_question_index]->show();
    }
    ui->prev_btn->setEnabled(false);
    if(_service.questions.size() == 1)
    {
        ui->next_btn->setEnabled(false);
    }
}

auto ServiceConfiguratorWidget::check_answers() noexcept -> std::optional<unsigned long>
{
    for(uint64_t i = 0; i < _answers.size(); i++)
    {
        if(_answers[i] == nullptr)
        {
            return i;
        }
    }

    return std::nullopt;
}

void ServiceConfiguratorWidget::on_actionReset_triggered()
{
    create_question_widgets_and_show_first_question_if_any(_service);
}

void ServiceConfiguratorWidget::on_actionSave_as_triggered()
{
    auto json = to_json();

    auto fsd = QFileDialog {};
    fsd.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    fsd.setDefaultSuffix("json");
    //fsd.setDirectory("./");
    fsd.setFileMode(QFileDialog::FileMode::AnyFile);
    fsd.setNameFilter("JSON files (*.json)");
    fsd.setViewMode(QFileDialog::ViewMode::Detail);
    if(fsd.exec())
    {
        auto selected_files = fsd.selectedFiles();
        auto selected_file = selected_files[0];
        auto file = std::ofstream {selected_file.toStdString()};
        if(file.is_open())
        {
            file << json;
        }
        else
        {
            show_error_with_ok("File couldn't be opened", "Dunno what caused it");
        }
    }
}
