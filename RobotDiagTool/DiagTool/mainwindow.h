#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_widget_NewDataToDisplay(int i);

private slots:
    void on_pushButton_2_clicked();

    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
