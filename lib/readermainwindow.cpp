#include "readermainwindow.h"
#include "ui_readermainwindow.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include "booksdetail.h"
#include "profile.h"

ReaderMainWindow::ReaderMainWindow(int userId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderMainWindow),
    model(new QSqlQueryModel(this)),
    currentUserId(userId)
{
    ui->setupUi(this);

    connect(ui->profile, &QPushButton::clicked, this, &ReaderMainWindow::openProfile);

    connect(ui->searchButton, &QPushButton::clicked, this, &ReaderMainWindow::applyFilters);

    loadBooks();
}

ReaderMainWindow::~ReaderMainWindow()
{
    delete ui;
}

void ReaderMainWindow::loadBooks(const QString &queryStr)
{
    QLayoutItem *item;
    while ((item = ui->gridLayout_2->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QSqlQuery query;
    if (queryStr.isEmpty()) {
        query.exec("SELECT id, title, author, image_path FROM books");
    } else {
        query.exec(queryStr);
    }

    int row = 0, col = 0;

    while (query.next()) {
        int bookId = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString author = query.value("author").toString();
        QString imagePath = query.value("image_path").toString();

        QWidget *bookWidget = new QWidget(this);
        QVBoxLayout *bookLayout = new QVBoxLayout(bookWidget);

        bookWidget->setMinimumSize(150, 150);
        bookWidget->setMaximumSize(150, 150);
        bookWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QLabel *imageLabel = new QLabel(bookWidget);
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bookLayout->addWidget(imageLabel);

        QLabel *titleLabel = new QLabel(title, bookWidget);
        bookLayout->addWidget(titleLabel);

        QLabel *authorLabel = new QLabel(author, bookWidget);
        bookLayout->addWidget(authorLabel);

        QPushButton *detailsButton = new QPushButton("Подробнее", bookWidget);
        bookLayout->addWidget(detailsButton);

        connect(detailsButton, &QPushButton::clicked, this, [=]() { showBookDetails(bookId); });

        ui->gridLayout_2->addWidget(bookWidget, row, col);

        col++;
        if (col >= 5) {
            col = 0;
            row++;
        }
    }
}

void ReaderMainWindow::applyFilters()
{
    QString queryStr = "SELECT id, title, author, category, year, image_path FROM books WHERE 1=1";

    if (ui->titleCheckBox->isChecked()) {
        queryStr += " AND title LIKE '%" + ui->titleEdit->text() + "%'";
    }
    if (ui->authorCheckBox->isChecked()) {
        queryStr += " AND author LIKE '%" + ui->authorEdit->text() + "%'";
    }
    if (ui->categoryCheckBox->isChecked()) {
        queryStr += " AND category LIKE '%" + ui->categoryEdit->text() + "%'";
    }
    if (ui->dateCheckBox->isChecked()) {
        queryStr += " AND year BETWEEN '" + ui->startDateEdit->text() + "' AND '" + ui->endDateEdit->text() + "'";
    }

    loadBooks(queryStr);
}

void ReaderMainWindow::showBookDetails(int bookId)
{
    booksDetail *detailsPage = new booksDetail(currentUserId);
    detailsPage->loadBookDetails(bookId);
    detailsPage->show();
}

void ReaderMainWindow::openProfile()
{
    profile *profilePage = new profile(currentUserId);
    profilePage->show();
}
