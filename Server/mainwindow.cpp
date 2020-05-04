#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QTextCodec.h"
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(550,583); // 禁止改变窗口大小。
    ui->textEdit_Recv->setReadOnly(true);
    ui->lineEdit_Port->setText("6000");
    ui->pushButton_Send->setEnabled(false);
 
    server = new QTcpServer();
 
    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
}
 
MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}
void MainWindow::on_pushButton_Listen_clicked()
{
    ui->textEdit_Recv->setText("");
    if(ui->pushButton_Listen->text() == tr("OpenServer"))
    {
        server = new QTcpServer();
 
        //连接信号槽
        connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
        ui->textEdit_Recv->insertPlainText("OpenSuccessfully\n");
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();
 
        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
            QMessageBox::information(this,tr("消息"),tr("打开服务器失败！"),QMessageBox::Yes);
            return;
        }
        //修改按键文字
       ui->pushButton_Listen->setText("CloseServer");
 
       QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
       QMessageBox::information(this,tr("消息"),tr("打开服务器成功！"),QMessageBox::Yes);
    }
    else
    {
        server->deleteLater();
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("OpenServer");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
        ui->textEdit_Recv->insertPlainText("CloseSuccessfully\n");
    }
 
}
 
void MainWindow::on_pushButton_Send_clicked()
{
 
    ui->textEdit_Recv->insertPlainText(tr("服务器：")+ui->textEdit_Send->toPlainText()+"\n");
    socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());
    socket->flush();
    ui->textEdit_Send->setText("");
}
 
void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //发送按键使能
    ui->pushButton_Send->setEnabled(true);
 
}
 
void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = QString::fromLocal8Bit(buffer);
        ui->textEdit_Recv->insertPlainText("李青说："+str+"\n");
    }
}
 
void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
}
