#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
#include <QTcpSocket>
 
namespace Ui {
class MainWindow;
}
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
 
private slots:
 
    void on_pushButton_Connect_clicked();
 
    void on_pushButton_Send_clicked();
 
    void socket_Read_Data();
 
    void socket_Disconnected();
 
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};
 
#endif // MAINWINDOW_H
