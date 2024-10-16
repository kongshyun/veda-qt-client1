#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QString>
#include <QFileDialog>
#include <QLabel>
#include <QDial>
#include <QComboBox>
#include <QFileDialog>
#include <QStackedLayout>
#include <QHostAddress>

class ChatWindow : public QWidget {
    Q_OBJECT
public:
    ChatWindow(const QString &username, const QString &password, QTcpSocket *socket, int index, QWidget *parent = nullptr);
    ~ChatWindow();  // 소멸자 선언

private slots:
    void sendMessage();  // 메세지 서버로 전송
    void sendFile();     // 파일 서버로 전송
    void onReadyRead();  // 서버로부터 echo 데이터 수신
    void onConnected();  // 채팅방과 서버가 연결될 경우 수행

    void onTemperatureChanged(int value);  //온도 변할경우 송신
    void onWindStrengthChanged(int index); //바람 세기 변할경우 송신

signals:
    void chatWindowClosed(int index);  // 채팅창이 닫힐 때 발생하는 시그널 (closeEvent로부터 발생한 시그널)

private:
    void closeEvent(QCloseEvent *event);    // 채팅방 닫을 때 발생하는 메소드
    void setupUi();                         // UI 설정

    void chatUi(QHBoxLayout *mainLayout);
    void airconManageUi(QHBoxLayout *mainLayout);

    int index;                 // 채팅방 번호(채팅방1 or 채팅방2)

    QHBoxLayout * mainLayout;
    QVBoxLayout * chatLayout;
    QVBoxLayout * airconLayout;

    QTextEdit *chatDisplay;    // 채팅 내용을 보여주는 텍스트 박스
    QLineEdit *messageInput;   // 메시지를 입력하는 텍스트 박스
    QPushButton *sendButton;    // 메시지 전송 버튼
    QPushButton *sendFileButton;// 파일 전송 버튼

    QDial *temperatureDial;
    QComboBox *windStrengthComboBox;

    QTcpSocket *socket;         // 소켓
    QString username;           // 사용자 이름
    QString password;           // 비밀번호
};

#endif // CHATWINDOW_H
