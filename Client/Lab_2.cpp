#include "Lab_2.h"

CSDT::CSDT(QWidget *parent)
    : QWidget(parent)
{
    label1 = new QLabel("Communicate", this);
    label2 = new QLabel("Send", this);

    label1->setFont(QFont("MS Shell diq 2", 12));
    label2->setFont(QFont("MS Shell diq 2", 12));

    label1->setGeometry(20, 20, 120, 20);
    label2->setGeometry(20, 220, 50, 20);


    textWrite = new QLineEdit(this);

    textWrite->setGeometry(20, 250, 300, 30);
    textWrite->setFont(QFont("MS Shell diq 2", 12));


    comboBox = new QComboBox(this);

    comboBox->setGeometry(350, 50, 70, 30);
    comboBox->setFont(QFont("MS Shell diq 2", 10));

    for (int i = 0; i < 100; i++)
        comboBox->addItem("COM" + QString::number(i));


    textShow = new QTextBrowser(this);
    textShow->setGeometry(20, 50, 300, 150);
    textShow->setFont(QFont("MS Shell diq 2", 12));


    sendButton = new QPushButton("Send", this);
    openButton = new QPushButton("Open", this);
    closeButton = new QPushButton("Close", this);

    sendButton->setGeometry(350, 250, 70, 30);
    openButton->setGeometry(350, 110, 70, 30);
    closeButton->setGeometry(350, 160, 70, 30);

    sendButton->setFont(QFont("MS Shell diq 2", 12));
    openButton->setFont(QFont("MS Shell diq 2", 12));
    closeButton->setFont(QFont("MS Shell diq 2", 12));

    connect(sendButton, &QPushButton::clicked, this, &CSDT::OnSendPressed);
    connect(openButton, &QPushButton::clicked, this, &CSDT::OnOpenPressed);
    connect(closeButton, &QPushButton::clicked, this, &CSDT::OnClosePressed);

}


void CSDT::OnSendPressed()
{

}

void CSDT::OnOpenPressed()
{

}

void CSDT::OnClosePressed()
{

}

/*
CSDT(QWidget *parent = 0);
    QLabel *label1, *label2, *label3;
    QLineEdit *textWtite;
    QPushButton *sendButton, *openButton, *closeButton;
    QComboBox *comboBox;
    QTextBrowser *textShow;

void OnSendPressed();
void OnOpenPressed();
void OnClosePressed();
*/
