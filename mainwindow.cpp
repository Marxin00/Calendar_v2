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
    phase();//wywołanie klasy phase
    WebRequest(TestURL);//wywołanie klasy webReq z parmetrem Test

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


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)//klasa przy zmianie wartości daty
{
    day_of_jl_year=date.toJulianDay();
    day_of_year=date.dayOfYear();
    float faza=((day_of_jl_year/29.5305902778)-0.3033);
    float x=floor(faza); //wyciąganie wartości po przecinku
    var_jl=faza-x;
    //definiowanie i zmienianie TestURL
    QString month=date.toString("MMMM").toLower();
    QString daynum= date.toString("d");
    TestURL ="https://www.namedaycalendar.com/poland/january/1";
    TestURL.replace("january",month).replace("1",daynum);
}



void MainWindow::on_save_button_clicked()//zapisywanie tekstu do bazy danych
{
    QString tekst=ui->plainTextEdit->toPlainText();
    BazaDanychWypelni(tekst);
}

//-----------------------------------------------------------[web]---------------------------------------------------------------

void MainWindow::WebRequest(QString URL)//wysyłanie żądania do witryny
{
    QNetworkAccessManager *img=new QNetworkAccessManager(this);
    connect(img,&QNetworkAccessManager::finished, this, &MainWindow::RequestProcessing);
    QNetworkRequest zadanie(URL);
    img->get(zadanie);
}

void MainWindow::RequestProcessing(QNetworkReply *replay)//przetwarzanie odpowiedzi żądania
{
    QString txt;
    txt=(replay->readAll());
    int startid=txt.indexOf("<div class=\"name\">");
    int endid=txt.lastIndexOf("<div class=\"name\">");
    int dif=(endid+20)-startid;
    QString ime = txt.mid(startid+18,dif);
    ime.remove("<div class=\"name\">").remove("</div>").remove("</div").remove("</di").remove("</d").remove("</");
    ui->label_imieniny->setText(ime);
}

//----------------------------------------------[obliczanie fazy]------------------------------------------------------
void MainWindow::phase()
{
    const QPixmap now_img(":/resources/phases/phase_new.jpg");
    const QPixmap pelnia_img(":/resources/phases/phase_full.jpg");
    const QPixmap pierwsza_kwarta_img(":/resources/phases/phase_first_quarte.jpg");
    const QPixmap ostatnia_kwarta_img(":/resources/phases/phase_third_quart.jpg");

    int with=ui->label_img->width();
    int height=ui->label_img->height();

    if(0<=var_jl and var_jl<0.25)
    {
       ui->label->setText("Nów");
       ui->label_img->setPixmap(now_img.scaled(with,height,Qt::KeepAspectRatio));
    }
    if(0.25<=var_jl and var_jl<0.50)
    {
        ui->label->setText("Pierwsza kwarta");
        ui->label_img->setPixmap(pierwsza_kwarta_img.scaled(with,height,Qt::KeepAspectRatio));
    }
    if(0.50<=var_jl and var_jl<0.75)
    {
        ui->label->setText("Pełnia");
        ui->label_img->setPixmap(pelnia_img.scaled(with,height,Qt::KeepAspectRatio));
    }
    if(0.75<=var_jl)
    {
        ui->label->setText("Ostatnia kwarta");
        ui->label_img->setPixmap(ostatnia_kwarta_img.scaled(with,height,Qt::KeepAspectRatio));
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
