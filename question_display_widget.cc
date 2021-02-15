#include "question_display_widget.h"
#include "ui_question_display_widget.h"

#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSpinBox>

#include <appointy_exception.h>
#include <choice_answer.h>
#include <choice_answer_signature.h>
#include <numeric_answer.h>
#include <numeric_answer_signature.h>

#include <id_radio_button.h>

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

auto create_choice_widget(const appointy::Option &option)
{
    auto rb = new QRadioButton {};
    rb->setText({option.text.c_str()});

    return rb;
}

auto check_choice(const QHBoxLayout * const layout, const appointy::AnswerSignature &answer_signature)
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
    return answer_signature.type == appointy::AnswerSignatureType::SINGLE ? true : false;
}

auto get_choices(const QHBoxLayout * const layout)
{
    std::vector<uint32_t> ids;
    for(int i = 0; i < layout->count(); i++)
    {
        auto &btn = dynamic_cast<IDRadioButton &>(*layout->itemAt(i));
        if(btn.isChecked())
        {
            ids.push_back(btn.id());
        }
    }

    return ids;
}

QuestionDisplayWidget::QuestionDisplayWidget(const appointy::Question &question, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionDisplayWidget),
    question {question}
{
    ui->setupUi(this);
}

QuestionDisplayWidget::~QuestionDisplayWidget()
{
    delete ui;
}

auto QuestionDisplayWidget::answer() const -> std::shared_ptr<appointy::Answer>
{
    auto type = (appointy::AnswerSignatureType)question.answer_signature->type;
    if(type == appointy::AnswerSignatureType::MANY ||
       type == appointy::AnswerSignatureType::SINGLE)
    {
        if(check_choice(dynamic_cast<QHBoxLayout *>(ui->answers->layout()), *question.answer_signature))
        {
            return std::shared_ptr<appointy::Answer> {new appointy::ChoiceAnswer {0, question.answer_signature->id, get_choices(dynamic_cast<QHBoxLayout *>(ui->answers->layout()))}};
        }

        return nullptr;
    }
    else if(type == appointy::AnswerSignatureType::INT)
    {
        return std::shared_ptr<appointy::Answer> {new appointy::NumericAnswer<int> {0, question.answer_signature->id, dynamic_cast<QSpinBox &>(*ui->answers->layout()->itemAt(0)).value()}};
    }
    else if(type == appointy::AnswerSignatureType::DOUBLE)
    {
        return std::shared_ptr<appointy::Answer> {new appointy::NumericAnswer<double> {0, question.answer_signature->id, dynamic_cast<QDoubleSpinBox &>(*ui->answers->layout()->itemAt(0)).value()}};
    }

    return nullptr;
}
