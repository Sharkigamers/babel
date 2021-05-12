#ifndef SIGNINSCREEN_H
#define SIGNINSCREEN_H

#include <QMainWindow>
#include <QDebug>
#include "QtSocket.hpp"
#include "chatwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class signInScreen; }
QT_END_NAMESPACE

namespace babel {
    namespace client {
        class signInScreen : public QMainWindow
        {
            Q_OBJECT

        public:
            signInScreen(babel::client::QtSocket &, QWidget *parent = nullptr);
            ~signInScreen();

        private slots:

            void on_confirmButton_clicked();
            babel::client::QtSocket &getSocket() {
                return (_socket);
            }

        private:
            Ui::signInScreen *ui;
            babel::client::QtSocket &_socket;
            std::unique_ptr<babel::client::chatWindow> chatScreen;
        };
    }
}
#endif // SIGNINSCREEN_H
