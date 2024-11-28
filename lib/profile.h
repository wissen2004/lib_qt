#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class profile;
}

class profile : public QWidget
{
    Q_OBJECT

public:
    explicit profile(int userId, QWidget *parent = nullptr);
    ~profile();

private slots:
    void updateProfile();
    void returnBook(int bookId);

private:
    Ui::profile *ui;
    int currentUserId;
    void loadProfile();
    void loadBorrowedBooks();
};

#endif // PROFILE_H
