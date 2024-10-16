#include "ChatWindow.h"

ChatWindow::ChatWindow(const QString &username, const QString &password, QTcpSocket *socket, int index, QWidget *parent)
    : QWidget(parent), username(username), password(password), socket(socket), index(index) {

    setupUi(); // 채팅방 객체 생성 시 UI 설정
}


void ChatWindow::setupUi(){
    mainLayout = new QHBoxLayout(this);
    airconManageUi(mainLayout);
    chatUi(mainLayout);

    setLayout(mainLayout);
    setWindowTitle(QString("채팅창 %1").arg(index));
}

//채팅UI
void ChatWindow::chatUi(QHBoxLayout *mainLayout) {       // UI 꾸미기

    chatLayout = new QVBoxLayout();

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);       // 채팅 내용을 수정할 수 없도록 설정
    chatLayout->addWidget(chatDisplay);

    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("메시지를 입력하세요...");  // 메시지 입력 프롬프트
    chatLayout->addWidget(messageInput);

    sendButton = new QPushButton("전송", this);               // 메시지 전송 버튼
    chatLayout->addWidget(sendButton);

    sendFileButton = new QPushButton("파일 전송", this);       // 파일 전송 버튼
    chatLayout->addWidget(sendFileButton);

    mainLayout->addLayout(chatLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);  // 버튼 누를 시 메시지 전송
    connect(sendFileButton, &QPushButton::clicked, this, &ChatWindow::sendFile); // 버튼 누를 시 파일 전송
    connect(socket, &QTcpSocket::readyRead, this, &ChatWindow::onReadyRead);     // 서버에서 echo 데이터 받아서 채팅창에 출력
    connect(socket, &QTcpSocket::connected, this, &ChatWindow::onConnected);     // 클라이언트 채팅방 소켓과 서버 연결될 경우 채팅창에 소켓 정보 출력
}


//에어컨관리UI
void ChatWindow::airconManageUi(QHBoxLayout *mainLayout){

    airconLayout = new QVBoxLayout();

    // 온도 조절 레이아웃 (아이콘 + 텍스트)
    QHBoxLayout *tempLayout = new QHBoxLayout();
    tempLayout->setContentsMargins(0, 0, 0, 0);  // 텍스트와 아이콘 사이의 여백을 최소화

    // 온도 조절 PNG 아이콘
    QLabel *tempIconLabel = new QLabel(this);
    QPixmap tempPixmap("temp.png");  // 아이콘 경로 설정
    QPixmap scaledTempPixmap = tempPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 아이콘 크기 조정
    tempIconLabel->setPixmap(scaledTempPixmap);

    // 온도 조절 라벨
    QLabel *tempLabel = new QLabel("온도조절", this);
    // 온도 조절 레이아웃에 아이콘과 텍스트 추가
    tempLayout->addWidget(tempIconLabel);  // 왼쪽에 아이콘 추가
    tempLayout -> addStretch();
    tempLayout->addWidget(tempLabel);      // 오른쪽에 텍스트 추가

    //온도조절 다이얼
    temperatureDial = new QDial(this);
    temperatureDial -> setRange(16,30); //온도범위
    temperatureDial ->setValue(22); //디폴트값
    //temperatureDial ->setFixedSize(100,100);//다이얼크기조절
    temperatureDial -> setNotchesVisible(true);
    temperatureDial -> setNotchTarget(1);

    airconLayout->addLayout(tempLayout);
    airconLayout->addWidget(temperatureDial, 0, Qt::AlignTop);


    //바람세기조절 레이아웃 (아이콘 +텍스트)
    QHBoxLayout *windLayout = new QHBoxLayout();
    windLayout->setContentsMargins(0, 0, 0, 0);  // 텍스트와 아이콘 사이의 여백을 최소화

    // 바람세기 PNG 아이콘
    QLabel *windIconLabel = new QLabel(this);
    QPixmap windPixmap("wind.png");  // 바람세기 아이콘 경로 설정
    QPixmap scaledWindPixmap = windPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 아이콘 크기 조정
    windIconLabel->setPixmap(scaledWindPixmap);

    QLabel *windLabel = new QLabel("바람세기", this);
    windLayout->addWidget(windIconLabel);  // 왼쪽에 아이콘 추가
    windLayout->addWidget(windLabel);      // 오른쪽에 텍스트 추가

    //바람세기 조절
    windStrengthComboBox = new QComboBox(this);
    windStrengthComboBox ->addItems({"1단", "2단","3단","4단","5단"});

    // 바람세기 레이아웃 추가
    airconLayout->addLayout(windLayout);  // 바람세기 레이아웃 추가
    airconLayout->addWidget(windStrengthComboBox); // 바람세기 콤보박스 추가

    mainLayout->addLayout(airconLayout);

    // Connect signals for aircon management
    connect(temperatureDial, &QDial::valueChanged, this, &ChatWindow::onTemperatureChanged);
    connect(windStrengthComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChatWindow::onWindStrengthChanged);

}

