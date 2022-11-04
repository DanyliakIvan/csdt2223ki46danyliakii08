#include "Lab_2.h"

CSDT::CSDT(QWidget *parent)
    : QWidget(parent)
{
    //creating an instances labels
    label1 = new QLabel("Communicate", this);
    label2 = new QLabel("Send", this);
    labelERROR = new QLabel(this);

    //seting fonts for labels
    label1->setFont(QFont("MS Shell diq 2", 12));
    label2->setFont(QFont("MS Shell diq 2", 12));    
    labelERROR->setFont(QFont("MS Shell diq 2", 12));

    //seting geometry for labels
    label1->setGeometry(20, 20, 120, 20);
    label2->setGeometry(20, 220, 50, 20);
    labelERROR->setGeometry(20, 300, 200, 30);


    //creating fild for sending message
    textWrite = new QLineEdit(this);
    textWrite->setGeometry(20, 250, 300, 30);
    textWrite->setFont(QFont("MS Shell diq 2", 12));

    //creating comboBox for choice COM port
    comboBox = new QComboBox(this);
    comboBox->setGeometry(350, 50, 70, 30);
    comboBox->setFont(QFont("MS Shell diq 2", 10));
    for (int i = 0; i < 100; i++)
        comboBox->addItem("COM" + QString::number(i));

    //creating fild for showing sended and received text
    textShow = new QTextBrowser(this);
    textShow->setGeometry(20, 50, 300, 150);
    textShow->setFont(QFont("MS Shell diq 2", 10));
    textShow->ensureCursorVisible();

    //creating some buttons for controling COM ports and sending messages
    sendButton = new QPushButton("Send", this);
    openButton = new QPushButton("Open", this);
    closeButton = new QPushButton("Close", this);

    sendButton->setGeometry(350, 250, 70, 30);
    openButton->setGeometry(350, 110, 70, 30);
    closeButton->setGeometry(350, 160, 70, 30);

    sendButton->setFont(QFont("MS Shell diq 2", 12));
    openButton->setFont(QFont("MS Shell diq 2", 12));
    closeButton->setFont(QFont("MS Shell diq 2", 12));

    //connecting buttons and functions
    connect(sendButton, &QPushButton::clicked, this, &CSDT::OnSendPressed);
    connect(openButton, &QPushButton::clicked, this, &CSDT::OnOpenPressed);
    connect(closeButton, &QPushButton::clicked, this, &CSDT::OnClosePressed);

    //creating an instance for working with COM port
    serial = new QSerialPort(this);
}


void CSDT::OnSendPressed()
{
    QString message;
    message = textWrite->text();
    sendMessage(message);
    serial->waitForReadyRead(300);

    outputBuffer = preparingTextForOutput(message, "sended");
    outputBuffer += preparingTextForOutput(receiveBuffer, "received");

    textShow->append(outputBuffer);

    receiveBuffer = "";
}

void CSDT::OnOpenPressed()
{
    QString COMPort = comboBox->currentText();
    serial->setPortName(COMPort);

    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!serial->open(QIODevice::ReadWrite))
    {
        labelERROR->setText("Failed to open!");
        QMessageBox::critical(this, "SERIAL PORT NOT CONNECTED", "Unable to connect to this COM port.\n\nPlease check your connections\nand try again.");
    }
    else
    {
        connect(serial, &QSerialPort::readyRead, this, &CSDT::receiveMessage);
        //connect(serial, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
        labelERROR->setText("Connected!");
    }
    QByteArray dataBA = serial->readAll();
}

void CSDT::OnClosePressed()
{
    labelERROR->setText("Disconnected!");
    serial->close();
    if(serial->isOpen())
    {
        QMessageBox::critical(this, "SERIAL PORT NOT DISCONNECTED", "Unable to disconnect to this COM port.\n\nPlease restart program\nand try again.");
    }
}

void CSDT::sendMessage(QString message)
{
    serial->write(message.toUtf8());
    serial->waitForBytesWritten(50);
}

void CSDT::receiveMessage()
{
    QByteArray dataBA = serial->readLine();
    QString message(dataBA);

    receiveBuffer += message;
}

QString CSDT::preparingTextForOutput(QString message, QString sendedOrReceive)
{
    QString outputMessage = "";

    outputMessage.append("[" + QDate::currentDate().toString(Qt::ISODate));
    outputMessage.append("] [");
    outputMessage.append(QTime::currentTime().toString(Qt::ISODate));
    outputMessage.append((sendedOrReceive == "sended")?("] [sended]:  "):("] [received]: "));
    outputMessage.append(message);
    if(sendedOrReceive == "sended")
        outputMessage.append("\n");

    return outputMessage;
}

CSDT::~CSDT()
{
    delete serial;
}
