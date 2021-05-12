/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** main
*/

#include "mainwindow.h"
#include "CAudio.hpp"
#include "COpus.hpp"
#include "CCallManager.hpp"
#include "QtSocket.hpp"

#include <boost/thread.hpp>

#include <QApplication>

void lopes()
{
    babel::client::CAudio myAudio;
    babel::client::COpus myOpus(myAudio.getSampleRate(), myAudio.getChannelNumber());

    myAudio.openRecordAudio();
    myAudio.openPlayAudio();
    myAudio.startStream(RECORD);
    myAudio.startStream(PLAY);
    std::vector<uint16_t> record;
    std::vector<uint16_t> decode;
    std::vector<uint16_t> encode;
    while (1) {
        record = myAudio.recordAudio();
        encode = myOpus.encode(record);
        decode = myOpus.decode(encode);
        myAudio.playAudio(decode);
    }
}

void eydou(int ac, char **av)
{
    QApplication a(ac, av);
    babel::client::QtSocket s(QString::fromStdString(std::string(av[1])), 42422);
    babel::client::MainWindow w(s, std::string(av[1]));

    // std::shared_ptr<babel::protocol::Message> lopes = babel::protocol::AProtocol::createConnectionMessage(0, 0, "lopes, jesuisgay78");
    // s.writeToServer(*lopes, [](){
    //     std::cout << "bigga" << std::endl;
    // });
    // s.readFromServer();
    w.show();
    a.exec();
}

int main(int argc, char *argv[])
{
    // boost::thread lo(boost::bind(&lopes));
    boost::thread ey(boost::bind(&eydou, argc, argv));
    
    ey.join();
    // lo.join();
    // babel::client::CCallManager call;
    // const std::map<std::string, std::string> info = {
    //     {"Host", "127.0.0.1"},
    //     {"Port", "4243"}
    // };
    // const std::shared_ptr<babel::protocol::Message> message = babel::protocol::AProtocol::createCallMessage(0, 0, babel::common::JsonUtility::mapToJson(info));

    // const std::map<std::string, std::string> info2 = {
    //     {"Host", "127.0.0.1"},
    //     {"Port", "4244"}
    // };
    // const std::shared_ptr<babel::protocol::Message> message2 = babel::protocol::AProtocol::createCallMessage(0, 0, babel::common::JsonUtility::mapToJson(info2));


    // if (argc != 3) {
    //     call.initCall(*message2);
    //     while (call.isInCall());
    // } else {
    //     call.startCall(*message);
    //     while (call.isInCall());
    // }

    // return 0;
}
