#ifndef BOOKS_H
#define BOOKS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class books;
}

class books : public QWidget
{
    Q_OBJECT

public:
    explicit books(QWidget *parent = nullptr);
    ~books();

private slots:
    void addBook();
    void updateBook();
    void deleteBook();
    void searchBook();
    void loadBooks();
    void selectImage();

private:
    Ui::books *ui;

    void clearForm();
    void fillForm(const QString &title);
};

#endif // BOOKS_H
