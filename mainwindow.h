#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <math.h>
#include <QtDebug>
#include <QPixmap>

#include <QtSql>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void BazaDanychStart(); //klasa otwierająca bazę danych
    void BazaDanychWypelni(QString dane); //klasa do dodawania danych do bazy
    void phase(); // klasa do obliczania fazy księżyca

private slots:
    void on_checkButton_clicked(); //klasa przycisku sprawdź
    void on_dateEdit_userDateChanged(const QDate &date); //klasa okna z datą
    void on_save_button_clicked();//zapisywanie tekstu do bazy danych

    void PobieranieObrazuEND(QNetworkReply*);
    void PobieranieTekstuEND(QNetworkReply*);
    void siec(QString URL);
    void siecT(QString URL);

private:
    Ui::MainWindow *ui;
    int day_of_year; //zmienna do przechowywania numeru dnia w roku
    float day_of_jl_year; //zmienna do przechowywania numeru dnia w roku juliańskiego
    float var_jl;
    QString NewURL ="https://svs.gsfc.nasa.gov/vis/a000000/a004800/a004874/phase_new.1026_print.jpg";
    QString ThirdquarterURL ="https://svs.gsfc.nasa.gov/vis/a000000/a004800/a004874/phase_third_quarter.2243_print.jpg";
    QString FullURL ="https://svs.gsfc.nasa.gov/vis/a000000/a004800/a004874/phase_full.3492_print.jpg";
    QString FirstquarterURL ="https://svs.gsfc.nasa.gov/vis/a000000/a004800/a004874/phase_first_quarter.5440_print.jpg";
    QString TestURL;
};
#endif // MAINWINDOW_H
