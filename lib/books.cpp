#include "books.h"
#include "ui_books.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileDialog>

books::books(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::books)
{
    ui->setupUi(this);

    ui->TableWidget->setColumnCount(7);
    QStringList headers = { "Название", "Автор", "Категория", "Год", "ISBN", "Общее количество", "Доступное количество" };
    ui->TableWidget->setHorizontalHeaderLabels(headers);
    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btnAdd, &QPushButton::clicked, this, &books::addBook);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &books::updateBook);
    connect(ui->btnDelete, &QPushButton::clicked, this, &books::deleteBook);
    connect(ui->btnSearch, &QPushButton::clicked, this, &books::searchBook);
    connect(ui->btnSelectImage, &QPushButton::clicked, this, &books::selectImage);

    loadBooks();
}

books::~books()
{
    delete ui;
}

void books::clearForm()
{
    ui->lineTitle->clear();
    ui->Author->clear();
    ui->Category->clear();
    ui->year->clear();
    ui->Isbn->clear();
    ui->Description->clear();
    ui->Total_stock->clear();
    ui->lineImagePath->clear();
    ui->labelImage->clear();
}

void books::fillForm(const QString &title)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE title = :title");
    query.bindValue(":title", title);

    if (query.exec() && query.next()) {
        ui->lineTitle->setText(query.value("title").toString());
        ui->Author->setText(query.value("author").toString());
        ui->Category->setText(query.value("category").toString());
        ui->year->setText(query.value("year").toString());
        ui->Isbn->setText(query.value("isbn").toString());
        ui->Description->setText(query.value("description").toString());
        ui->Total_stock->setText(query.value("total_stock").toString());
        ui->lineImagePath->setText(query.value("image_path").toString());

        QPixmap pixmap(query.value("image_path").toString());
        ui->labelImage->setPixmap(pixmap.scaled(ui->labelImage->size(), Qt::KeepAspectRatio));
    } else {
        QMessageBox::warning(this, "Ошибка", "Книга с таким названием не найдена.");
    }
}

void books::addBook()
{
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, category, year, isbn, description, total_stock, available_stock, image_path) "
                  "VALUES (:title, :author, :category, :year, :isbn, :description, :total_stock, :available_stock, :image_path)");

    query.bindValue(":title", ui->lineTitle->text());
    query.bindValue(":author", ui->Author->text());
    query.bindValue(":category", ui->Category->text());
    query.bindValue(":year", ui->year->text());
    query.bindValue(":isbn", ui->Isbn->text());
    query.bindValue(":description", ui->Description->text());
    query.bindValue(":total_stock", ui->Total_stock->text());
    query.bindValue(":available_stock", ui->Total_stock->text());
    query.bindValue(":image_path", ui->lineImagePath->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Книга успешно добавлена.");
        clearForm();
        loadBooks();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void books::updateBook()
{
    QSqlQuery query;
    query.prepare("UPDATE books SET author = :author, category = :category, year = :year, "
                  "isbn = :isbn, description = :description, total_stock = :total_stock, available_stock = :available_stock, "
                  "image_path = :image_path WHERE title = :title");

    query.bindValue(":title", ui->lineTitle->text());
    query.bindValue(":author", ui->Author->text());
    query.bindValue(":category", ui->Category->text());
    query.bindValue(":year", ui->year->text());
    query.bindValue(":isbn", ui->Isbn->text());
    query.bindValue(":description", ui->Description->text());
    query.bindValue(":total_stock", ui->Total_stock->text());
    query.bindValue(":available_stock", ui->Total_stock->text());
    query.bindValue(":image_path", ui->lineImagePath->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Книга успешно обновлена.");
        loadBooks();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void books::deleteBook()
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE title = :title");
    query.bindValue(":title", ui->lineTitle->text());

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Книга успешно удалена.");
        clearForm();
        loadBooks();
    } else {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
    }
}

void books::searchBook()
{
    QString title = ui->lineTitle->text();
    if (!title.isEmpty()) {
        fillForm(title);
    } else {
        QMessageBox::warning(this, "Ошибка", "Введите название книги для поиска.");
    }
}

void books::loadBooks()
{
    QSqlQuery query("SELECT title, author, category, year, isbn, total_stock, available_stock FROM books");


    ui->TableWidget->setColumnCount(7);
    QStringList headers = { "Название", "Автор", "Категория", "Год", "ISBN", "Общее количество", "Доступное количество" };
    ui->TableWidget->setHorizontalHeaderLabels(headers);
    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->TableWidget->insertRow(row);
        for (int col = 0; col < query.record().count(); ++col) {
            ui->TableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "Информация", "В базе данных нет записей.");
    }
}

void books::selectImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        ui->lineImagePath->setText(filePath);
        QPixmap pixmap(filePath);
        ui->labelImage->setPixmap(pixmap.scaled(ui->labelImage->size(), Qt::KeepAspectRatio));
    }
}
