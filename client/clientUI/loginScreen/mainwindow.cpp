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

babel::client::MainWindow::MainWindow(babel::client::QtSocket &socket, const std::string &serverIp, QWidget *parent)
    :_socket(socket) 
    , QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signInScreen(socket)
    , _serverIp(serverIp)
    // , chatScreen(socket)
{
    ui->setupUi(this);
}

babel::client::MainWindow::~MainWindow()
{
    delete ui;
}

void babel::client::MainWindow::on_pushButton_2_clicked()
{
    if (ui->lineEdit->text() == nullptr || ui->lineEdit_2->text() == nullptr)
        return;
    const std::map<std::string, std::string> map = {
        {"Username", ui->lineEdit->text().toUtf8().constData()},
        {"Password", ui->lineEdit_2->text().toUtf8().constData()}
    };
    const std::string message = babel::common::JsonUtility::mapToJson(map);

    std::shared_ptr<babel::protocol::Message> lopes = babel::protocol::AProtocol::createConnectionMessage(0, 0, message);
    _socket.writeToServer(*lopes, [](){
        std::cout << "bigga" << std::endl;
    });
    const std::shared_ptr<babel::protocol::Message> response = _socket.readFromServer();
    if (response->getMessageType() == babel::protocol::RESPONSE_OK) {
        this->hide();
        this->chatScreen = std::make_unique<babel::client::chatWindow>(getSocket(), babel::client::ContactInfo(map.at("Username"), response->toId()), _serverIp);
        this->chatScreen->init();
        this->chatScreen->show();
    } else {
        std::cout << "Connection denied, create an account" << std::endl;
    }
}

void babel::client::MainWindow::on_pushButton_clicked()
{
    this->hide();
    this->signInScreen.show();
}
