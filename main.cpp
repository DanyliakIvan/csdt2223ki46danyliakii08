#include "Dino.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CSDT csdt;
    csdt.mainWindow();

    return a.exec();
}

CSDT::CSDT(QObject *parent)
    : QObject(parent)
{

}

CSDT::~CSDT()
{
    serial->close();
    delete serial;
}

void CSDT::mainWindow()
{
    int choice;
    while(cap(), cout << "1 - Play\n2 - Instrustions\n3 - Connecting to COM\n0 - Exit\n\nEnter your choice: ", cin >> choice, choice != 0)
    {
        switch(choice)
        {
            case 0:
            {
                system("CLS");
                return;
            }
            case 1:
            {
                system("CLS");
                controlingPressedKeys();
                break;
            }
            case 2:
            {
                system("CLS");
                instruction();
                break;
            }
            case 3:
            {
                system("CLS");
                connectionToCOM();
                break;
            }
            default:
            {
                cout << "\nWHERE DO YOU SEE THIS NUMBER?! \nI SEE YOU CANNOT READ...\nWell.Try again.\n\n";
                Sleep(3000);
                system("CLS");
                break;
            }
        }
    }
}

void CSDT::cap()
{
    cout << "\n---------------------------\n";
    cout << "| DANYLIAK'S DINOREX GAME |\n";
    cout << "---------------------------\n\n";
}

void CSDT::instruction()
{
    cap();

    cout << "\030 or [SPACE] - Your DinoRex will jump once\n";
    cout << "\031 - Your DinoRex will land immediately\n";
    cout << "-> - Your DinoRex will increase his speed a bit\n"; //\032
    cout << "<- - Your DinoRex will decrease his speed a bit\n\n";//\033

    cout << "There should be other instructions\n\n";

    int exit;
    while(cout <<"Type \"0\" to exit: ", cin >> exit, exit != 0) {}
    system("CLS");
}

void CSDT::controlingPressedKeys()
{
    HANDLE hStdin;
    const short bufferSize = 128;
    INPUT_RECORD irInBuf[bufferSize];

    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (hStdin == INVALID_HANDLE_VALUE)
    {
        cout << "ERROR: GetStdHabdle" << endl;
    }

    //int index = 0;
    //int eventCount = 0;

    while (1)
    {
        DWORD cNumRead = 0;
        BOOL peekSuccessful = PeekConsoleInput(
            hStdin, irInBuf, bufferSize, &cNumRead
        );

        if (!peekSuccessful)
        {
            cout << "ERROR: PeekConsoleInput" << endl;
        }

        if (!FlushConsoleInputBuffer(hStdin))
        {
            cout << "ERROR: FlushConsoleInputBuffer" << endl;
        }


        for (DWORD i = 0; i < cNumRead; i++)
        {
            if (irInBuf[i].EventType == KEY_EVENT)
            {
                KEY_EVENT_RECORD ker = irInBuf[i].Event.KeyEvent;
                if (ker.bKeyDown)
                {
                    switch (ker.wVirtualKeyCode)
                    {
                        case KEY_UP:
                            pressedUp = true;
                            break;
                        case KEY_DOWN:
                            pressedDown = true;
                            break;
                        case KEY_LEFT:
                            pressedLeft = true;
                            break;
                        case KEY_RIGHT:
                            pressedRight = true;
                            break;
                    }
                }
            }
        }
        controlingDraw();
        //Sleep(1);
    }
}

void CSDT::controlingDraw()
{
    codeData();
    sendData();

    receiveData();
    decodeData();

    checkForRestartOrDisapperenceOfCactus();

    system("CLS");

    cap();


    cout << "SCORE: " << SCORE++ << "\n\n\n";

    receivedMessage = "";
    drawDino();
    controlingDrawCactus();

    previousPlacementOfFirstCactus = arrPlacementOfCactus[0];
}

void CSDT::controlingDrawCactus()
{
    for(int i = 0; i < 6; i += 2)
    {
        if(arrPlacementOfCactus[i] != 214) //prevoius ( != 252)
        {
            if(arrPlacementOfCactus[i + 1] == 1)
                drawBigCactus(arrPlacementOfCactus[i]);
            else
                drawSmallCactus(arrPlacementOfCactus[i]);
        }
    }
}

