#include "question_display_widget.h"
#include "ui_question_display_widget.h"

#include <iostream>

#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSpinBox>

#include <appointy_exception.h>
#include <choice_answer.h>
#include <choice_answer_signature.h>
#include <numeric_answer.h>
#include <numeric_answer_signature.h>

#include <id_radio_button.h>
#include <id_check_box.h>

auto create_numeric_answer_widget(const appointy::AnswerSignature &answer_signature)
{
    if(answer_signature.type == appointy::AnswerSignatureType::DOUBLE)
    {
        auto spin_box = new QDoubleSpinBox {};
        auto &double_as = dynamic_cast<const appointy::NumericAnswerSignature<double> &>(answer_signature);
        if(double_as.min)
        {
            spin_box->setMinimum(double_as.min.value());
        }
        if(double_as.max)
        {
            spin_box->setMaximum(double_as.max.value());
        }
        if(double_as.default_value)
        {
            spin_box->setValue(double_as.default_value.value());
        }

        return (QWidget *)spin_box;
    }
    if(answer_signature.type == appointy::AnswerSignatureType::INT)
    {
        auto spin_box = new QSpinBox {};
        auto &double_as = dynamic_cast<const appointy::NumericAnswerSignature<int> &>(answer_signature);
        if(double_as.min)
        {
            spin_box->setMinimum(double_as.min.value());
        }
        if(double_as.max)
        {
            spin_box->setMaximum(double_as.max.value());
        }
        if(double_as.default_value)
        {
            spin_box->setValue(double_as.default_value.value());
        }

        return (QWidget *)spin_box;
    }
    throw appointy::Exception
    {
        "Invalid answer signature type. Allowed are " +
        (std::string)(appointy::AnswerSignatureType)appointy::AnswerSignatureType::INT + " and " +
        (std::string)(appointy::AnswerSignatureType)appointy::AnswerSignatureType::DOUBLE + ". Given was " +
        (std::string)answer_signature.type
    };
}

auto create_choice_widget(const appointy::Option &option, appointy::AnswerSignatureType type)
{
    if(type == appointy::AnswerSignatureType::SINGLE)
    {
        auto *rb = new IDRadioButton {{option.text.c_str()}, option.id};

        return (QWidget *)rb;
    }
    else if(type == appointy::AnswerSignatureType::MANY)
    {
        auto *cb = new IDCheckBox {{option.text.c_str()}, option.id};

        return (QWidget *)cb;
    }
    return (QWidget *)nullptr;
}

auto check_choice(const QLayout * const layout, const appointy::AnswerSignature &answer_signature)
{
    for(int i = 0; i < layout->count(); i++)
    {
        if(answer_signature.type == appointy::AnswerSignatureType::SINGLE)
        {
            auto *rb = dynamic_cast<IDRadioButton *>(layout->itemAt(i)->widget());
            if(rb->isChecked())
            {
                return true;
            }
        }
        else if(answer_signature.type == appointy::AnswerSignatureType::MANY)
        {
            auto *cb = dynamic_cast<QCheckBox *>(layout->itemAt(i)->widget());
            if(cb->isChecked())
            {
                return true;
            }
        }
    }
    return answer_signature.type == appointy::AnswerSignatureType::SINGLE ? false : true;
}

auto get_choices(const QLayout * const layout)
{
    std::vector<uint32_t> ids;
    for(int i = 0; i < layout->count(); i++)
    {
        if(IDRadioButton *rb = dynamic_cast<IDRadioButton *>(layout->itemAt(i)->widget()))
        {
            if(rb->isChecked())
            {
                ids.push_back(rb->id());
            }
        }
        else if(IDCheckBox *cb = dynamic_cast<IDCheckBox *>(layout->itemAt(i)->widget()))
        {
            if(cb->isChecked())
            {
                ids.push_back(cb->id());
            }
        }
    }

    return ids;
}

QuestionDisplayWidget::QuestionDisplayWidget(const appointy::Question &question, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionDisplayWidget),
    _question {question}
{
    ui->setupUi(this);
    ui->text->setText(question.text.c_str());
    auto type = question.answer_signature->type;
    if(type == appointy::AnswerSignatureType::MANY || type == appointy::AnswerSignatureType::SINGLE)
    {
        auto cas = dynamic_cast<const appointy::ChoiceAnswerSignature &>(*question.answer_signature);
        for(auto &option : cas.options)
        {
            ui->answers->layout()->addWidget(create_choice_widget(option, question.answer_signature->type));
        }
    }
    else
    {
        ui->answers->layout()->addWidget(create_numeric_answer_widget(*question.answer_signature));
    }
}

QuestionDisplayWidget::~QuestionDisplayWidget()
{
    delete ui;
}

 auto QuestionDisplayWidget::validate() const-> void
{
    if(!check_choice(ui->answers->layout(), *_question.answer_signature))
    {
        throw appointy::Exception {"Question not answered. The text of the question reads: " + _question.text};
    }
}

auto QuestionDisplayWidget::to_json() const -> nlohmann::json
{
    validate();
    return answer()->to_json();
}

appointy::Question QuestionDisplayWidget::question() const
{
    return _question;
}

auto QuestionDisplayWidget::answer() const -> std::shared_ptr<appointy::Answer>
{
    auto type = (appointy::AnswerSignatureType)_question.answer_signature->type;
    if(type == appointy::AnswerSignatureType::MANY ||
       type == appointy::AnswerSignatureType::SINGLE)
    {
        if(check_choice(ui->answers->layout(), *_question.answer_signature))
        {
            return std::shared_ptr<appointy::Answer> {new appointy::ChoiceAnswer {_question.answer_signature->id, get_choices(ui->answers->layout())}};
        }

        return nullptr;
    }
    else if(type == appointy::AnswerSignatureType::INT)
    {
        return std::shared_ptr<appointy::Answer> {new appointy::NumericAnswer<int> {_question.answer_signature->id, dynamic_cast<QSpinBox &>(*ui->answers->layout()->itemAt(0)->widget()).value()}};
    }
    else if(type == appointy::AnswerSignatureType::DOUBLE)
    {
        return std::shared_ptr<appointy::Answer> {new appointy::NumericAnswer<double> {_question.answer_signature->id, dynamic_cast<QDoubleSpinBox &>(*ui->answers->layout()->itemAt(0)->widget()).value()}};
    }

    return nullptr;
}
