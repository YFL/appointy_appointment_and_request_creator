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
    create_question_widgets_and_show_first_question_if_any();
}

ServiceConfiguratorWidget::~ServiceConfiguratorWidget()
{
    delete ui;
}

auto ServiceConfiguratorWidget::validate() const -> void
{
    auto index = check_answers();
    if(index)
    {
        throw appointy::Exception {"One of the questions was not answered.\nThe text of the question reads: " + question_widgets[index.value()]->question().text};
    }
}

auto ServiceConfiguratorWidget::to_json() -> nlohmann::json
{
    validate();
    auto json = "{}"_json;
    json["service_id"] = _service.id;
    json["answers"] = "[]"_json;

    for(auto i = 0ul; i < question_widgets.size(); i++)
    {
        json["answers"].push_back(question_widgets[i]->answer()->to_json());
    }

    return json;
}

auto ServiceConfiguratorWidget::service_id() const noexcept -> nlohmann::json
{
    return _service.id;
}

auto ServiceConfiguratorWidget::configuration() const -> std::vector<std::shared_ptr<appointy::Answer>>
{
    validate();
    auto answers = std::vector<std::shared_ptr<appointy::Answer>> {};
    for(auto *qw : question_widgets)
    {
        answers.push_back(qw->answer());
    }

    return answers;
}

auto ServiceConfiguratorWidget::service() const noexcept -> appointy::Service
{
    return _service;
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

auto ServiceConfiguratorWidget::create_question_widgets_and_show_first_question_if_any() noexcept -> void
{
    if(_service.questions.size() > 0)
    {
        for(auto &question : _service.questions)
        {
            question_widgets.push_back(new QuestionDisplayWidget {question, ui->question_widget});
            question_widgets.back()->hide();
        }
        current_question_index = 0;
        question_widgets[current_question_index]->show();
    }
    ui->prev_btn->setEnabled(false);
    if(_service.questions.size() <= 1)
    {
        ui->next_btn->setEnabled(false);
    }
}

auto ServiceConfiguratorWidget::check_answers() const noexcept -> std::optional<unsigned long>
{
    for(uint64_t i = 0; i < question_widgets.size(); i++)
    {
        try
        {
            question_widgets[i]->validate();
        }
        catch(const appointy::Exception &)
        {
            return i;
        }
    }

    return std::nullopt;
}

void ServiceConfiguratorWidget::on_actionReset_triggered()
{
    for(auto *question : question_widgets)
    {
        delete question;
    }
    question_widgets.clear();
    create_question_widgets_and_show_first_question_if_any();
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
