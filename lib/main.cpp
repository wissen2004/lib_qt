#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\ASUS\\Desktop\\3kurs\\System_programming\\lib\\lib.db");

    if(db.open()) {
        qDebug() << "Database opened successfully";
    } else {
        qDebug() << "Failed to open the database";
    }


    login w;
    w.show();


    return a.exec();
}
