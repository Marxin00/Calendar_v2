#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Interaktywny Kalendarz");
    BazaDanychStart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-----------------------------------------------[przyciski i okno daty]---------------------------------------------------

void MainWindow::on_checkButton_clicked()
{
    phase();
    ui->plainTextEdit->clear();

    QSqlQuery query;
    query.prepare("SELECT txt FROM notes WHERE day=?");
    query.addBindValue(day_of_year);
    if(!query.exec())
        qWarning()<<"Błąd odczytu z bazy: "<<query.lastError().text();
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
    float x=floor(faza); //wyciąganie wartości po przecinku
    var_jl=faza-x;

}



void MainWindow::on_save_button_clicked()
{
    QString tekst=ui->plainTextEdit->toPlainText();
    BazaDanychWypelni(tekst);
}

//-----------------------------------------------------------[web]---------------------------------------------------------------

void MainWindow::siec(QString URL)
{
    QNetworkAccessManager *img=new QNetworkAccessManager(this);
    connect(img,&QNetworkAccessManager::finished, this, &MainWindow::PobieranieZakonczone);
    const QUrl addres = QUrl(URL);
    QNetworkRequest zadanie(addres);
    img->get(zadanie);
}

void MainWindow::PobieranieZakonczone(QNetworkReply *replay)
{
    QPixmap img;
    img.loadFromData(replay->readAll());
    ui->label_img->setPixmap(img);
}
//----------------------------------------------[obliczanie fazy]------------------------------------------------------
void MainWindow::phase()
{
    if(0<=var_jl and var_jl<0.25)
    {
       ui->label->setText("Nów");
       siec(NewURL);
    }
    if(0.25<=var_jl and var_jl<0.50)
    {
        ui->label->setText("Pierwsza kwarta");
        siec(FirstquarterURL);
    }
    if(0.50<=var_jl and var_jl<0.75)
    {
        ui->label->setText("Pełnia");
        siec(FullURL);
    }
    if(0.75<=var_jl)
    {
        ui->label->setText("Ostatnia kwarta");
        siec(ThirdquarterURL);
    }
}

//------------------------------------------------------------[ baza danych--------------------------------------------------------------------
void MainWindow::BazaDanychStart()
{
    const QString DRIVER("QSQLITE"); //definiowanie sterownika

    if(QSqlDatabase::isDriverAvailable(DRIVER))//sprawdznie dostępności sterownika
    {
        QSqlDatabase baza=QSqlDatabase::addDatabase(DRIVER); //otwieranie bazy
        baza.setDatabaseName("database.db"); //nadawanie nazwy

        if(!baza.open())
              qWarning()<<"Blad otwarcia bazy danych: "<<baza.lastError();
    }
    else
    {
        qWarning()<<"Blad sterownika: "<<DRIVER<<"dostępny";
    }

}


void MainWindow::BazaDanychWypelni(QString dane)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO notes(day,txt) VALUES(?,?)"); //tworzenie "zapytania"
    query.addBindValue(day_of_year); //bindowanie zmiennej do przekazywanej wartości
    query.addBindValue(dane); //bindowanie zmiennej do przekazywanej wartości
        if (!query.exec()) //wykonanie zapytania
            qWarning() << "Blad zapisu danych do bazy: " << query.lastError().text();
}