void CSDT::drawDino()
{

    gotoXY(0, heightFromCapToDinoHead - jumping);
    cout << "            XXXXXXXX\n";
    cout << "           XX XXXXXXX\n";
    cout << "           XXXXXXXXXX\n";
    cout << "           XXXX\n";
    cout << "           XXXXXXXXX\n";
    cout << "           XXXX\n";
    cout << "X         XXXXX\n";
    cout << "XX      XXXXXXXXX\n";
    cout << "XXX    XXXXXXXX X\n";
    cout << "XXXXXXXXXXXXXXX\n";
    cout << " XXXXXXXXXXXXX\n";
    cout << "  XXXXXXXXXXX\n";
    cout << "    XXXXXXXX\n";
    cout << "     XXX  XX\n";
    cout << "     X     X\n";
    cout << "     XXX   XXX\n";

    gotoXY(0, heightFromCapToDinoHead + 16);
    for(int i = 0; i < 215; i++)
        cout << "_";
}


void CSDT::drawSmallCactus(int placementOfCactusX)
{

    gotoXY(placementOfCactusX, 13 + heightFromCapToDinoHead);
    cout << "|||";
    gotoXY(placementOfCactusX, 14 + heightFromCapToDinoHead);
    cout << "\\|/";
    gotoXY(placementOfCactusX, 15 + heightFromCapToDinoHead);
    cout << " |";
}

void CSDT::drawBigCactus(int placementOfCactusX)
{
    gotoXY(placementOfCactusX, 11 + heightFromCapToDinoHead);
    cout << "| | |";
    gotoXY(placementOfCactusX, 12 + heightFromCapToDinoHead);
    cout << "| |_/";
    gotoXY(placementOfCactusX, 13 + heightFromCapToDinoHead);
    cout << "\\_|";
    gotoXY(placementOfCactusX, 14 + heightFromCapToDinoHead);
    cout << "  |";
    gotoXY(placementOfCactusX, 15 + heightFromCapToDinoHead);
    cout << "  |";

}

void CSDT::gotoXY(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD CursorPosition;

    CursorPosition.X = x;
    CursorPosition.Y = y;

    SetConsoleCursorPosition(console, CursorPosition);
}


void CSDT::connectionToCOM()
{
    cap();

    serial->setPortName("COM5");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!serial->open(QIODevice::ReadWrite))
    {
        cout << "\n\n\nUnable to connect to this COM port.\n\nPlease check your connections\nand try again.\n\n\n";
        exit(0);
    }
    else
    {
        cout << "\n\n\nConnection to COM port was successfully.\nLet's go.\n\n\n";
        connect(serial, &QSerialPort::readyRead, this, &CSDT::receiveData);
        Sleep(500);
        system("CLS");
    }
}

void CSDT::receiveData()
{
    serial->waitForReadyRead(200);
    QByteArray dataBA(serial->readLine());

    receivedMessage += dataBA;
}

void CSDT::decodeData()
{
    QByteArray res = receivedMessage.toLocal8Bit();
    unsigned char *test1 = (unsigned char *)strdup(res.constData());

    jumping = (int)test1[0] - 100;
    arrPlacementOfCactus[0] = (int)test1[1] * 1.7;
    arrPlacementOfCactus[2] = (int)test1[2] * 1.7;
    arrPlacementOfCactus[4] = (int)test1[3] * 1.7;

    char fifthByte = test1[4] - 50;

    arrPlacementOfCactus[5] = fifthByte % 2;
    fifthByte = fifthByte >> 1;
    arrPlacementOfCactus[3] = fifthByte % 2;
    fifthByte = fifthByte >> 1;
    arrPlacementOfCactus[1] = fifthByte % 2;
    fifthByte = fifthByte >> 1;

    pressedRight = fifthByte % 2;
    fifthByte = fifthByte >> 1;
    pressedLeft = fifthByte % 2;
    fifthByte = fifthByte >> 1;
    pressedDown = fifthByte % 2;
    fifthByte = fifthByte >> 1;
    pressedUp = fifthByte % 2;
}

void CSDT::codeData()
{
    byteForSending = 0;
    byteForSending += pressedUp;
    byteForSending = byteForSending << 1;
    byteForSending += pressedDown;
    byteForSending = byteForSending << 1;
    byteForSending += pressedLeft;
    byteForSending = byteForSending << 1;
    byteForSending += pressedRight;
}

void CSDT::sendData()
{
    QString message = ""; message += byteForSending;
    serial->write(message.toUtf8(), 1);
    serial->waitForBytesWritten(20);
}

void CSDT::checkForRestartOrDisapperenceOfCactus()
{

    if(previousPlacementOfFirstCactus < 10 && arrPlacementOfCactus[0] > 20)
    {
        SCORE += 100;
    }
    else if(previousPlacementOfFirstCactus < 20 && arrPlacementOfCactus[0] > 200)
    {
        system("CLS");
        cap();
        cout << "YOU LOST!\n YOUR SCORE - " << SCORE << "\nTHE GAME WILL BE RESTARTED IN 3 SECONDS!\n\n\n";
        //restartGame();
        SCORE = 0;
        Sleep(3000);
    }

}
