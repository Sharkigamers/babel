#include "signinscreen.h"
#include "ui_signinscreen.h"
#include "JsonUtility.hpp"

babel::client::signInScreen::signInScreen(babel::client::QtSocket &socket, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::signInScreen)
    , _socket(socket)
    // , chatScreen(socket)
{
    ui->setupUi(this);
}

babel::client::signInScreen::~signInScreen()
{
    delete ui;
}

void babel::client::signInScreen::on_confirmButton_clicked()
{
    if (ui->usernameEdit->text() == nullptr || ui->passwordEdit->text() == nullptr)
        return;
    if (ui->passwordEdit->text() != ui->passwordEditConfirmation->text()) {
        qDebug() << "password are not matching";
    } else {
        const std::map<std::string, std::string> map = {
            {"Username", ui->usernameEdit->text().toUtf8().constData()},
            {"Password", ui->passwordEdit->text().toUtf8().constData()}
        };
        const std::string message = babel::common::JsonUtility::mapToJson(map);

        std::shared_ptr<babel::protocol::Message> lopes = babel::protocol::AProtocol::createCreateAccountMessage(0, 0, message);
        _socket.writeToServer(*lopes, [](){
            std::cout << "bigga" << std::endl;
        });
        const std::shared_ptr<babel::protocol::Message> response = _socket.readFromServer();


        if (response->getMessageType() == babel::protocol::RESPONSE_OK) {
            this->hide();
            this->chatScreen = std::make_unique<babel::client::chatWindow>(getSocket(), babel::client::ContactInfo(map.at("Username"), response->toId()), std::string("127.0.0.1"));
            this->chatScreen->init();
            this->chatScreen->show();
        } else {
            std::cout << "Connection denied, account already exist" << std::endl;
            ui->usernameEdit->setStyleSheet("border:red");
        }
    }
}
