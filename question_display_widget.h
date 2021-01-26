#ifndef QUESTION_DISPLAY_WIDGET_H
#define QUESTION_DISPLAY_WIDGET_H

#include <QWidget>

namespace Ui {
class QuestionDisplayWidget;
}

class QuestionDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionDisplayWidget(QWidget *parent = nullptr);
    ~QuestionDisplayWidget();

private:
    Ui::QuestionDisplayWidget *ui;
};

#endif // QUESTION_DISPLAY_WIDGET_H
