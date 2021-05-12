/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chatWindow
{
public:
    QWidget *centralwidget;
    QListView *contactList;
    QFrame *frame;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *chatWindow)
    {
        if (chatWindow->objectName().isEmpty())
            chatWindow->setObjectName(QString::fromUtf8("chatWindow"));
        chatWindow->resize(1227, 705);
        centralwidget = new QWidget(chatWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        contactList = new QListView(centralwidget);
        contactList->setObjectName(QString::fromUtf8("contactList"));
        contactList->setGeometry(QRect(0, 0, 291, 651));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(299, -1, 881, 641));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        stackedWidget = new QStackedWidget(frame);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(10, 10, 201, 131));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        chatWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(chatWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1227, 22));
        chatWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(chatWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        chatWindow->setStatusBar(statusbar);

        retranslateUi(chatWindow);

        QMetaObject::connectSlotsByName(chatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *chatWindow)
    {
        chatWindow->setWindowTitle(QCoreApplication::translate("chatWindow", "chatWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class chatWindow: public Ui_chatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
