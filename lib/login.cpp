#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "adminmainwindow.h"
#include "readermainwindow.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &login::onLoginClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &login::onExitClicked);
}

login::~login()
{
    delete ui;
}

void login::onLoginClicked()
{
    QString email = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QSqlQuery query;
    query.prepare("SELECT id, name, role FROM users WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            int userId = query.value("id").toInt();
            QString role = query.value("role").toString();

            if (role == "admin") {
                AdminMainWindow *adminWindow = new AdminMainWindow();
                adminWindow->show();
                this->close();
            } else if (role == "reader") {
                ReaderMainWindow *readerWindow = new ReaderMainWindow(userId);
                readerWindow->show();
                this->close();
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError();
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных.");
    }
}

void login::onExitClicked()
{
    QApplication::quit();
}
