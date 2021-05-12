#if defined(_WIN32)
	#include <io.h>
	#include <process.h>
#else
	#include <unistd.h>
#endif
#include <map>
#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "JsonUtility.hpp"


babel::client::chatWindow::chatWindow(babel::client::QtSocket &socket, const babel::client::ContactInfo &currentUser, const std::string &serverIp, QWidget *parent)
    : QMainWindow(parent)
    , _socket(socket)
    , ui(new Ui::chatWindow)
    , _currentUser(currentUser)
    , _ipServer(QString::fromStdString(serverIp))
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString("Session : " + currentUser.getUsername()));
    // boost::thread lopesCount([this] () {
        
    // });
    // _ipServer = socket.getHostName();
    _callManager = std::make_unique<babel::client::CCallManager>(_ipServer.toStdString());
}

void babel::client::chatWindow::init()
{
    _eventServer = std::make_unique<boost::thread>([this]() {
        boost::mutex lopes;
        while (1) {
			boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
            std::shared_ptr<babel::protocol::Message> message = this->_socket.readFromServer();
            lopes.lock();
            if (message->getMessageType() != babel::protocol::NONE) {
                this->_serverMessages.push_back(message);
            }
            lopes.unlock();
        }
    });
    _eventManager = std::make_unique<QTimer>(this);
    connect(_eventManager.get(), &QTimer::timeout, this, &babel::client::chatWindow::treatEvent);
    _eventManager->start(10);
}

void babel::client::chatWindow::treatEvent()
{
    boost::mutex lopes;
    lopes.lock();
    if (this->_serverMessages.size() > 0) {
        if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::CONTACT_INFO) {
            const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(this->_serverMessages[0]->getMessage());
            babel::client::ContactInfo lopes(objects.at("Username"), std::atoi(objects.at("ID").c_str()));
            if (objects.at("Connected") == "false") {
                try
                {
                    _contactList.erase(std::find(_contactList.begin(), _contactList.end(), lopes));
                }
                catch(const std::exception& e)
                {}
            } else if (objects.at("Connected") == "true")
                this->_contactList.push_back(lopes);
            addToListView();
        } else if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::REQUEST_CALL) {
            std::cout << "Request call" << std::endl;
            const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(this->_serverMessages[0]->getMessage());

            if (_callManager->isInCall()) {
                const std::map<std::string, std::string> mapError = {
                    {"Error", "User already in call"}
                };
                const std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(_currentUser.getId(), _serverMessages[0]->fromId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(mapError));

                _socket.writeToServer(*response, [](){});
            } else {
                acceptCall(_serverMessages[0]);
            }
        } else if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::ACCEPT_CALL) {
            std::cout << "Accept call" << std::endl;
            const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(this->_serverMessages[0]->getMessage());

            _callManager->setNewHostPortWrite(_ipServer.toStdString(), static_cast<unsigned int>(std::atoi(objects.at("Port").c_str())));
            _callManager->partnerId(this->_serverMessages[0]->fromId());
        } else if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::RESPONSE_OK && this->_serverMessages[0]->getMessageCategory() == babel::protocol::messageCategory::CALL) {
            const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(this->_serverMessages[0]->getMessage());

            _callManager->setNewHostPortWrite(_ipServer.toStdString(), static_cast<unsigned int>(std::atoi(objects.at("Port").c_str())));
        } else if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::HANGUP) {
            if (_callManager->isInCall()) {
                try
                {
                    _callManager->endCall();       
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
        
        
        //else if (this->_serverMessages[0]->getMessageType() == babel::protocol::messageType::RESPONSE_KO && this->_serverMessages[0]->getMessageCategory() == babel::protocol::messageCategory::CALL) {
        //     const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(this->_serverMessages[0]->getMessage());

        //     if (_callManager->isInCall()) {
        //         const std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(_currentUser.getId(), _serverMessages[0]->fromId(), babel::protocol::CALL, "");

        //         _callManager->endCall();

        //         _socket.writeToServer(*response, [](){});
        //     }
        // }
        this->_serverMessages.erase(_serverMessages.begin());
    }
    lopes.unlock();
}

babel::client::chatWindow::~chatWindow()
{
}

void babel::client::chatWindow::addToListView()
{
    model = std::make_unique<QStringListModel>(this);

    QStringList List;
    for (const auto &x : _contactList) {
        QString lopes = QString::fromStdString(x.getUsername());
        List.push_back(lopes);
    }
    model->setStringList(List);
    ui->listView->setModel(model.get());
}

void babel::client::chatWindow::startCall(const unsigned int toId)
{
    _callManager->initCall();
    
    const std::map<std::string, std::string> objectMap = {
        {"Port", "42423"}
    };

    const std::shared_ptr<babel::protocol::Message> message = babel::protocol::AProtocol::createCallMessage(_currentUser.getId(), toId, babel::common::JsonUtility::mapToJson(objectMap));

    _socket.writeToServer(*message, [](){});
}

void babel::client::chatWindow::acceptCall(const std::shared_ptr<babel::protocol::Message> &message)
{
    _callManager->startCall(*message);

    std::cout << "From Id: " << message->fromId() << std::endl;
    std::cout << "To Id: " << message->toId() << std::endl;

    const std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createAcceptCallMessage(_currentUser.getId(), message->fromId(), message->getMessage());

    _socket.writeToServer(*response, [](){});
    _callManager->partnerId(message->toId());
}

void babel::client::chatWindow::on_pushButton_4_clicked()
{
    const std::map<std::string, std::string> map = {
        {"Username", ui->contactEdit->text().toUtf8().constData()}
    };
    const std::string message = babel::common::JsonUtility::mapToJson(map);

    std::shared_ptr<babel::protocol::Message> lopes = babel::protocol::AProtocol::createAddContactMessage(_currentUser.getId(), 0, message);
    _socket.writeToServer(*lopes, [](){
        std::cout << "bigga" << std::endl;
    });
}

void babel::client::chatWindow::on_listView_clicked(const QModelIndex &index)
{
    (void)index;
    if (_callManager->isInCall()) return;
    QModelIndex indexLopes = ui->listView->currentIndex();
    QString itemText = indexLopes.data(Qt::DisplayRole).toString();
    for (const auto &x : _contactList) {
        if (itemText.toStdString() == x.getUsername())
            startCall(x.getId());
    }
}

void babel::client::chatWindow::on_pushButton_clicked()
{
    //add call function here
}

void babel::client::chatWindow::on_pushButton_2_clicked()
{
    if (_callManager->isInCall()) {
        try
        {
            std::shared_ptr<babel::protocol::Message> message = babel::protocol::AProtocol::createHangupMessage(_currentUser.getId(), _callManager->partnerId(), "");
            _socket.writeToServer(*message, [](){});
            _callManager->endCall();       
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
