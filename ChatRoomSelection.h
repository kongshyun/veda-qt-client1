#ifndef CHATROOMSELECTION_H
#define CHATROOMSELECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTcpServer>
#include "ChatWindow.h"

class ChatRoomSelection : public QWidget {
    Q_OBJECT

public:
    ChatRoomSelection(const QString &username, const QString &password, QTcpSocket * socket, QWidget *parent = nullptr);

private:
    void openairconWindow();              // 채팅방 서버 연결 후 에어컨 채팅창UI띄우기
    void openwindowWindow();              // 채팅방 서버 연결 후 창문 채팅창 UI띄우기
    void setupUi();                     // UI 설정
    QVBoxLayout *layout;//전체 레이아웃
    QHBoxLayout *airconLayout;//에어컨 버튼쪽
    QHBoxLayout *tvLayout;//TV 버튼쪽


    QLabel *infoLabel;//메인제목
    QLabel *airconIcon;//에어컨이미지아이콘
    QLabel *tvIcon;//티비이미지아이콘

    QPushButton *chatRoom1Button;
    QPushButton *chatRoom2Button;

    QTcpSocket *socket;                 // 각 채팅방이 사용할 소켓 정보
    QString username;
    QString password;
    bool window1 = false;               // 채팅방 1이 열려있는지 확인하는 플래그
    bool window2 = false;               // 채팅방 2이 열려있는지 확인하는 플래그
    int index=0;                        // 채팅방 숫자( 1, 2) 저장할 변수
    void onairconWindowClosed(int index); // 채팅방 닫힐경우 실행 될 메소드
    void onwindowWindowClosed(int index); // 채팅방 닫힐경우 실행 될 메소드


private slots:
    void onChatRoom1Selected();         // 채팅방 1 버튼이 눌릴경우 수행
    void onChatRoom2Selected();         // 채팅방 2 버튼이 눌릴경우 수행

};

#endif // CHATROOMSELECTION_H
