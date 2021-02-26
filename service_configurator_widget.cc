#include "service_configurator_widget.h"
#include "ui_service_configurator_widget.h"

#include <fstream>
#include <sstream>
#include <string>

#include <QFileDialog>

#include <json_parser.h>
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
        // when I used fod.selectedFiles()[0] I got a warning that I shuoldnt use operator[]() on a temporary QList object
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
    auto answer = dynamic_cast<QuestionDisplayWidget &>(*ui->question_widget).answer();
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
