#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"
#include "books.h"
#include "users.h"

AdminMainWindow::AdminMainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &AdminMainWindow::openBooksWindow);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &AdminMainWindow::openUsersWindow);
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

void AdminMainWindow::openBooksWindow()
{
    books *booksWindow = new books();
    booksWindow->show();
}

void AdminMainWindow::openUsersWindow()
{
    users *usersWindow = new users();
    usersWindow->show();
}
