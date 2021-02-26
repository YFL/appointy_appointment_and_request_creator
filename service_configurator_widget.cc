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

ServiceConfiguratorWidget::ServiceConfiguratorWidget(QWidget *parent) :
    QMainWindow(parent),
    _service {nullptr},
    service_selector {nullptr},
    ui(new Ui::ServiceConfiguratorWidget)
{
    ui->setupUi(this);
}

ServiceConfiguratorWidget::~ServiceConfiguratorWidget()
{
    delete ui;
}

void ServiceConfiguratorWidget::on_actionOpen_triggered()
{
    using nlohmann::json;

    auto fod = QFileDialog {};
    fod.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fod.setDefaultSuffix("json");
    //fod.setDirectory("./");
    fod.setFileMode(QFileDialog::FileMode::ExistingFile);
    fod.setNameFilter("JSON files (*.json)");
    fod.setViewMode(QFileDialog::ViewMode::Detail);
    if(fod.exec())
    {
        // when I used fod.selectedFiles()[0] I got a warning that I shouldnt use operator[]() on a temporary QList object
        auto selected_files = fod.selectedFiles();
        auto selected_file = selected_files[0];
        auto file = std::ifstream {selected_file.toStdString()};
        auto line = std::string {};
        auto ss = std::stringstream {};
        while(std::getline(file, line))
        {
            ss << line;
        }

        try
        {
            auto jsn = json::parse(ss.str());
            if(jsn.is_array())
            {
                std::vector<appointy::Service> services;
                services.reserve(jsn.size());
                for(auto &service : jsn)
                {
                    services.push_back(appointy::JSON_Parser::parse_service(service));
                }
                service_selector = new ServiceSelectorWindow {services, this};
                connect(service_selector, &ServiceSelectorWindow::selected, this, &ServiceConfiguratorWidget::on_service_selected);
                service_selector->show();
                return;
            }

            // if the json contains only one service this runs
            change_service_and_show_first_question_if_any(appointy::JSON_Parser::parse_service({ss.str()}));
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("File contents erronous", e.what());
        }
        catch(const nlohmann::detail::parse_error &e)
        {
            show_error_with_ok("File contents erronous", e.what());
        }
    }
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
    change_service_and_show_first_question_if_any(service);
    delete service_selector;
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

void ServiceConfiguratorWidget::change_service_and_show_first_question_if_any(const appointy::Service &service)
{
    _service = std::unique_ptr<appointy::Service> { new appointy::Service {service}};
    _answers.clear();
    _answers.resize(_service->questions.size());
    for(QWidget *w : question_widgets)
    {
        delete w;
    }
    question_widgets.clear();
    if(_service->questions.size() > 0)
    {
        for(auto &question : _service->questions)
        {
            question_widgets.push_back(new QuestionDisplayWidget {question, ui->question_widget});
            connect(question_widgets.back(), &QuestionDisplayWidget::apply, this, &ServiceConfiguratorWidget::on_answer_apply);
        }
        current_question_index = 0;
        question_widgets[current_question_index]->show();
    }
    ui->prev_btn->setEnabled(false);
    if(_service->questions.size() == 1)
    {
        ui->next_btn->setEnabled(false);
    }
}

void ServiceConfiguratorWidget::on_actionReset_triggered()
{
    change_service_and_show_first_question_if_any(*_service);
}

void ServiceConfiguratorWidget::on_actionSave_as_triggered()
{
    if(_service)
    {
        try
        {
            auto first_date = string_to_date(ui->first_date->text().toStdString());
            try
            {
                auto last_date = string_to_date(ui->last_date->text().toStdString());

                auto qtime = ui->interval_start->time();
                try
                {
                    auto interval_start = appointy::Time {qtime.hour(), qtime.minute(), qtime.second()};
                    qtime = ui->interval_end->time();
                    try
                    {
                        auto interval_end = appointy::Time {qtime.hour(), qtime.minute(), qtime.second()};

                        auto json = "{}"_json;
                        json["first_date"] = first_date.to_json();
                        json["last_date"] = last_date.to_json();
                        json["interval_start"] = interval_start.to_json();
                        json["interval_end"] = interval_end.to_json();
                        json["service_id"] = _service->id;
                        json["answers"] = "[]"_json;

                        for(auto i = 0ul; i < _answers.size(); i++)
                        {
                            if(_answers[i] == nullptr)
                            {
                                show_error_with_ok("One of the questions was not answered", "The text of the questions reads: " + question_widgets[i]->question().text);
                                return;
                            }
                            json["answers"].push_back(_answers[i]->to_json());
                        }

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
                    catch(const appointy::Exception &e)
                    {
                        show_error_with_ok("Illegal interval end", e.what());
                    }
                }
                catch(const appointy::Exception &e)
                {
                    show_error_with_ok("Illegal interval start", e.what());
                }
            }
            catch(const appointy::Exception &e)
            {
                show_error_with_ok("Illegal last date", e.what());
            }
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("Illegal first date", e.what());
        }
    }
    else
    {
        show_error_with_ok("No service loaded", "");
    }
}
