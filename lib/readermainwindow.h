#ifndef READERMAINWINDOW_H
#define READERMAINWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "profile.h"

namespace Ui {
class ReaderMainWindow;
}

class ReaderMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderMainWindow(int userId, QWidget *parent = nullptr);
    ~ReaderMainWindow();

private slots:
    void showBookDetails(int bookId);
    void openProfile();
    void applyFilters();

private:
    Ui::ReaderMainWindow *ui;
    QSqlQueryModel *model;
    int currentUserId;
    void loadBooks(const QString &queryStr = "");
};

#endif // READERMAINWINDOW_H
