#ifndef LAB_2_H
#define LAB_2_H

#endif // LAB_2_H

#pragma once

#include <QCoreApplication>
#include <QProcess>
#include <QRandomGenerator>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <iostream>
#include <windows.h>

#define KEY_UP 38 //72
#define KEY_DOWN 40 //80
#define KEY_LEFT 37 //75
#define KEY_RIGHT 39  //77

using namespace std;

class CSDT : public QObject
{
    Q_OBJECT
public:
    CSDT(QObject *parent = 0);
    ~CSDT();
    void mainWindow();


private:
    int SCORE = 0;

    bool pressedUp = false;
    bool pressedDown = false;
    bool pressedLeft = false;
    bool pressedRight = false;

    int heightFromCapToDinoHead = 20; // не використовуємо на платі

    int jumping = 0;

    int arrPlacementOfCactus[6] = {252, 1, 252, 1, 252, 1}; //3 cactus and 3 type of cactus (small or big) FE: XtXtXt
    int previousPlacementOfFirstCactus = arrPlacementOfCactus[0];

    uchar byteForSending;

    QString receivedMessage;

    QSerialPort *serial = new QSerialPort();

    void cap();
    void instruction();
    void controlingPressedKeys();
    void controlingDraw();
    void controlingDrawCactus();
    void drawDino();
    void drawSmallCactus(int placementOfCactusX);
    void drawBigCactus(int placementOfCactusX);
    void gotoXY(int x, int y);

    void connectionToCOM();
    void decodeData();
    void codeData();
    void sendData();
    void checkForRestartOrDisapperenceOfCactus();

private slots:
    void receiveData();
};
