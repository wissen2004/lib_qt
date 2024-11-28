#include "booksdetail.h"
#include "ui_booksdetail.h"
#include <QMessageBox>
#include <QDate>

booksDetail::booksDetail(int userId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::booksDetail),
    currentUserId(userId)
{
    ui->setupUi(this);

    connect(ui->reserveButton, &QPushButton::clicked, this, &booksDetail::reserveBook);

    connect(ui->addCommentButton, &QPushButton::clicked, this, &booksDetail::addComment);

    loadComments();
}

booksDetail::~booksDetail()
{
    delete ui;
}

void booksDetail::loadBookDetails(int bookId)
{
    currentBookId = bookId;

    QSqlQuery query;
    query.prepare("SELECT title, author, category, year, isbn, description, image_path FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (query.exec() && query.next()) {
        QString title = query.value("title").toString();
        QString author = query.value("author").toString();
        QString category = query.value("category").toString();
        int year = query.value("year").toInt();
        QString description = query.value("description").toString();
        QString imagePath = query.value("image_path").toString();

        ui->titleLabel->setText(title);
        ui->authorLabel->setText(author);
        ui->categoryLabel->setText(category);
        ui->yearLabel->setText(QString::number(year));
        ui->textEdit->setText(description);
        ui->textEdit->setReadOnly(true);

        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            ui->label_7->setPixmap(pixmap.scaled(150, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->label_7->setText("Нет изображения");
        }

        loadComments();
    } else {
        qDebug() << "Ошибка загрузки данных книги";
    }
}

void booksDetail::reserveBook()
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM borrows WHERE book_id = :book_id AND status = 'borrowed'");
    checkQuery.bindValue(":book_id", currentBookId);
    checkQuery.exec();

    if (checkQuery.next()) {
        QMessageBox::warning(this, "borrows", "Книга уже забронирована другим пользователем.");
    } else {
        QSqlQuery reserveQuery;
        reserveQuery.prepare("INSERT INTO borrows (user_id, book_id, borrow_date, due_date, status) VALUES (:user_id, :book_id, :borrow_date, :due_date, 'borrowed')");
        reserveQuery.bindValue(":user_id", currentUserId);
        reserveQuery.bindValue(":book_id", currentBookId);
        reserveQuery.bindValue(":borrow_date", QDate::currentDate().toString("yyyy-MM-dd"));
        reserveQuery.bindValue(":due_date", QDate::currentDate().addDays(14).toString("yyyy-MM-dd"));

        if (reserveQuery.exec()) {
            QMessageBox::information(this, "Бронирование", "Книга успешно забронирована.");
        } else {}
    }
}

void booksDetail::addComment()
{
    QString commentText = ui->commentTextEdit->toPlainText();
    if (commentText.isEmpty()) {
        QMessageBox::warning(this, "Комментарий", "Комментарий не может быть пустым.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO comments (user_id, book_id, comment, created_at) VALUES (:user_id, :book_id, :comment, :created_at)");
    query.bindValue(":user_id", currentUserId);
    query.bindValue(":book_id", currentBookId);
    query.bindValue(":comment", commentText);
    query.bindValue(":created_at", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec()) {
        QMessageBox::information(this, "Комментарий", "Комментарий успешно добавлен.");
        ui->commentTextEdit->clear();
        loadComments();
    } else {}
}

void booksDetail::loadComments()
{
    while (QLayoutItem *item = ui->commentsLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QSqlQuery query;
    query.prepare("SELECT users.name, comments.comment, comments.created_at FROM comments JOIN users ON comments.user_id = users.id WHERE comments.book_id = :book_id ORDER BY comments.created_at DESC");
    query.bindValue(":book_id", currentBookId);

    if (query.exec()) {
        while (query.next()) {
            QString userName = query.value("name").toString();
            QString commentText = query.value("comment").toString();
            QString createdAt = query.value("created_at").toString();

            QLabel *userLabel = new QLabel("Пользователь: " + userName, this);
            QLabel *commentLabel = new QLabel("Комментарий: " + commentText, this);
            QLabel *dateLabel = new QLabel("Дата: " + createdAt, this);

            ui->commentsLayout->addWidget(userLabel);
            ui->commentsLayout->addWidget(commentLabel);
            ui->commentsLayout->addWidget(dateLabel);
        }
    } else {}
}
