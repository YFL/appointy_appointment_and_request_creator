#include "question_display_widget.h"
#include "ui_question_display_widget.h"

QuestionDisplayWidget::QuestionDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionDisplayWidget)
{
    ui->setupUi(this);
}

QuestionDisplayWidget::~QuestionDisplayWidget()
{
    delete ui;
}
