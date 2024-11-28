#ifndef BOOKSDETAIL_H
#define BOOKSDETAIL_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class booksDetail;
}

class booksDetail : public QWidget
{
    Q_OBJECT

public:
    explicit booksDetail(int userId, QWidget *parent = nullptr);
    ~booksDetail();

    void loadBookDetails(int bookId);

private slots:
    void reserveBook();
    void addComment();
    void loadComments();

private:
    Ui::booksDetail *ui;
    int currentUserId;
    int currentBookId;
};

#endif // BOOKSDETAIL_H
