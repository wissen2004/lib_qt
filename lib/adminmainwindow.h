#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include <QWidget>

namespace Ui {
class AdminMainWindow;
}

class AdminMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMainWindow(QWidget *parent = nullptr);
    ~AdminMainWindow();

private slots:
    void openBooksWindow();
    void openUsersWindow();

private:
    Ui::AdminMainWindow *ui;
};

#endif // ADMINMAINWINDOW_H
