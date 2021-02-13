#include "question_display_widget.h"
#include "ui_question_display_widget.h"

#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>

#include <appointy_exception.h>
#include <choice_answer.h>
#include <choice_answer_signature.h>
#include <numeric_answer_signature.h>

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
            auto *rb = dynamic_cast<QRadioButton *>(layout->itemAt(i)->widget());
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

auto QuestionDisplayWidget::answer() const -> std::unique_ptr<appointy::Answer>
{
    auto type = (appointy::AnswerSignatureType)question.answer_signature->type;
    if(type == appointy::AnswerSignatureType::MANY ||
       type == appointy::AnswerSignatureType::SINGLE)
    {
        if(check_choice(dynamic_cast<QHBoxLayout *>(ui->answers->layout()), *question.answer_signature))
        {

        }
    }
    else
    {
        ui->answers->layout()->addWidget(create_numeric_answer_widget(*question.answer_signature));
    }
}
