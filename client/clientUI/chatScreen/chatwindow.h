#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class chatWindow; }
QT_END_NAMESPACE

namespace babel {
    namespace client {
        class chatWindow : public QMainWindow
        {
            Q_OBJECT

        public:
            chatWindow(QWidget *parent = nullptr);
            ~chatWindow();

        private:
            Ui::chatWindow *ui;
        };
    }
}
#endif // CHATWINDOW_H
