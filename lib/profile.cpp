#include "profile.h"
#include "ui_profile.h"
#include <QMessageBox>

profile::profile(int userId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::profile),
    currentUserId(userId)
{
    ui->setupUi(this);

    loadProfile();
    loadBorrowedBooks();

    connect(ui->pushButton, &QPushButton::clicked, this, &profile::updateProfile);
}

profile::~profile()
{
    delete ui;
}

void profile::loadProfile()
{
    QSqlQuery query;
    query.prepare("SELECT name, email, password, contact FROM users WHERE id = :id");
    query.bindValue(":id", currentUserId);
    if (query.exec() && query.next()) {
        ui->nameEdit->setText(query.value("name").toString());
        ui->emailEdit->setText(query.value("email").toString());
        ui->passwordEdit->setText(query.value("password").toString());
        ui->contactEdit->setText(query.value("contact").toString());
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить данные профиля.");
    }
}

void profile::updateProfile()
{
    QSqlQuery query;
    query.prepare("UPDATE users SET name = :name, email = :email, password = :password, contact = :contact WHERE id = :id");
    query.bindValue(":name", ui->nameEdit->text());
    query.bindValue(":email", ui->emailEdit->text());
    query.bindValue(":password", ui->passwordEdit->text());
    query.bindValue(":contact", ui->contactEdit->text());
    query.bindValue(":id", currentUserId);
    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Профиль обновлен успешно.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить профиль.");
    }
}

void profile::loadBorrowedBooks()
{
    QLayoutItem *item;
    while ((item = ui->borrowedBooksLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QSqlQuery query;
    query.prepare("SELECT books.id, books.title FROM borrows JOIN books ON borrows.book_id = books.id WHERE borrows.user_id = :user_id AND borrows.status = 'borrowed'");
    query.bindValue(":user_id", currentUserId);
    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            int bookId = query.value("id").toInt();
            QString bookTitle = query.value("title").toString();

            QLabel *titleLabel = new QLabel(bookTitle, this);
            titleLabel->setFixedSize(100, 30);

            QPushButton *returnButton = new QPushButton("Вернуть", this);
            returnButton->setFixedSize(100, 30);

            connect(returnButton, &QPushButton::clicked, this, [=]() { returnBook(bookId); });

            ui->borrowedBooksLayout->addWidget(titleLabel, row, 0);
            ui->borrowedBooksLayout->addWidget(returnButton, row, 1);
            row++;
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить забронированные книги.");
    }

    ui->borrowedBooksLayout->setRowStretch(0, 1);
    ui->borrowedBooksLayout->setColumnStretch(0, 1);
}



void profile::returnBook(int bookId)
{
    QSqlQuery query;
    query.prepare("UPDATE borrows SET status = 'returned', return_date = :return_date WHERE book_id = :book_id AND user_id = :user_id");
    query.bindValue(":book_id", bookId);
    query.bindValue(":user_id", currentUserId);
    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Книга успешно возвращена.");
        loadBorrowedBooks();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось вернуть книгу.");
    }
}
