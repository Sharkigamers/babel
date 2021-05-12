/*
** EPITECH PROJECT, 2020
** babel
** File description:
** mainwindow
*/

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <map>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "JsonUtility.hpp"

babel::client::MainWindow::MainWindow(babel::client::QtSocket &socket, QWidget *parent)
    :_socket(socket) 
    , QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

babel::client::MainWindow::~MainWindow()
{
    delete ui;
}

void babel::client::MainWindow::on_pushButton_2_clicked()
{
    qDebug() << ui->lineEdit->text();
    qDebug() << ui->lineEdit_2->text();
    const std::map<std::string, std::string> map = {
        {"Username", ui->lineEdit->text().toUtf8().constData()},
        {"Password", ui->lineEdit_2->text().toUtf8().constData()}
    };
    const std::string message = babel::common::JsonUtility::mapToJson(map);
    babel::client::QtSocket &s = getSocket();

    std::shared_ptr<babel::protocol::Message> lopes = babel::protocol::AProtocol::createConnectionMessage(0, 0, message);
    s.writeToServer(*lopes, [](){
        std::cout << "bigga" << std::endl;
    });
    s.readFromServer();
    this->hide();
    this->c.show();
}

void babel::client::MainWindow::on_pushButton_clicked()
{
    this->hide();
    this->s.show();
}
