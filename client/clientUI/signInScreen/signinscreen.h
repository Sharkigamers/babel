#ifndef SIGNINSCREEN_H
#define SIGNINSCREEN_H

#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class signInScreen; }
QT_END_NAMESPACE

namespace babel {
    namespace client {
        class signInScreen : public QMainWindow
        {
            Q_OBJECT

        public:
            signInScreen(QWidget *parent = nullptr);
            ~signInScreen();

        private slots:

            void on_confirmButton_clicked();

        private:
            Ui::signInScreen *ui;
        };
        }
}
#endif // SIGNINSCREEN_H
