#include "login.h"

Login::Login(QWidget *parent) : QWidget(parent)
{
    setupUi(); // 로그인 객체 생성되면 UI 설정 시작
}

void Login::setupUi()
{
    // UI 꾸미기
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 이미지 아이콘 추가
    QPixmap pixmap("login.png"); // 이미지 경로 설정(ㄴshow in Explorer)
    QPixmap scaledPixmap = pixmap.scaled(50, 50,\
        Qt::KeepAspectRatio, Qt::SmoothTransformation);//크기 조정, 비율 유지
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(scaledPixmap);
    iconLabel->setAlignment(Qt::AlignCenter); // 이미지 중앙 정렬
    layout->addWidget(iconLabel, 0, Qt::AlignCenter);

    messageLabel = new QLabel("로그인", this);
    layout->addWidget(messageLabel, 0, Qt::AlignCenter);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("사용자 이름");
    layout->addWidget(usernameEdit);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("비밀번호");
    passwordEdit->setEchoMode(QLineEdit::Password); // 비밀번호 숨기기
    layout->addWidget(passwordEdit);
    loginButton = new QPushButton("로그인", this);
    layout->addWidget(loginButton);
    //////////////////////////////

    connect(loginButton, &QPushButton::clicked, this, &Login::handleLogin);      // UI에서의 상호작용 (로그인 버튼 클릭)
}

void Login::handleLogin(){                           // 로그인 버튼 클릭 후 작업
    QString username = usernameEdit->text();         // 로그인 username
    QString password = passwordEdit->text();         // 로그인 password
    QString serverAddress = "127.0.0.1";             // 접속할 서버 ip 주소

    loginSocket = new QTcpSocket(this);               // 로그인시 서버와 연결할 소켓 객체 생성
    loginSocket->connectToHost(serverAddress, 12345); // 소켓 바인드(IP주소, 포트 이용)

    if (loginSocket->waitForConnected()) {                                  // 서버와 연결되면
        qDebug() << "Connected to server.";                                 // 연결 성공 로그 출력

        loginSocket->write(username.toUtf8() + " " + password.toUtf8());     // 로그인 정보(입력한 username, password) 를 서버로 전송

        if (loginSocket->waitForReadyRead()) {                              // 서버의 응답을 기다림
            QByteArray response = loginSocket->readAll();                   // 서버로부터 받은 데이터 ( 서버 응답 == "client connected" )
            QString serverResponse = QString::fromUtf8(response);
            messageLabel->setText(serverResponse);
            emit loginSuccessful(username, password, loginSocket);          // 정상적으로 서버로부터 데이터 받으면 loginSuccessful 시그널 발생
                                                                            // loginSuccessful 시그널은 Client가 받을 예정
                                                                            // 시그널 받으면 Login에서 ChatRoomSelection으로 창 변경
        }
    }
    else {
        messageLabel->setText("서버에 연결할 수 없습니다.");
    }
}
