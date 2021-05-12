// /*
// ** EPITECH PROJECT, 2020
// ** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
// ** File description:
// ** CCommandHandler
// */

// #ifndef CCOMMANDHANDLER_HPP_
// #define CCOMMANDHANDLER_HPP_

// #include <memory>
// #include <map>
// #include <functional>

// #include "IProtocol.hpp"
// #include "CServerClient.hpp"
// #include "JsonUtility.hpp"

// namespace babel {
//     namespace server {
//         class CServer;

//         class CCommandHandler {
//             public:
//                 CCommandHandler(babel::server::CServer &);
//                 ~CCommandHandler();

//                 void handleCommand(babel::server::CServerClient &, const babel::protocol::Message &);

//                 void requestConnection(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestCreateAccount(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestAddContact(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestRemoveContact(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestRequestCall(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestHangCall(babel::server::CServerClient &, const babel::protocol::Message &);
//                 void requestContactsInfo(babel::server::CServerClient &, const babel::protocol::Message &);

//                 // babel::server::CServer *getServer() const { return _server; };
//                 // void setServer(babel::server::CServer *serv) { _server = serv; };

//             protected:
//             private:
//                 babel::server::CServer &_server;
//         };
//     }
// }

// #endif /* !CCOMMANDHANDLER_HPP_ */
