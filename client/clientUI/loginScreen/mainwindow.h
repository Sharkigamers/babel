#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatwindow.h"
#include "signinscreen.h"
#include "QtSocket.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace babel {
    namespace client {
        class MainWindow : public QMainWindow
        {
            Q_OBJECT

        public:
            MainWindow(babel::client::QtSocket &, const std::string &, QWidget *parent = nullptr);
            ~MainWindow();

        private slots:

            void on_pushButton_2_clicked();
            void on_pushButton_clicked();
            babel::client::QtSocket &getSocket() {
                return (_socket);
            }

        private:
            Ui::MainWindow *ui;
            babel::client::QtSocket &_socket;
            std::unique_ptr<babel::client::chatWindow> chatScreen;
            babel::client::signInScreen signInScreen;
            std::string _serverIp;
        };
    }
}
#endif // MAINWINDOW_H
