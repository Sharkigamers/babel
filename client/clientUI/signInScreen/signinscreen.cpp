#include "signinscreen.h"
#include "ui_signinscreen.h"

babel::client::signInScreen::signInScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::signInScreen)
{
    ui->setupUi(this);
}

babel::client::signInScreen::~signInScreen()
{
    delete ui;
}

void babel::client::signInScreen::on_confirmButton_clicked()
{
    if (ui->passwordEdit->text() != ui->passwordEditConfirmation->text()) {
        qDebug() << "password are not matching";
    } else
        qDebug() << "great passwords !!";
}
