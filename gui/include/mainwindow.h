#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Start_clicked();

    void on_selectImgPrev_clicked();

    void on_selectImgCurr_clicked();

    void on_selectSaveFile_clicked();

    void onFinishedFlowCalculation(QString info);

private:
    Ui::MainWindow *ui;
    bool is_started_;

};

class FlowCalculationThread : public QThread
{
    Q_OBJECT

    QString img_prev_name_;
    QString img_curr_name_;
    QString flow_name_;
    double smoothing_sigma_;
    double smoothing_window_size_;
    int window_size_;
    double max_error_;
    int max_scaling_level_;

    void run();
    // Define signal:
    signals:
    void finishedFlowCalculation(QString info);

public:
    FlowCalculationThread(const QString& img_prev_name,
                          const QString& img_curr_name,
                          const QString& flow_name,
                          double smoothing_sigma,
                          double smoothing_window_size,
                          int window_size,
                          double max_error,
                          int max_scaling_level):
        img_prev_name_(img_prev_name),
        img_curr_name_(img_curr_name),
        flow_name_(flow_name),
        smoothing_sigma_(smoothing_sigma),
        smoothing_window_size_(smoothing_window_size),
        window_size_(window_size),
        max_error_(max_error),
        max_scaling_level_(max_scaling_level)
    {}
};

#endif // MAINWINDOW_H
