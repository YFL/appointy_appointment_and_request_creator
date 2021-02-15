#include "service_configurator_widget.h"
#include "ui_service_configurator_widget.h"

#include <fstream>
#include <string>
#include <sstream>

#include <QFileDialog>

#include <json_parser.h>

#include "../service_creator/util.h"

ServiceConfiguratorWidget::ServiceConfiguratorWidget(QWidget *parent) :
    QMainWindow(parent),
    _service {nullptr},
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
    auto fod = QFileDialog {};
    fod.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fod.setDefaultSuffix("json");
    fod.setDirectory("./");
    fod.setFileMode(QFileDialog::FileMode::ExistingFile);
    fod.setNameFilter("JSON files (*.json)");
    fod.setViewMode(QFileDialog::ViewMode::Detail);
    if(fod.exec())
    {
        auto selected_file = fod.selectedFiles()[0];
        auto file = std::ifstream {selected_file.toStdString()};
        auto line = std::string {};
        auto ss = std::stringstream {};
        while(std::getline(file, line))
        {
            ss << line;
        }

        try
        {
            _service = std::unique_ptr<appointy::Service> {new appointy::Service {appointy::JSON_Parser::parse_service({ss.str()})}};
            _answers.clear();
            question_widgets.clear();
            for(auto &question : _service->questions)
            {
                question_widgets.push_back(new QuestionDisplayWidget {question, this});
            }
            current_question_index = 0;
            ui->question_widget = question_widgets[current_question_index];
        }
        catch(const appointy::Exception &e)
        {
            show_error_with_ok("File contents erronous", e.what());
        }
    }
}

void ServiceConfiguratorWidget::on_next_btn_clicked()
{
    auto answer = dynamic_cast<QuestionDisplayWidget *>(ui->question_widget)->answer();
    if(answer)
    {
        _answers[current_question_index++] = answer;
        if(current_question_index == question_widgets.size())
        {
            ui->next_btn->setEnabled(false);
        }
        ui->question_widget = question_widgets[current_question_index];
    }
    else
    {
        show_error_with_ok("You didn't answer the question", "");
    }
}

void ServiceConfiguratorWidget::on_prev_btn_clicked()
{
    ui->question_widget = question
}
