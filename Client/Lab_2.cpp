#include "Lab_2.h"
//#include "check_correctness.h"


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
    textWrite->setGeometry(20, 250, 600, 30);
    textWrite->setFont(QFont("MS Shell diq 2", 12));
    textWrite->setPlaceholderText("Less than 30 symbols!");

    //creating comboBox for choice COM port
    comboBox = new QComboBox(this);
    comboBox->setGeometry(650, 50, 70, 30);
    comboBox->setFont(QFont("MS Shell diq 2", 10));
    for (int i = 0; i < 100; i++)
        comboBox->addItem("COM" + QString::number(i));

    //creating fild for showing sended and received text
    textShow = new QTextBrowser(this);
    textShow->setGeometry(20, 50, 600, 150);
    textShow->setFont(QFont("Consolas", 10));
    textShow->ensureCursorVisible();

    //creating some buttons for controling COM ports and sending messages
    sendButton = new QPushButton("Send", this);
    openButton = new QPushButton("Open", this);
    closeButton = new QPushButton("Close", this);
    openDBButton = new QPushButton("Connect to DB", this);

    sendButton->setGeometry(650, 250, 70, 30);
    openButton->setGeometry(650, 110, 70, 30);
    closeButton->setGeometry(650, 170, 70, 30);
    openDBButton->setGeometry(650, 280, 70, 30);

    sendButton->setFont(QFont("MS Shell diq 2", 12));
    openButton->setFont(QFont("MS Shell diq 2", 12));
    closeButton->setFont(QFont("MS Shell diq 2", 12));
    openDBButton->setFont(QFont("MS Shell diq 2", 12));

    //connecting buttons and functions
    connect(sendButton, &QPushButton::clicked, this, &CSDT::OnSendPressed);
    connect(openButton, &QPushButton::clicked, this, &CSDT::OnOpenPressed);
    connect(closeButton, &QPushButton::clicked, this, &CSDT::OnClosePressed);
    connect(openDBButton, &QPushButton::clicked, this, &CSDT::OnConectionToDB);

    //creating an instance for working with COM port
    serial = new QSerialPort(this);
}


void CSDT::OnSendPressed()
{
    QString message;
    message = textWrite->text();

    //check message for correctness

    QString receiveCheckedMessage = checkmessage(message);

    /*if(receiveCheckedMessage != message)
    {
         QMessageBox::critical(this, "MESSAGE IS NOT CORRECT", "Error detalis:\n\n" + receiveCheckedMessage + ".");
         return;
    }*/

    //if(errorCheckingWithCOM()) return;

    sendMessage(message);
    outputBuffer = preparingTextForOutput(message, "sended");
    serial->waitForReadyRead(350);
    outputBuffer += preparingTextForOutput(receiveBuffer, "received");

    SendDataToDB();
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

    //errorCheckingWithCOM();

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
    //errorCheckingWithCOM();

    serial->write(message.toUtf8());
    serial->waitForBytesWritten(50);
}

void CSDT::receiveMessage()
{
    QByteArray dataBA = serial->readLine();
    QString message(dataBA);

    receiveBuffer += message;
}

bool CSDT::errorCheckingWithCOM()
{
    if(!serial->open(QIODevice::ReadWrite))
    {
        labelERROR->setText("Failed to open!");
        QMessageBox::critical(this, "SERIAL PORT NOT CONNECTED", "Unable to connect to this COM port.\n\nPlease check your connections\nand try again.");
        return 1;
    }
    else
    {
        connect(serial, &QSerialPort::readyRead, this, &CSDT::receiveMessage);
        labelERROR->setText("Connected!");
        return 0;
    }
}

QString CSDT::preparingTextForOutput(QString message, QString sendedOrReceive)
{
    QString outputMessage = "";

    outputMessage.append("[" + QDate::currentDate().toString(Qt::ISODate));
    outputMessage.append("] [");
    outputMessage.append(QTime::currentTime().toString(Qt::ISODate));
    outputMessage.append((sendedOrReceive == "sended")?("] [sended]:   "):("] [received]: "));
    outputMessage.append(message);

    if(sendedOrReceive == "sended")
    {
        JSON.insert("SerialNumber", serialNumberOfTheRequest++);
        JSON.insert("DataIn", QDate::currentDate().toString(Qt::ISODate));
        JSON.insert("TimeIn", QTime::currentTime().toString(Qt::ISODate));
        JSON.insert("MessageIn", message);

        outputMessage.append("\n");
    }
    else
    {
        JSON.insert("DataOut", QDate::currentDate().toString(Qt::ISODate));
        JSON.insert("TimeOut", QTime::currentTime().toString(Qt::ISODate));
        JSON.insert("MessageOut", message);
    }

    return outputMessage;
}

void CSDT::OnConectionToDB()
{
    db = QSqlDatabase::addDatabase("QODBC");

    db.setConnectOptions();

    db.setDatabaseName("DRIVER={SQL Server};SERVER=VANO\\MSSQLSERVER01;DATABASE=ForCSDT;UID=sa;Trusted_Connection=Yes;");

    if(db.open())
        labelERROR->setText("Connected");
    else
        labelERROR->setText("The database is not connected");
}

void CSDT::SendDataToDB()
{
    QSqlQuery * query = new QSqlQuery();
    query->prepare("INSERT INTO task4_db_for_CSDT_Danyliak(DataIn,TimeIn,MessageIn,DataOut,TimeOut,MessageOut) "
                   "VALUES(:DataIn,:TimeIn,:MessageIn,:DataOut,:TimeOut,:MessageOut)");

    query->bindValue(":DataIn", JSON["DataIn"].toString());
    query->bindValue(":TimeIn", JSON["TimeIn"].toString());
    query->bindValue(":MessageIn", JSON["MessageIn"].toString());
    query->bindValue(":DataOut", JSON["DataOut"].toString());
    query->bindValue(":TimeOut", JSON["TimeOut"].toString());
    query->bindValue(":MessageOut", JSON["MessageOut"].toString());

    if(!query->exec())
    {
        QMessageBox::critical(this, "The data was not writen in DB", "Looks like something went wrong.\n\nPlease check your connections\nand try again.");
    }
}

CSDT::~CSDT()
{
    delete label1;
    delete label2;
    delete labelERROR;
    delete textWrite;
    delete sendButton;
    delete openButton;
    delete closeButton;
    delete openDBButton;
    delete comboBox;
    delete textShow;
    delete serial;
}
