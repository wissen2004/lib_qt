#include "users.h"
#include "ui_users.h"
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

users::users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::users)
{
    ui->setupUi(this);

    connect(ui->btnAdd, &QPushButton::clicked, this, &users::addUser);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &users::updateUser);
    connect(ui->btnDelete, &QPushButton::clicked, this, &users::deleteUser);
    connect(ui->btnSearch, &QPushButton::clicked, this, &users::searchUser);

    loadUsers();
}

users::~users()
{
    delete ui;
}

void users::clearForm()
{
    ui->lineName->clear();
    ui->lineEmail->clear();
    ui->linePassword->clear();
    ui->lineRole->clear();
    ui->lineContact->clear();
}

void users::fillForm(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE name = :name");
    query.bindValue(":name", name);

    if (query.exec() && query.next()) {
        ui->lineName->setText(query.value("name").toString());
        ui->lineEmail->setText(query.value("email").toString());
        ui->linePassword->setText(query.value("password").toString());
        ui->lineRole->setText(query.value("role").toString());
        ui->lineContact->setText(query.value("contact").toString());
    } else {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем не найден.");
    }
}

void users::addUser()
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (name, email, password, role, contact, created_at) "
                  "VALUES (:name, :email, :password, :role, :contact, DATETIME('now'))");

    query.bindValue(":name", ui->lineName->text());
    query.bindValue(":email", ui->lineEmail->text());
    query.bindValue(":password", ui->linePassword->text());
    query.bindValue(":role", ui->lineRole->text());
    query.bindValue(":contact", ui->lineContact->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Пользователь успешно добавлен.");
        clearForm();
        loadUsers();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void users::updateUser()
{
    QSqlQuery query;
    query.prepare("UPDATE users SET email = :email, password = :password, role = :role, contact = :contact WHERE name = :name");

    query.bindValue(":name", ui->lineName->text());
    query.bindValue(":email", ui->lineEmail->text());
    query.bindValue(":password", ui->linePassword->text());
    query.bindValue(":role", ui->lineRole->text());
    query.bindValue(":contact", ui->lineContact->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Пользователь успешно обновлен.");
        loadUsers();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void users::deleteUser()
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE name = :name");
    query.bindValue(":name", ui->lineName->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Пользователь успешно удален.");
        clearForm();
        loadUsers();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void users::searchUser()
{
    QString name = ui->lineName->text();
    if (!name.isEmpty()) {
        fillForm(name);
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя для поиска.");
    }
}

void users::loadUsers()
{
    QSqlQuery query("SELECT name, email, role, contact, created_at FROM users");

  ui->tableWidget->setColumnCount(5);
QStringList headers = { "Имя", "Почта", "Пароль", "Роль", "Контакты"};
ui->tableWidget->setHorizontalHeaderLabels(headers);
ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

ui->tableWidget->setRowCount(0);

int row = 0;
while (query.next()) {
    ui->tableWidget->insertRow(row);

    for (int col = 0; col < query.record().count(); ++col) {
        ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
    }

    row++;
}

if (row == 0) {
    QMessageBox::information(this, "Информация", "В базе данных нет записей.");
}
}
