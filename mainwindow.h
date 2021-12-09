#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <math.h>
#include <QtSql>

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
    void on_pushButton_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *ui;
    int day_of_year;
    float day_of_jl_year;
    float var_jl;
    QString plik="";

};
#endif // MAINWINDOW_H
