/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** main
*/

#include "mainwindow.h"
#include "CAudio.hpp"
#include "COpus.hpp"
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
    std::vector<uint32_t> record;
    std::vector<uint32_t> encode;
    while (1) {
        record = myAudio.recordAudio();
        encode = myOpus.encode(record);
        myAudio.playAudio(record);
    }
}

void eydou(int ac, char **av)
{
    QApplication a(ac, av);
    babel::client::QtSocket s("127.0.0.1", 4242);
    babel::client::MainWindow w(s);

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
    boost::thread lo(boost::bind(&lopes));
    // boost::thread ey(boost::bind(&eydou, argc, argv));
    
    // ey.join();
    lo.join();

    return 0;
}
