#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class users;
}

class users : public QWidget
{
    Q_OBJECT

public:
    explicit users(QWidget *parent = nullptr);
    ~users();

private slots:
    void addUser();
    void updateUser();
    void deleteUser();
    void searchUser();
    void loadUsers();

private:
    Ui::users *ui;

    void clearForm();
    void fillForm(const QString &name);
};

#endif // USERS_H
