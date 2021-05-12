#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <boost/thread.hpp>
#include <QMainWindow>
#include <QTimer>
#include "QtSocket.hpp"
#include "QtDataSocket.hpp"
#include "AProtocol.hpp"
#include "ContactInfo.hpp"
#include "CCallManager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class chatWindow; }
QT_END_NAMESPACE

namespace babel {
    namespace client {
        class chatWindow : public QMainWindow
        {
            Q_OBJECT

        public:
            chatWindow(babel::client::QtSocket &, const babel::client::ContactInfo& , const std::string &, QWidget *parent = nullptr);
            ~chatWindow();

            void init();
            void startCall(const unsigned int);
            void acceptCall(const std::shared_ptr<babel::protocol::Message> &);
            void initDataSocket(const QString &, qint16);
            void treatEvent();
            void addToListView();

        private slots:
            void on_pushButton_4_clicked();
            void on_pushButton_clicked();
            void on_pushButton_2_clicked();
            void on_listView_clicked(const QModelIndex &index);

        private:
            babel::client::QtSocket &_socket;
            Ui::chatWindow *ui;
            std::unique_ptr<QTimer> _eventManager;
            std::vector<std::shared_ptr<babel::protocol::Message> > _serverMessages;
            std::unique_ptr<boost::thread> _eventServer;
            std::unique_ptr<boost::thread> _lopesEvent;
            babel::client::ContactInfo _currentUser;
            std::vector<babel::client::ContactInfo> _contactList;
            std::unique_ptr<QStringListModel> model;
            std::unique_ptr<babel::client::CCallManager> _callManager;
            QString _ipServer;
        };
    }
}
#endif // CHATWINDOW_H
