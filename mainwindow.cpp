#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap now_img (":/Phase_pick/phases/phase_new.jpg");
    QPixmap pelnia_img (":/Phase_pick/phases/phase_full.jpg");
    QPixmap pierwsza_kwarta_img (":/Phase_pick/phases/phase_first_quarter.jpg");
    QPixmap ostatnia_kwarta_img (":/Phase_pick/phases/phase_third_quarter.jpg");

    if(0<=var_jl and var_jl<0.25)
    {
       ui->label->setText("Nów");
       ui->label_img->setPixmap(now_img);
    }
    if(0.25<=var_jl and var_jl<0.50)
    {
        ui->label->setText("Pierwsza kwarta");
        ui->label_img->setPixmap(pierwsza_kwarta_img);
    }
    if(0.50<=var_jl and var_jl<0.75)
    {
        ui->label->setText("Pełnia");
        ui->label_img->setPixmap(pelnia_img);

    }
    if(0.75<=var_jl)
    {
        ui->label->setText("Ostatnia kwarta");
        ui->label_img->setPixmap(ostatnia_kwarta_img);
    }
}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    day_of_jl_year=date.toJulianDay();
    day_of_year=date.dayOfYear();
    float faza=((day_of_jl_year/29.5305902778)-0.3033);
    float x= floor(faza);
    var_jl=faza-x;

}


