#ifndef LOGIN_H
#define LOGIN_H

#include <QVBoxLayout> // QVBoxLayout을 사용하기 위한 헤더 파일
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

#include "clientData.h"

class Login : public QWidget
{
    Q_OBJECT // Qt의 신호 및 슬롯을 사용하기 위해 필요

public:
    Login(QWidget *parent = nullptr);

signals:
    void loginSuccessful(const QString &username, const QString &password, QTcpSocket * socket);

private slots:
    void handleLogin();             // 로그인 버튼 누른 후 로그인 처리

private:
    void setupUi();                 // ui 생성
    QLabel *messageLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *portEdit;
    QPushButton *loginButton;

    QTcpSocket * loginSocket;       // 로그인 시 서버와 연결할 소켓

};

#endif // LOGIN_H
