#include "chatwindow.h"
#include "ui_chatwindow.h"

babel::client::chatWindow::chatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::chatWindow)
{
    ui->setupUi(this);
}

babel::client::chatWindow::~chatWindow()
{
    delete ui;
}

