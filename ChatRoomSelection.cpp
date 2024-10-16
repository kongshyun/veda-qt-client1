#include "ChatRoomSelection.h"

ChatRoomSelection::ChatRoomSelection(const QString &username, const QString &password, QTcpSocket *socket, QWidget *parent)
    : QWidget(parent), username(username), password(password), socket(socket) {
    // Login 객체의 username, password, socket 그대로 이용

    setupUi();  // 위젯 ui 설정
}

void ChatRoomSelection::setupUi(){  // ui 설정
    layout = new QVBoxLayout(this);
    infoLabel = new QLabel("스마트 홈 관리 시스템", this);
    layout->addWidget(infoLabel, 0, Qt::AlignCenter);

    //채팅창1 에어컨관리 UI구성
    airconLayout = new QHBoxLayout();
    airconIcon=new QLabel(this);
    QPixmap airconPixmap("aircon.png");
    QPixmap scaledPixmap = airconPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 아이콘 크기 조정
    airconIcon->setPixmap(scaledPixmap);
    chatRoom1Button = new QPushButton("1 에어컨 관리 채팅", this);
    chatRoom1Button->setFixedHeight(50);  // 버튼 크기 증가
    chatRoom1Button->setFixedWidth(200);  // 버튼의 너비를 고정
    airconLayout->addStretch();  // 왼쪽 여백 조절
    airconLayout->addWidget(airconIcon, 0, Qt::AlignVCenter);  // 아이콘 수직 정렬
    airconLayout->addWidget(chatRoom1Button, 0, Qt::AlignVCenter);  // 버튼 수직 정렬
    airconLayout->addStretch();  // 오른쪽 여백 조절
    layout-> addLayout(airconLayout); //전체 레이아웃에 ariconLayout 추가


    //채팅창2 TV관리 UI구성
    tvLayout = new QHBoxLayout();
    tvIcon=new QLabel(this);
    QPixmap tvPixmap("tv.png");
    QPixmap tvscaledPixmap = tvPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 아이콘 크기 조정
    tvIcon->setPixmap(tvscaledPixmap);
    chatRoom2Button = new QPushButton("2 TV 관리 채팅", this);
    chatRoom2Button->setFixedHeight(50);  // 버튼 크기 증가
    chatRoom2Button->setFixedWidth(200);  // 버튼의 너비를 고정
    tvLayout->addStretch();  // 왼쪽 여백 조절
    tvLayout->addWidget(tvIcon, 0, Qt::AlignVCenter);  // 아이콘 수직 정렬
    tvLayout->addWidget(chatRoom2Button, 0, Qt::AlignVCenter);  // 버튼 수직 정렬
    tvLayout->addStretch();  // 오른쪽 여백 조절
    layout->addLayout(tvLayout);

    setLayout(layout);//전체 레이아웃 출력
    //////////////////////////////////////////////////////

    connect(chatRoom1Button, &QPushButton::clicked, this, &ChatRoomSelection::onChatRoom1Selected);     // 채팅방 1 버튼을 누르면 채팅방 1로 이동
    connect(chatRoom2Button, &QPushButton::clicked, this, &ChatRoomSelection::onChatRoom2Selected);     // 채팅방 2 버튼을 누르면 채팅방 2로 이동
}

void ChatRoomSelection::onChatRoom1Selected() {         // 채팅방 1 버튼을 누를 경우
    if(window1 == false){                               // 이전에 채팅방 1 이 열려있지 않으면 채팅방 열기
        index=1;                                        // 채팅방 번호
        window1 = true;                                 // 채팅방 1이 열려있으므로 true로 변경
        openairconWindow();                               // 채팅창 열기
    }
}

void ChatRoomSelection::onChatRoom2Selected() {         // 채팅방 2 버튼을 누를 경우
    if(window2 == false){                               // 이전에 채팅방 2 이 열려있지 않으면 채팅방 열기
        index=2;                                        // 채팅방 번호
        window2 = true;                                 // 채팅방 1이 열려있으므로 true로 변경
        openwindowWindow();                               // 채팅창 열기
    }
}


//에어컨채팅창--------------------------------------------------

void ChatRoomSelection::openairconWindow() {              // 채팅방 객체 만들어서 윈도우 띄우기
    QTcpSocket *socket = new QTcpSocket(this);          // 각 채팅방에 대해 새로운 소켓 생성 (채팅방끼리 다른 소켓을 사용하기 위해)
                                                        // Login 때 사용했던 소켓 그대로 사용하면 채팅방끼리의 독립성 깨짐
    socket->connectToHost("127.0.0.1", 12345);          // 서버 주소와 포트로 바인드 및 서버 연결

    ChatWindow *chatWindow = new ChatWindow(username, password, socket, index, nullptr); // 소켓 및 사용자 정보를 매개변수로 받아 채팅방 객체 생성
    chatWindow->setAttribute(Qt::WA_DeleteOnClose);                                      // 채팅창이 닫힐 때 메모리 해제 설정
    chatWindow->show();                                                                  // 새로운 ChatWindow를 표시

    connect(chatWindow, &ChatWindow::chatWindowClosed, this, &ChatRoomSelection::onairconWindowClosed);      // 채팅방이 닫힐 때의 시그널과 슬롯 연결
}

void ChatRoomSelection::onairconWindowClosed(int index) {     // 채팅방이 닫힐 경우 발생 (index는 채팅방 번호)
    if(index==1){           // 채팅방 번호가 1일 경우
        window1 = false;    // 채팅방 1 닫힘
    }
    else{                   // 채팅방 번호가 2일 경우
        window2 = true;     // 채팅방 2 닫힘
    }
    qDebug() << "채팅창이 닫혔습니다. index 플래그가 변경되었습니다:" << index;     // 로그 출력
}


//창문 채팅창--------------------------------------------------
void ChatRoomSelection::openwindowWindow() {              // 채팅방 객체 만들어서 윈도우 띄우기
    QTcpSocket *socket = new QTcpSocket(this);          // 각 채팅방에 대해 새로운 소켓 생성 (채팅방끼리 다른 소켓을 사용하기 위해)
        // Login 때 사용했던 소켓 그대로 사용하면 채팅방끼리의 독립성 깨짐
    socket->connectToHost("127.0.0.1", 12345);          // 서버 주소와 포트로 바인드 및 서버 연결

    ChatWindow *chatWindow = new ChatWindow(username, password, socket, index, nullptr); // 소켓 및 사용자 정보를 매개변수로 받아 채팅방 객체 생성
    chatWindow->setAttribute(Qt::WA_DeleteOnClose);                                      // 채팅창이 닫힐 때 메모리 해제 설정
    chatWindow->show();                                                                  // 새로운 ChatWindow를 표시

    connect(chatWindow, &ChatWindow::chatWindowClosed, this, &ChatRoomSelection::onwindowWindowClosed);      // 채팅방이 닫힐 때의 시그널과 슬롯 연결
}

void ChatRoomSelection::onwindowWindowClosed(int index) {     // 채팅방이 닫힐 경우 발생 (index는 채팅방 번호)
    if(index==1){           // 채팅방 번호가 1일 경우
        window1 = false;    // 채팅방 1 닫힘
    }
    else{                   // 채팅방 번호가 2일 경우
        window2 = true;     // 채팅방 2 닫힘
    }
    qDebug() << "채팅창이 닫혔습니다. index 플래그가 변경되었습니다:" << index;     // 로그 출력
}
