#ifndef LAB_2_H
#define LAB_2_H

#endif // LAB_2_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTextBrowser>

#include <QString>

#include <QDate>
#include <QTime>
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class CSDT : public QWidget {

public:
    CSDT(QWidget *parent = 0);
    ~CSDT();

private:
    QLabel *label1, *label2, *labelERROR;
    QLineEdit *textWrite;
    QPushButton *sendButton, *openButton, *closeButton;
    QComboBox *comboBox;
    QTextBrowser *textShow;

    QString outputBuffer, receiveBuffer;

    QSerialPort *serial;


    void sendMessage(QString);
    void receiveMessage();
    void PREreceiveMessage();
    QString preparingTextForOutput(QString, QString);
    bool errorCheckingWithCOM();

private slots:
    void OnSendPressed();
    void OnOpenPressed();
    void OnClosePressed();
};
