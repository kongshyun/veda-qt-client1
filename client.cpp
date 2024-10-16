#include "Client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    loginWidget = new Login(this);                                              // main 에서 Client 생성 및 이동 후 Login 위젯 생성
    setCentralWidget(loginWidget);                                              // Login 위젯 실행 (초기 화면)

                                                                                // Login 객체에서 로그인 성공 시(Login::loginSuccessful) 핸들러 연결
    connect(loginWidget, &Login::loginSuccessful, this, &Client::afterLogin);   // 로그인 위젯에서 채팅방 선택하는 위젯으로 변경하는 기능
}

Client::~Client()
{
}

void Client::afterLogin(const QString &username, const QString &password, QTcpSocket *socket)   // 로그인 이후 채팅방 선택하는 위젯으로 전환
{
    this->username = username;  // 프로그램에서 사용할 사용자 이름 저장
    this->password = password;  // 프로그램에서 사용할 비밀번호 저장
    this->socket = socket;      // 채팅창과 서버랑 연결할때 사용할 소켓 저장

                                // 소켓의 부모를 Client로 설정
    socket->setParent(this);    // Client가 소켓을 관리하도록 설정 (매우 중요, 이 코드 없으면 서버 통신이 중간에 끊김)


    chatRoomSelection = new ChatRoomSelection(username, password, socket, this);
    setCentralWidget(chatRoomSelection);        // ChatRoomSelection (채팅방 선택하는) 윈도우로 전환
}
