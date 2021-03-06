#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <request_widget_base.h>
#include <service_configurator_widget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_BookingRequest_triggered();

    void on_save_btn_clicked();

    void on_actionNew_AppointmentConfiguration_triggered();

    void on_preview_btn_clicked();

    void on_actionNew_Appointment_triggered();

private:
    auto change_and_show_request_widget(QWidget *widget) noexcept -> void;
    auto request_widget() const noexcept -> const RequestWidgetBase *;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
