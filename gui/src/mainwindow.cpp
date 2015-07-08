#include "gui/include/mainwindow.h"
#include "ui_mainwindow.h"
#include "optical_flow/include/image_processing.h"
#include<QString>
#include<QFileDialog>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_started_ = false;
    ui->smoothing_sigma->setText(QString::number(0.6));
    ui->max_error->setText(QString::number(5.));
    ui->max_scaling_level->setText(QString::number(4));
    ui->smoothing_window_size->setText(QString::number(5));
    ui->window_size->setText(QString::number(15));
    ui->state->setText(QString("Choose imgs and press\nstart button\nto calulate a flow"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectImgPrev_clicked()
{
    QString img_prev_name = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                tr("Img Files (*.png *.jpg)"));
    ui->text_img1->setPlainText(img_prev_name);
}

void MainWindow::on_selectImgCurr_clicked()
{
    QString img_curr_name = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                tr("Img Files (*.png *.jpg)"));
    ui->text_img2->setPlainText(img_curr_name);
}

void MainWindow::on_selectSaveFile_clicked()
{
    QString flow_name = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                               tr("Images (*.png *.jpg)"));
    ui->text_flow->setPlainText(flow_name);
}

void MainWindow::on_Start_clicked()
{
    if(is_started_)
    {
        return;
    }
    is_started_ = true;

    QString img_prev_name =  ui->text_img1->toPlainText();
    QString img_curr_name =  ui->text_img2->toPlainText();
    QString flow_name =  ui->text_flow->toPlainText();
    QString smoothing_sigma_str = ui->smoothing_sigma->toPlainText();
    QString max_error_str = ui->max_error->toPlainText();
    QString max_scaling_level_str = ui->max_scaling_level->toPlainText();
    QString smoothing_window_size_str = ui->smoothing_window_size->toPlainText();
    QString window_size_str = ui->window_size->toPlainText();
    if(img_prev_name == QString())
    {
        ui->state->setText(QString("img 1 name invalid value"));
        is_started_ = false;
        return;
    }
    if(img_curr_name == QString())
    {
        ui->state->setText(QString("img 2 name invalid value"));
        is_started_ = false;
        return;
    }
    if(flow_name == QString())
    {
        ui->state->setText(QString("img 2 name value"));
        is_started_ = false;
        return;
    }
    bool ok;
    double smoothing_sigma = smoothing_sigma_str.toDouble(&ok);
    if(!ok || smoothing_sigma < 0)
    {
        ui->state->setText(QString("smoothing sigma invalid value"));
        is_started_ = false;
        return;
    }
    double max_error = max_error_str.toDouble(&ok);
    if(!ok || max_error < 0)
    {
        ui->state->setText(QString("max error invalid value"));
        is_started_ = false;
        return;
    }
    int max_scaling_level = max_scaling_level_str.toInt(&ok,10);
    if(!ok || max_scaling_level < 0)
    {
        ui->state->setText(QString("max scaling level invalid value"));
        is_started_ = false;
        return;
    }
    int smoothing_window_size = smoothing_window_size_str.toInt(&ok, 10);
    if(!ok || smoothing_window_size < 0)
    {
        ui->state->setText(QString("smoothing windows size invalid value"));
        is_started_ = false;
        return;
    }
    int window_size = window_size_str.toInt(&ok, 10);
    if(!ok || window_size < 0)
    {
        ui->state->setText(QString("window size invalid value"));
        is_started_ = false;
        return;
    }

    FlowCalculationThread *calculationThread = new FlowCalculationThread(img_prev_name,
                                                                         img_curr_name,
                                                                         flow_name,
                                                                         smoothing_sigma,
                                                                         smoothing_window_size,
                                                                         window_size,
                                                                         max_error,
                                                                         max_scaling_level);
    connect(calculationThread, SIGNAL(finishedFlowCalculation(QString)),
            SLOT(onFinishedFlowCalculation(QString)));
    connect(calculationThread, SIGNAL(finished()),
            calculationThread, SLOT(deleteLater()));
    calculationThread->start();

    ui->state->setText(QString("flow is calculating"));
    return;

}

void MainWindow::onFinishedFlowCalculation(QString info)
{
    ui->state->setText(info);
    is_started_ = false;
}

void FlowCalculationThread::run()
{
    using namespace image_processing;
    Matrix<double> img_curr = IntensityMatrix(img_curr_name_.toStdString());
    Matrix<double> img_prev = IntensityMatrix(img_prev_name_.toStdString());
    Matrix<Matrix<double> > flows = PyramidalLucasKanadeRefined(window_size_, max_error_,
                                                                smoothing_window_size_,
                                                                smoothing_sigma_,
                                                                max_scaling_level_,
                                                                img_prev,
                                                                img_curr);
    QImage flow_img = getQImageFromFlows(flows);
    flow_img.save(flow_name_);
    emit finishedFlowCalculation(QString("flow calculation finished"));
    return;
}

