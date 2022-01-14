#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Interaktywny Kalendarz");
    qDebug()<<QSqlDatabase::drivers();
    BazaDanychStart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap now_img (":/new/prefix1/phases/moon (2).jpg");
    QPixmap pelnia_img (":/new/prefix1/phases/moon (4).jpg");
    QPixmap pierwsza_kwarta_img (":/new/prefix1/phases/moon (1).jpg");
    QPixmap ostatnia_kwarta_img (":/new/prefix1/phases/moon (3).jpg");

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

    plik.clear();
    ui->plainTextEdit->clear();

    QSqlQuery query;
    query.prepare("SELECT txt FROM notes WHERE day=?");
    query.addBindValue(day_of_year);
    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();
    if(query.first())
        ui->plainTextEdit->setPlainText(query.value(0).toString());
    else
        ui->plainTextEdit->setPlaceholderText("brak notatki");


}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    day_of_jl_year=date.toJulianDay();
    day_of_year=date.dayOfYear();
    float faza=((day_of_jl_year/29.5305902778)-0.3033);
    float x= floor(faza);
    var_jl=faza-x;

}



void MainWindow::on_save_button_clicked()
{
    QString tekst=ui->plainTextEdit->toPlainText();
    BazaDanychWypelni(tekst);
}

void MainWindow::BazaDanychStart()
{
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QSqlDatabase baza=QSqlDatabase::addDatabase("QSQLITE");
        baza.setDatabaseName("database.db");

        if(!baza.open())
              qWarning() << "MainWindow::BazaDanychStart - Blad: " << baza.lastError();
    }
    else
    {
        qWarning() << "MainWindow::DatabaseStart - Blad: brsk syerownika " << DRIVER << " dostępny";
    }

}


void MainWindow::BazaDanychWypelni(QString dane)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO notes(day,txt) VALUES(?,?)");
    query.addBindValue(day_of_year);
    query.addBindValue(dane);
        if (!query.exec())
            qWarning() << "MainWindow::BazaDanychWypelni - Blad: " << query.lastError().text();
}
