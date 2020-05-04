#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTcpSocket"
#include "QMessageBox"
#include "QTextCodec.h"
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(642,700); // 禁止改变窗口大小。
    socket=new QTcpSocket();
 
    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
 
    ui->textEdit_Recv->setReadOnly(true);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("6000");
}
 
MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}
void MainWindow::on_pushButton_Connect_clicked()
{
 
    if(ui->pushButton_Connect->text() == tr("连接"))
    {
        QString IP;
        int port;
 
        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();
        ui->textEdit_Recv->setText("");
        ui->textEdit_Recv->insertPlainText("正在连接"+ui->lineEdit_IP->text()+":"+ui->lineEdit_Port->text()+"\n");
        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);
        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
            QMessageBox::warning(this,tr("消息"),tr("连接失败！请重新连接"),QMessageBox::Yes);
            ui->textEdit_Recv->insertPlainText("连接失败\n");
            return;
        }
         ui->textEdit_Recv->insertPlainText("连接成功\n");
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
        QMessageBox::information(this,tr("消息"),tr("连接成功"),QMessageBox::Yes);
 
        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        ui->textEdit_Recv->setText("断开连接\n");
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}
 
void MainWindow::on_pushButton_Send_clicked()
{
 
    ui->textEdit_Recv->insertPlainText(tr("李青说：")+ui->textEdit_Send->toPlainText()+"\n");
    socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());
    socket->flush();
    ui->textEdit_Send->setText("");
}
 
void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
 
    if(!buffer.isEmpty())
    {
        QString str = QString::fromLocal8Bit(buffer);
        ui->textEdit_Recv->insertPlainText("服务器消息："+str+"\n");
    }
}
 
void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    qDebug() << "Disconnected!";
}