void ChatWindow::onTemperatureChanged(int value) {
    chatDisplay->append(QString("에어컨 온도 조절: %1°C").arg(value));
    // Send this value to the server if needed
}

void ChatWindow::onWindStrengthChanged(int index) {
    chatDisplay->append(QString("바람 세기 조절: %1단").arg(index + 1));
    // Send this value to the server if needed
}
ChatWindow::~ChatWindow() {
    // 자원 해제 시 필요한 작업 (기본적으로 Qt가 자동으로 관리)
}

void ChatWindow::sendMessage() {
    QString message = messageInput->text();                                                         // 입력된 메시지 가져오기
    if (!message.isEmpty()) {
        QString displayMessage = QString("%1[%2] : %3").arg(username).arg(index).arg(message);      // 인덱스(채팅방 번호)를 포함한 메시지 형식

        if (socket && socket->state() == QTcpSocket::ConnectedState) {                              // 서버로 메시지 전송
            socket->write(displayMessage.toUtf8());                                                 // 메시지를 UTF-8로 변환 후 전송
        }
        else {
            chatDisplay->append("서버에 연결되지 않았습니다.");                                         // 연결 오류 메시지 표시
        }
        messageInput->clear();                                                                      // 메세지 전송 후 입력란 비우기
    }
}
void ChatWindow::sendFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "파일 선택", "", "모든 파일 (*)");           // 파일 대화상자를 열어 파일 선택
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {                                                        // 파일 내용을 읽어서 전송
            QByteArray fileData = file.readAll();                                                    // 파일 내용을 바이트 배열로 읽음
            file.close();                                                                            // 파일 닫기

            // 서버로 파일 데이터 전송
            if (socket && socket->state() == QTcpSocket::ConnectedState) {                           // 서버와 연결되어 있으면
                socket->write(fileData);                                                             // 파일 데이터 전송
                chatDisplay->append(QString("파일 전송 완료: %1").arg(fileName));                      // 성공 메시지 표시
            }
            else {
                chatDisplay->append("서버에 연결되지 않았습니다.");                                      // 연결 오류 메시지 표시
            }
        }
        else {
            chatDisplay->append("파일을 열 수 없습니다.");                                              // 파일 열기 오류 메시지 표시
        }
    }
}


void ChatWindow::onReadyRead() {                                                                    // 서버로부터 메시지를 수신할 때 호출됨
    QByteArray data = socket->readAll();                                                            // 서버에서 소켓 통해서 데이터 받아오기
    QString message = QString::fromUtf8(data).trimmed().replace("\n", "").replace("\r", "");        // 모든 줄바꿈 제거
    chatDisplay->append(message);                                                                   // append() 사용 시 자동 줄바꿈

}


void ChatWindow::onConnected() {
    QString socketInfo = QString("소켓 상태: %1\n서버 주소: %2\n포트 번호: %3")
                             .arg(socket->state())
                             .arg(socket->peerAddress().toString())
                             .arg(socket->peerPort());
    chatDisplay->append(socketInfo);                                                                  // 소켓 상태 채팅창에 출력
}


void ChatWindow::closeEvent(QCloseEvent *event) {                                                     // 채팅방 닫을때 시그널 발생
    emit chatWindowClosed(index);                                                                     // 시그널 발생 (닫힌 채팅방의 인덱스 보내줌)
    QWidget::closeEvent(event);                                                                       // QWidget의 closeEvent를 호출하여
                                                                                                      // 기본적인 윈도우 닫기 동작을 수행
}
