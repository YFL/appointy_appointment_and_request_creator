#ifndef QUESTION_DISPLAY_WIDGET_H
#define QUESTION_DISPLAY_WIDGET_H

#include <QWidget>

#include <answer.h>
#include <question.h>

namespace Ui {
class QuestionDisplayWidget;
}

class QuestionDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionDisplayWidget(const appointy::Question &question, QWidget *parent = nullptr);
    ~QuestionDisplayWidget();

public:
    auto answer() const -> std::shared_ptr<appointy::Answer>;

private:
    Ui::QuestionDisplayWidget *ui;
    const appointy::Question question;
};

#endif // QUESTION_DISPLAY_WIDGET_H
