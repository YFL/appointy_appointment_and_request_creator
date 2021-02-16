#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_actionNew_Appointment_triggered();

    void on_save_btn_clicked();

    void on_actionNew_AppointmentRequest_triggered();

private:
    ServiceConfiguratorWidget *service_configurator;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
