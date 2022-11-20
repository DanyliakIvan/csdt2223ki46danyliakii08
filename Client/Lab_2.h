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
#include <QMessageBox>

#include <QDate>
#include <QTime>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QJsonObject>

#include "check_correctness.h"

class CSDT : public QWidget {

public:
    CSDT(QWidget *parent = 0);
    ~CSDT();

private:
    QLabel *label1, *label2, *labelERROR;
    QLineEdit *textWrite;
    QPushButton *sendButton, *openButton, *closeButton, *openDBButton;
    QComboBox *comboBox;
    QTextBrowser *textShow;

    QString outputBuffer, receiveBuffer;

    QSerialPort *serial;

    QSqlDatabase db;
    int serialNumberOfTheRequest = 1;

    QJsonObject JSON;


    void sendMessage(QString);
    void receiveMessage();
    void PREreceiveMessage();
    QString preparingTextForOutput(QString, QString);
    bool errorCheckingWithCOM();
    void SendDataToDB();

private slots:
    void OnSendPressed();
    void OnOpenPressed();
    void OnClosePressed();
    void OnConectionToDB();
};
