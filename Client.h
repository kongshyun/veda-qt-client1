#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include "ChatRoomSelection.h"
#include "login.h"

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void afterLogin(const QString &username, const QString &password, QTcpSocket * socket);     // Login 객체에서 사용한 멤버변수들을 그대로 매개변수로 사용

private:
    Login *loginWidget;                         // Login 수행할 객체
    ChatRoomSelection *chatRoomSelection;       // 채팅방 선택화면 나오는 객체

    QTcpSocket *socket;         // Login 객체에서 사용한 소켓을 이어 받을 변수 (서버와의 연결 유지 위해)
    QString username;           // Login 객체에서 사용한 username을 받을 변수 (채팅방에서도 사용할 사용자 이름)
    QString password;           // Login 객체에서 사용한 password를 받을 변수
    //QString port;             // Login 객체에서 사용한 포트번호를 받을 변수 (필요 없을 듯)
};

#endif // CLIENT_H
